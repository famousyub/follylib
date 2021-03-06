/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <folly/io/async/AsyncSocket.h>
#include <folly/io/async/EventBase.h>
#include <folly/portability/GMock.h>

namespace folly {

namespace test {

class MockAsyncSocket : public AsyncSocket {
 public:
  typedef std::unique_ptr<MockAsyncSocket, Destructor> UniquePtr;

  explicit MockAsyncSocket(EventBase* base) : AsyncSocket(base) {}

  MOCK_METHOD6(
      connect_,
      void(
          AsyncSocket::ConnectCallback*,
          const folly::SocketAddress&,
          int,
          const folly::SocketOptionMap&,
          const folly::SocketAddress&,
          const std::string&));
  void connect(
      AsyncSocket::ConnectCallback* callback,
      const folly::SocketAddress& address,
      int timeout,
      const folly::SocketOptionMap& options,
      const folly::SocketAddress& bindAddr,
      const std::string& ifName) noexcept override {
    connect_(callback, address, timeout, options, bindAddr, ifName);
  }

  MOCK_CONST_METHOD1(getPeerAddress, void(folly::SocketAddress*));
  MOCK_METHOD0(detachNetworkSocket, NetworkSocket());
  MOCK_CONST_METHOD0(getNetworkSocket, NetworkSocket());
  MOCK_METHOD0(closeNow, void());
  MOCK_CONST_METHOD0(good, bool());
  MOCK_CONST_METHOD0(readable, bool());
  MOCK_CONST_METHOD0(hangup, bool());
  MOCK_CONST_METHOD1(getLocalAddress, void(SocketAddress*));
  MOCK_METHOD1(setReadCB, void(ReadCallback*));
  MOCK_METHOD1(_setPreReceivedData, void(std::unique_ptr<IOBuf>&));
  MOCK_CONST_METHOD0(getRawBytesWritten, size_t());
  MOCK_METHOD4(setSockOptVirtual, int(int, int, void const*, socklen_t));
  MOCK_METHOD1(setErrMessageCB, void(AsyncSocket::ErrMessageCallback*));
  MOCK_METHOD1(setSendMsgParamCB, void(AsyncSocket::SendMsgParamsCallback*));
  MOCK_CONST_METHOD0(getSecurityProtocol, std::string());
  void setPreReceivedData(std::unique_ptr<IOBuf> data) override {
    return _setPreReceivedData(data);
  }

  MOCK_METHOD1(
      addLifecycleObserver,
      void(folly::AsyncTransport::LifecycleObserver* observer));
  MOCK_METHOD1(
      removeLifecycleObserver,
      bool(folly::AsyncTransport::LifecycleObserver* observer));
  MOCK_CONST_METHOD0(
      getLifecycleObservers, std::vector<AsyncTransport::LifecycleObserver*>());
};

} // namespace test
} // namespace folly
