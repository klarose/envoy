#include "envoy/network/address.h"

#include "common/network/utility.h"

#include "extensions/filters/network/original_src/original_src_socket_option.h"

#include "test/mocks/common.h"
#include "test/mocks/network/mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using testing::_;
using testing::Eq;

namespace Envoy {
namespace Network {

// TODO(klarose): fix namespace

class OriginalSrcSocketOptionTest : public testing::Test {
public:
  std::unique_ptr<OriginalSrcSocketOption>
  makeOptionByAddress(const Address::InstanceConstSharedPtr& address) {
    return std::make_unique<OriginalSrcSocketOption>(address);
  }

protected:
  NiceMock<MockConnectionSocket> socket_;
  std::vector<uint8_t> key_;
};

TEST_F(OriginalSrcSocketOptionTest, TestSetOptionPreBindSetsAddress) {
  const auto address = Utility::parseInternetAddress("127.0.0.2");
  auto option = makeOptionByAddress(address);
  EXPECT_CALL(socket_, setLocalAddress(PointeesEq(address), Eq(false)));
  EXPECT_EQ(option->setOption(socket_, envoy::api::v2::core::SocketOption::STATE_PREBIND), true);
}

TEST_F(OriginalSrcSocketOptionTest, TestSetOptionPreBindSetsAddressSecond) {
  const auto address = Utility::parseInternetAddress("1.2.3.4");
  auto option = makeOptionByAddress(address);
  EXPECT_CALL(socket_, setLocalAddress(PointeesEq(address), Eq(false)));
  EXPECT_EQ(option->setOption(socket_, envoy::api::v2::core::SocketOption::STATE_PREBIND), true);
}

TEST_F(OriginalSrcSocketOptionTest, TestSetOptionNotPrebindDoesNotSetAddress) {
  const auto address = Utility::parseInternetAddress("1.2.3.4");
  auto option = makeOptionByAddress(address);
  EXPECT_CALL(socket_, setLocalAddress(_, _)).Times(0);
  EXPECT_EQ(option->setOption(socket_, envoy::api::v2::core::SocketOption::STATE_LISTENING), true);
}

TEST_F(OriginalSrcSocketOptionTest, TestIpv4HashKey) {
  const auto address = Utility::parseInternetAddress("1.2.3.4");
  auto option = makeOptionByAddress(address);
  option->hashKey(key_);

  std::vector<uint8_t> expected_key = {OriginalSrcSocketOption::IPV4_KEY};
  // 12 bytes of 0-padding so we have a constant size
  expected_key.insert(expected_key.end(), 12, 0);
  // The ip address broken into big-endian octets.
  expected_key.insert(expected_key.end(), {1, 2, 3, 4});
  EXPECT_EQ(key_, expected_key);
}

TEST_F(OriginalSrcSocketOptionTest, TestIpv4HashKeyOther) {
  const auto address = Utility::parseInternetAddress("255.254.253.0");
  auto option = makeOptionByAddress(address);
  option->hashKey(key_);

  std::vector<uint8_t> expected_key = {OriginalSrcSocketOption::IPV4_KEY};
  // 12 bytes of 0-padding so we have a constant size
  expected_key.insert(expected_key.end(), 12, 0);
  // The ip address broken into big-endian octets.
  expected_key.insert(expected_key.end(), {255, 254, 253, 0});
  EXPECT_EQ(key_, expected_key);
}

TEST_F(OriginalSrcSocketOptionTest, TestIpv6HashKey) {
  const auto address = Utility::parseInternetAddress("102:304:506:708:90a:b0c:d0e:f00");
  auto option = makeOptionByAddress(address);
  option->hashKey(key_);

  std::vector<uint8_t> expected_key = {OriginalSrcSocketOption::IPV6_KEY};
  expected_key.insert(expected_key.end(), {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb,
                                           0xc, 0xd, 0xe, 0xf, 0x0});
  EXPECT_EQ(key_, expected_key);
}

TEST_F(OriginalSrcSocketOptionTest, TestIpv6HashKeyOther) {
  const auto address = Utility::parseInternetAddress("F02:304:519:708:90a:b0e:FFFF:0000");
  auto option = makeOptionByAddress(address);
  option->hashKey(key_);

  std::vector<uint8_t> expected_key = {OriginalSrcSocketOption::IPV6_KEY};
  expected_key.insert(expected_key.end(), {0xF, 0x2, 0x3, 0x4, 0x5, 0x19, 0x7, 0x8, 0x9, 0xa, 0xb,
                                           0xe, 0xff, 0xff, 0x0, 0x0});
  EXPECT_EQ(key_, expected_key);
}

} // namespace Network
} // namespace Envoy
