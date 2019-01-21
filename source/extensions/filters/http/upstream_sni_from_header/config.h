#pragma once

#include "envoy/config/filter/http/upstream_sni_from_header/v2/upstream_sni_from_header.pb.h"
#include "envoy/config/filter/http/upstream_sni_from_header/v2/upstream_sni_from_header.pb.validate.h"

#include "extensions/filters/http/common/factory_base.h"
#include "extensions/filters/http/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace UpstreamSniFromHeaderFilter {

/**
 * Config registration for the header-to-metadata filter. @see NamedHttpFilterConfigFactory.
 */
class UpstreamSniFromHeaderConfig
    : public Common::FactoryBase<envoy::config::filter::http::upstream_sni_from_header::v2::Config> {
public:
  UpstreamSniFromHeaderConfig() : FactoryBase(HttpFilterNames::get().UpstreamSniFromHeader) {}

private:
  Http::FilterFactoryCb createFilterFactoryFromProtoTyped(
      const envoy::config::filter::http::upstream_sni_from_header::v2::Config& proto_config,
      const std::string& stats_prefix, Server::Configuration::FactoryContext& context) override;
};

} // namespace UpstreamSniFromHeaderFilter
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
