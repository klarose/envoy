licenses(["notice"])  # Apache 2

# HTTP L7 filter that transforms request data into dynamic metadata
# Public docs: docs/root/configuration/http_filters/upstream_sni_from_header_filter.rst

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_library(
    name = "upstream_sni_from_header_filter_lib",
    srcs = ["upstream_sni_from_header_filter.cc"],
    hdrs = ["upstream_sni_from_header_filter.h"],
    deps = [
        "//include/envoy/server:filter_config_interface",
        "//source/common/network:transport_socket_options_lib",
        "//source/extensions/filters/http:well_known_names",
        "@envoy_api//envoy/config/filter/http/upstream_sni_from_header/v2:upstream_sni_from_header_cc",
    ],
)

envoy_cc_library(
    name = "config",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    deps = [
        "//include/envoy/registry",
        "//source/common/protobuf:utility_lib",
        "//source/extensions/filters/http:well_known_names",
        "//source/extensions/filters/http/common:factory_base_lib",
        "//source/extensions/filters/http/upstream_sni_from_header:upstream_sni_from_header_filter_lib",
    ],
)
