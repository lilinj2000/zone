#include <memory>
#include "ZoneConfig.hh"
#include "TraderServer.hh"

int main(int argc, char* argv[])
{

  std::unique_ptr<zone::ZoneConfig> config;
  config.reset(new zone::ZoneConfig(argc, argv));

  zone::ZoneOptions* zone_options = config->zoneOptions();

  std::unique_ptr<zone::TraderServer> server;
  server.reset(new zone::TraderServer(zone_options, config->zeroTraderOptions()));

  server->run();
  
  return 0;
}
