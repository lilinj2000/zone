// Copyright (c) 2010
// All rights reserved.

#ifndef ZONE_CONFIG_HH
#define ZONE_CONFIG_HH

#include <string>
#include <memory>
#include "soil/Config.hh"

namespace zone {

namespace po = boost::program_options;

class Options : public soil::Options {
 public:
  Options();
  virtual ~Options();

  virtual po::options_description* configOptions();

  std::string instrument_id;
  double price;
  int volume;
  bool is_buy;

  int order_counter;
  int order_interval;

  std::string log_cfg;

  std::string timestamp_file;

 private:
  boost::program_options::options_description config_options_;
};

class Config {
 public:
  explicit Config(int argc = 0, char* argv[] = nullptr);

  ~Config();

  Options* options() {
    return options_.get();
  }

  soil::Options* zeroTraderOptions() {
    return zero_trader_options_.get();
  }

 private:
  std::unique_ptr<Options> options_;
  std::unique_ptr<soil::Options> zero_trader_options_;
};

}  // namespace zone

#endif
