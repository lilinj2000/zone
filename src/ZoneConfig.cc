#include "ZoneConfig.hh"
#include "ZoneLog.hh"
#include "zero/TraderService.hh"

#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>

namespace zone
{

ZoneOptions::ZoneOptions():
    config_options_("ZoneConfigOptions")
{

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
       "timestamp file")
      ;

  return;
  
}

ZoneOptions::~ZoneOptions()
{
}

po::options_description* ZoneOptions::configOptions()
{
  return &config_options_;
}

ZoneConfig::ZoneConfig(int argc, char* argv[])
{
  zone_options_.reset(new ZoneOptions());
  zero_trader_options_.reset( zero::TraderService::createOptions() );
      
  std::unique_ptr<soil::Config> config( soil::Config::create() );
  config->registerOptions( zone_options_.get() );
  config->registerOptions( zero_trader_options_.get() );

  config->configFile() = "zone.cfg";
  config->loadConfig(argc, argv);
  
  // init the log
  ZONE_LOG_INIT( zone_options_->log_cfg );
  
  return;
}

ZoneConfig::~ZoneConfig()
{
}

};  
