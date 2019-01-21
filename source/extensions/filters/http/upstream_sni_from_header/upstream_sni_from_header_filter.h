#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "envoy/config/filter/http/upstream_sni_from_header/v2/upstream_sni_from_header.pb.h"
#include "envoy/server/filter_config.h"

#include "common/common/logger.h"

#include "absl/strings/string_view.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace UpstreamSniFromHeaderFilter {

typedef envoy::config::filter::http::upstream_sni_from_header::v2::Config::Rule Rule;
typedef envoy::config::filter::http::upstream_sni_from_header::v2::Config::ValueType ValueType;
typedef std::vector<std::pair<Http::LowerCaseString, Rule>> UpstreamSniFromHeaderRules;

/**
 *  Encapsulates the filter configuration with STL containers and provides an area for any custom
 *  configuration logic.
 */
class Config : public Logger::Loggable<Logger::Id::config> {
public:
  Config(const envoy::config::filter::http::upstream_sni_from_header::v2::Config config);

  UpstreamSniFromHeaderRules requestRules() const { return request_rules_; }
  UpstreamSniFromHeaderRules responseRules() const { return response_rules_; }
  bool doResponse() const { return response_set_; }
  bool doRequest() const { return request_set_; }

private:
  typedef Protobuf::RepeatedPtrField<Rule> ProtobufRepeatedRule;

  UpstreamSniFromHeaderRules request_rules_;
  UpstreamSniFromHeaderRules response_rules_;
  bool response_set_;
  bool request_set_;

  /**
   *  configToVector is a helper function for converting from configuration (protobuf types) into
   *  STL containers for usage elsewhere.
   *
   *  @param config A protobuf repeated field of metadata that specifies what headers to convert to
   *         metadata
   *  @param vector A vector that will be populated with the configuration data from config
   *  @return true if any configuration data was added to the vector, false otherwise. Can be used
   *          to validate whether the configuration was empty.
   */
  static bool configToVector(const ProtobufRepeatedRule&, UpstreamSniFromHeaderRules&);

  const std::string& decideNamespace(const std::string& nspace) const;
};

typedef std::shared_ptr<Config> ConfigSharedPtr;

/**
 *  Upstream SNI from Header examines request/response headers and creates transport socket options
 *  to set the upstream SNI from them.
 */
class UpstreamSniFromHeaderFilter : public Http::StreamFilter,
                               public Logger::Loggable<Logger::Id::filter> {
public:
  UpstreamSniFromHeaderFilter(const ConfigSharedPtr config);
  ~UpstreamSniFromHeaderFilter();

  // Http::StreamFilterBase
  void onDestroy() override {}

  // StreamDecoderFilter
  Http::FilterHeadersStatus decodeHeaders(Http::HeaderMap& headers, bool) override;
  Http::FilterDataStatus decodeData(Buffer::Instance&, bool) override {
    return Http::FilterDataStatus::Continue;
  }
  Http::FilterTrailersStatus decodeTrailers(Http::HeaderMap&) override {
    return Http::FilterTrailersStatus::Continue;
  }
  void setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callbacks) override;

  // StreamEncoderFilter
  Http::FilterHeadersStatus encode100ContinueHeaders(Http::HeaderMap&) override {
    return Http::FilterHeadersStatus::Continue;
  }
  Http::FilterHeadersStatus encodeHeaders(Http::HeaderMap& headers, bool) override;
  Http::FilterDataStatus encodeData(Buffer::Instance&, bool) override {
    return Http::FilterDataStatus::Continue;
  }
  Http::FilterTrailersStatus encodeTrailers(Http::HeaderMap&) override {
    return Http::FilterTrailersStatus::Continue;
  }
  Http::FilterMetadataStatus encodeMetadata(Http::MetadataMap&) override {
    return Http::FilterMetadataStatus::Continue;
  }
  void setEncoderFilterCallbacks(Http::StreamEncoderFilterCallbacks& callbacks) override;

private:

  const ConfigSharedPtr config_;
  Http::StreamDecoderFilterCallbacks* decoder_callbacks_{};
  Http::StreamEncoderFilterCallbacks* encoder_callbacks_{};

  /**
   *  writeSni encapsulates (1) searching for the header and (2) writing it to the
   *  request metadata.
   *  @param headers the map of key-value headers to look through. These could be response or
   *                 request headers depending on whether this is called from the encode state or
   *                 decode state.
   *  @param rules the header-to-metadata mapping set in configuration.
   *  @param callbacks the callback used to fetch the StreamInfo (which is then used to get
   *                   metadata). Callable with both encoder_callbacks_ and decoder_callbacks_.
   */
  void writeSni(Http::HeaderMap& headers, const UpstreamSniFromHeaderRules& rules,
                             Http::StreamFilterCallbacks& callbacks);
  void addSniOption(const absl::string_view& new_cni);
};

} // namespace UpstreamSniFromHeaderFilter
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
