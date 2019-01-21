#include "extensions/filters/http/upstream_sni_from_header/upstream_sni_from_header_filter.h"

#include "common/config/well_known_names.h"
#include "common/network/transport_socket_options_impl.h"
#include "common/protobuf/protobuf.h"

#include "extensions/filters/http/well_known_names.h"

#include "absl/strings/numbers.h"
#include "absl/strings/string_view.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace UpstreamSniFromHeaderFilter {
namespace {

const uint32_t MAX_HEADER_VALUE_LEN = 100;

} // namespace

Config::Config(const envoy::config::filter::http::upstream_sni_from_header::v2::Config config) {
  request_set_ = Config::configToVector(config.request_rules(), request_rules_);
  response_set_ = Config::configToVector(config.response_rules(), response_rules_);

  // don't allow an empty configuration
  if (!response_set_ && !request_set_) {
    throw new EnvoyException("Must at least specify either response or request config");
  }
}

bool Config::configToVector(const ProtobufRepeatedRule& proto_rules,
                            UpstreamSniFromHeaderRules& vector) {
  if (proto_rules.size() == 0) {
    ENVOY_LOG(debug, "no rules provided");
    return false;
  }

  for (const auto& entry : proto_rules) {
    std::pair<Http::LowerCaseString, Rule> rule = {Http::LowerCaseString(entry.header()), entry};

    // Rule must have at least one of the `on_header_*` fields set.
    if (!entry.has_on_header_present() && !entry.has_on_header_missing()) {
      const auto& error = fmt::format("header to metadata filter: rule for header '{}' has neither "
                                      "`on_header_present` nor `on_header_missing` set",
                                      entry.header());
      throw EnvoyException(error);
    }

    vector.push_back(rule);
  }

  return true;
}

UpstreamSniFromHeaderFilter::UpstreamSniFromHeaderFilter(const ConfigSharedPtr config) : config_(config) {}

UpstreamSniFromHeaderFilter::~UpstreamSniFromHeaderFilter() {}

Http::FilterHeadersStatus UpstreamSniFromHeaderFilter::decodeHeaders(Http::HeaderMap& headers, bool) {
  if (config_->doRequest()) {
    writeSni(headers, config_->requestRules(), *decoder_callbacks_);
  }

  return Http::FilterHeadersStatus::Continue;
}

void UpstreamSniFromHeaderFilter::setDecoderFilterCallbacks(
    Http::StreamDecoderFilterCallbacks& callbacks) {
  ENVOY_LOG(debug, "Setting decoder callbacks");
  decoder_callbacks_ = &callbacks;
}

Http::FilterHeadersStatus UpstreamSniFromHeaderFilter::encodeHeaders(Http::HeaderMap& headers, bool) {
  if (config_->doResponse()) {
    writeSni(headers, config_->responseRules(), *encoder_callbacks_);
  }
  return Http::FilterHeadersStatus::Continue;
}

void UpstreamSniFromHeaderFilter::setEncoderFilterCallbacks(
    Http::StreamEncoderFilterCallbacks& callbacks) {
  encoder_callbacks_ = &callbacks;
  ENVOY_LOG(debug, "Setting encoder callbacks");
}

void UpstreamSniFromHeaderFilter::writeSni(Http::HeaderMap& headers,
                                       const UpstreamSniFromHeaderRules& rules,
                                       Http::StreamFilterCallbacks&) {
  for (const auto& rulePair : rules) {
    const auto& header = rulePair.first;
    const auto& rule = rulePair.second;
    const Http::HeaderEntry* header_entry = headers.get(header);

    if (header_entry != nullptr && rule.has_on_header_present()) {
      const auto& keyval = rule.on_header_present();
      absl::string_view value =
          keyval.value().empty() ? header_entry->value().getStringView() : keyval.value();

      if (!value.empty()) {
        addSniOption(value);
        ENVOY_LOG(info, "Changing value to {}", value);
      } else {
        ENVOY_LOG(info, "value is empty, not changing header");
      }

      if (rule.remove()) {
        headers.remove(header);
      }
    } else if (rule.has_on_header_missing()) {
      // Add metadata for the header missing case.
      const auto& keyval = rule.on_header_missing();

      if (!keyval.value().empty()) {
        addSniOption(keyval.value());
      } else {
        ENVOY_LOG(info, "value is empty, not changing header");
      }
    }
  }
}

void UpstreamSniFromHeaderFilter::addSniOption(const absl::string_view& new_sni) {
  auto option = std::make_shared<Network::TransportSocketOptionsImpl>(new_sni);
  encoder_callbacks_->streamInfo().addTransportSocketOption(std::move(option));
}

} // namespace UpstreamSniFromHeaderFilter
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
