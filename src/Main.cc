// Copyright (c) 2010
// All rights reserved.

#include <memory>
#include "Config.hh"
#include "Server.hh"

int main(int argc, char* argv[]) {
  std::unique_ptr<zone::Config> config;
  config.reset(new zone::Config(argc, argv));

  zone::Options* options = config->options();

  std::unique_ptr<zone::Server> server;
  server.reset(new zone::Server(options, config->zeroTraderOptions()));

  server->run();

  return 0;
}
