// Copyright (c) 2010
// All rights reserved.

#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

#include "Config.hh"
#include "Log.hh"
#include "zero/TraderService.hh"

namespace zone {

Options::Options():
    config_options_("ZoneConfigOptions") {
  namespace po = boost::program_options;

  config_options_.add_options()
      ("zone.instrument_id", po::value<std::string>(&instrument_id),
       "instrument id")
      ("zone.price", po::value<double>(&price),
       "price")
      ("zone.volume", po::value<int>(&volume),
       "volume")
      ("zone.is_buy", po::value<bool>(&is_buy),
       "is buy")
      ("zone.order_counter", po::value<int>(&order_counter),
       "order counter")
      ("zone.order_interval", po::value<int>(&order_interval),
       "order interval")
      ("zone.log_cfg", po::value<std::string>(&log_cfg),
       "log config file")

      ("zone.timestamp_file", po::value<std::string>(&timestamp_file),
       "timestamp file");

  return;
}

Options::~Options() {
}

po::options_description* Options::configOptions() {
  return &config_options_;
}

Config::Config(int argc, char* argv[]) {
  options_.reset(new Options());
  zero_trader_options_.reset(
      zero::TraderService::createOptions());

  std::unique_ptr<soil::Config> config(soil::Config::create());
  config->registerOptions(options_.get());
  config->registerOptions(zero_trader_options_.get());

  config->configFile() = "zone.cfg";
  config->loadConfig(argc, argv);

  // init the log
  ZONE_LOG_INIT(options_->log_cfg);

  return;
}

Config::~Config() {
}

}  // namespace zone
