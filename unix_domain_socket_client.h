// Copyright(c) 2023-present, Casper Pedersen.
// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <netinet/in.h>
#include <spdlog/common.h>
#include <spdlog/details/os.h>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

class unix_domain_socket_client {
  static constexpr int TX_BUFFER_SIZE = 1024 * 10;
  int socket_ = -1;
  struct sockaddr_un unixSockAddr_;

  void cleanup_() {
    if (socket_ != -1) {
      ::close(socket_);
      socket_ = -1;
    }
  }

public:
  unix_domain_socket_client(const std::string &unix_dgram_sock) {
    socket_ = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_ < 0) {
      spdlog::throw_spdlog_ex("error: Create Socket Failed!");
    }

    if (unix_dgram_sock.size() > sizeof(unixSockAddr_.sun_path) - 1) {
      spdlog::throw_spdlog_ex("error: Path to long!");
    }

    int option_value = TX_BUFFER_SIZE;
    if (::setsockopt(socket_, SOL_SOCKET, SO_SNDBUF,
                     reinterpret_cast<const char *>(&option_value),
                     sizeof(option_value)) < 0) {
      cleanup_();
      spdlog::throw_spdlog_ex("error: setsockopt(SO_SNDBUF) Failed!");
    }

    ::memset(&unixSockAddr_, 0, sizeof(unixSockAddr_));
    unixSockAddr_.sun_family = AF_UNIX;
    ::strncpy(unixSockAddr_.sun_path, unix_dgram_sock.data(),
              sizeof(unixSockAddr_.sun_path) - 1);

    if (connect(socket_, (struct sockaddr *)&unixSockAddr_,
                sizeof(struct sockaddr_un)) == -1) {
      cleanup_();
      spdlog::throw_spdlog_ex("error: connect Failed!");
    }
  }

  ~unix_domain_socket_client() { cleanup_(); }

  int fd() const { return socket_; }

  void send(const char *data, size_t n_bytes) {
    if (::write(socket_, data, n_bytes) != static_cast<ssize_t>(n_bytes)) {
      spdlog::throw_spdlog_ex("write failed", errno);
    }
  }
};
