#include "extensions/filters/http/upstream_sni_from_header/config.h"

#include <string>

#include "envoy/config/filter/http/upstream_sni_from_header/v2/upstream_sni_from_header.pb.validate.h"
#include "envoy/registry/registry.h"

#include "common/protobuf/utility.h"

#include "extensions/filters/http/upstream_sni_from_header/upstream_sni_from_header_filter.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace UpstreamSniFromHeaderFilter {

Http::FilterFactoryCb UpstreamSniFromHeaderConfig::createFilterFactoryFromProtoTyped(
    const envoy::config::filter::http::upstream_sni_from_header::v2::Config& proto_config,
    const std::string&, Server::Configuration::FactoryContext&) {
  ConfigSharedPtr filter_config(std::make_shared<Config>(proto_config));

  return [filter_config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
    callbacks.addStreamFilter(
        Http::StreamFilterSharedPtr{new UpstreamSniFromHeaderFilter(filter_config)});
  };
}

/**
 * Static registration for the header-to-metadata filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<UpstreamSniFromHeaderConfig,
                                 Server::Configuration::NamedHttpFilterConfigFactory>
    register_;

} // namespace UpstreamSniFromHeaderFilter
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
