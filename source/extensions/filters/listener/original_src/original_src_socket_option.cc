#include "extensions/filters/listener/original_src/original_src_socket_option.h"

#include <arpa/inet.h>

#include "common/common/assert.h"
#include "common/network/socket_option_factory.h"

namespace Envoy {
namespace Extensions {
namespace ListenerFilters {
namespace OriginalSrc {

constexpr uint8_t OriginalSrcSocketOption::IPV4_KEY;
constexpr uint8_t OriginalSrcSocketOption::IPV6_KEY;

OriginalSrcSocketOption::OriginalSrcSocketOption(
    Network::Address::InstanceConstSharedPtr src_address, uint32_t mark)
    : src_address_(std::move(src_address)) {
  // Source transparency only works on IP connections.
  ASSERT(src_address_->type() == Network::Address::Type::Ip);

  auto mark_option = Network::SocketOptionFactory::buildSocketMarkOptions(mark);
  options_to_apply_.insert(options_to_apply_.end(), mark_option->begin(), mark_option->end());
  auto transparent_option = Network::SocketOptionFactory::buildIpTransparentOptions();
  options_to_apply_.insert(options_to_apply_.end(), transparent_option->begin(),
                           transparent_option->end());
}

bool OriginalSrcSocketOption::setOption(
    Network::Socket& socket, envoy::api::v2::core::SocketOption::SocketState state) const {

  if (state == envoy::api::v2::core::SocketOption::STATE_PREBIND) {
    socket.setLocalAddress(src_address_);
  }

  bool result = true;
  std::for_each(options_to_apply_.begin(), options_to_apply_.end(),
                [&socket, state](const Network::Socket::OptionConstSharedPtr& option) {
                  option->setOption(socket, state);
                });
  return result;
}

/**
 * Inserts an address, already in network order, to a byte array.
 */
template <typename T> void addressIntoVector(std::vector<uint8_t>& vec, const T& address) {
  const uint8_t* byte_array = reinterpret_cast<const uint8_t*>(&address);
  vec.insert(vec.end(), byte_array, byte_array + sizeof(T));
}

void OriginalSrcSocketOption::hashKey(std::vector<uint8_t>& key) const {
  /* We do two things here to ensure that there isn't any ambiguity when combining the hash key with
   * variable length options:
   * 1. Ensure the the key is fixed length by padding ipv4 addresses up to 16 bytes with 0s.
   * 2. Enusre that Ipv6 addresses cannot collide with padded v4 addresses by placing a unique value
   *    before the address.
   */
  if (src_address_->ip()->version() == Network::Address::IpVersion::v4) {
    key.push_back(IPV4_KEY);
    // padding
    key.insert(key.end(), 12, 0);
    // note raw_address is already in network order
    uint32_t raw_address = src_address_->ip()->ipv4()->address();
    addressIntoVector(key, raw_address);
  } else if (src_address_->ip()->version() == Network::Address::IpVersion::v6) {
    key.push_back(IPV6_KEY);
    // note raw_address is already in network order
    absl::uint128 raw_address = src_address_->ip()->ipv6()->address();
    addressIntoVector(key, raw_address);
  }
}

absl::optional<Network::Socket::Option::Details>
OriginalSrcSocketOption::getOptionDetails(const Network::Socket&,
                                          envoy::api::v2::core::SocketOption::SocketState) const {
  // TODO(klarose): The option details stuff will likely require a bit of a rework when we actually
  // put options in here to support multiple options at once. Sad.
  NOT_IMPLEMENTED_GCOVR_EXCL_LINE
  return absl::nullopt; // nothing right now.
}

} // namespace OriginalSrc
} // namespace ListenerFilters
} // namespace Extensions
} // namespace Envoy
