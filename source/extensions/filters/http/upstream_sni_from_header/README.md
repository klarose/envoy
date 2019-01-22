# Upstream SNI from Header

This filter sets the upstream SNI from a downstream HTTP ver. It works by adding a transport
socket option to the StreamInfo. The StreamInfo is exposed by the LoadBalancerContext, which is
used when creating the http connection pool get the socket options. These are passed to the
connection pool, which will use them when creating its upstream connections. Since the socket option
is used to create the hash key, there will be one unique pool per SNI.

## Alternatives

- Rather than creating the socket option directly in the filter, I considered setting the sni filter
  state from there, then using that to create the socket option.
- It doesn't really make sense to set the SNI on each newStream to the connection pool, since that
  would require the pool to map its connections to SNIs. Tricky.

## Example Config

Using the below config, we can run the following curl and have it forward to generic upstreams:

`curl http://localhost:8888/ -H "Host: www.example.com" -H "x-envoy-original-dst-host: 93.184.216.34:443" -H "x-new-sni: www.example.com"`

```text
static_resources:
  listeners:
  - address:
      socket_address:
        address: 0.0.0.0
        port_value: 8888
    filter_chains:
    - filters:
      - name: envoy.http_connection_manager
        config:
          codec_type: auto
          use_remote_address: true
          xff_num_trusted_hops: 1
          stat_prefix: ingress_http
          route_config:
            name: generic_route
            # Necessary to prevent redirects on the upstream
            request_headers_to_remove:
             - "x-forwarded-proto"
            virtual_hosts:
            - name: to_generic
              domains:
              - "*"
              routes:
              - match:
                  prefix: "/"
                route:
                  cluster: upstream_generic
          http_filters:
          - name: envoy.filters.http.upstream_sni_from_header
            config:
             request_rules:
              - header: "x-new-sni"
                on_header_present:
                  key: "blah"
                  type: "STRING"
                remove: true
          - name: envoy.router
            config: {}
  clusters:
  - name: upstream_generic
    connect_timeout: 2s
    type: original_dst
    original_dst_lb_config:
      use_http_header: true
    lb_policy: original_dst_lb
    tls_context:
      common_tls_context:
        validation_context:  # Without this a bad SNI may go unnoticed
          trusted_ca:
            filename: /etc/ssl/certs/ca-certificates.crt
admin:
  access_log_path: "/dev/null"
  address:
    socket_address:
      address: 0.0.0.0
      port_value: 8001
```

Note the comments in the config.
