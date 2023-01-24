#include "spdlog/spdlog.h"
#include "unix_domain_socket_sink.h"
#include <iostream>
#include <vector>

// Test with - nc -lUk /tmp/log.sock

void setupLogging(std::vector<spdlog::sink_ptr> &sinks) {
  try {
    auto sink = std::make_shared<unix_domain_socket_sink_mt>("/tmp/log.sock");

    // https://pkg.go.dev/github.com/kr/logfmt?utm_source=godoc
    sink->set_pattern("time=%Y-%m-%d %T.%e name=%n level=%l msg=\"%v\"");
    sinks.push_back(sink);

    auto logger = std::make_shared<spdlog::logger>(
        "ToLogOrNotToLog", std::begin(sinks), std::end(sinks));
    logger->set_level(spdlog::level::debug);
    spdlog::register_logger(logger);
  } catch (const spdlog::spdlog_ex &ex) {
    std::cout << "Log initialization failed: " << ex.what() << std::endl;
  }
}

int main() {
  std::vector<spdlog::sink_ptr> sinks;
  setupLogging(sinks);

  auto logger = spdlog::get("ToLogOrNotToLog");
  logger->info("A message");

  return 0;
}
