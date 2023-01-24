// Copyright(c) 2023-present, Casper Pedersen.
// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include "unix_domain_socket_client.h"
#include <spdlog/common.h>
#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>

#include <chrono>
#include <functional>
#include <mutex>
#include <string>

// Simple UNIX domain socket dgram client sink

template <typename Mutex>
class unix_domain_socket_sink : public spdlog::sinks::base_sink<Mutex> {
public:
  explicit unix_domain_socket_sink(std::string unix_dgram_sock)
      : client_{unix_dgram_sock} {}

  ~unix_domain_socket_sink() override = default;

protected:
  void sink_it_(const spdlog::details::log_msg &msg) override {
    spdlog::memory_buf_t formatted;
    spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
    client_.send(formatted.data(), formatted.size());
  }

  void flush_() override {}
  unix_domain_socket_client client_;
};

using unix_domain_socket_sink_mt = unix_domain_socket_sink<std::mutex>;
using unix_domain_socket_sink_st =
    unix_domain_socket_sink<spdlog::details::null_mutex>;
