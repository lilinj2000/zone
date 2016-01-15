#ifndef ZONE_CONFIG_HH
#define ZONE_CONFIG_HH

#include <string>
#include <memory>
#include "soil/Config.hh"

namespace zone
{

namespace po = boost::program_options;

class ZoneOptions : public soil::Options
{
 public:

  ZoneOptions();
  
  virtual ~ZoneOptions();

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

class ZoneConfig
{
 public:
  
  ZoneConfig(int argc=0, char* argv[]=NULL);
  ~ZoneConfig();

  ZoneOptions* zoneOptions()
  {
    return zone_options_.get();
  }

  soil::Options* zeroTraderOptions()
  {
    return zero_trader_options_.get();
  }

 private:
  std::unique_ptr<ZoneOptions> zone_options_;
  
  std::unique_ptr<soil::Options> zero_trader_options_;

};

}  


#endif 
