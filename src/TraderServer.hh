#ifndef ZONE_TRADER_SERVER_HH
#define ZONE_TRADER_SERVER_HH

#include "zero/TraderService.hh"
#include "ZoneConfig.hh"
#include "air/TimeStampDataFile.hh"

namespace zone
{

typedef std::map<int, air::TimeStampData*> TimeStampRecords;

class TraderServer : public zero::TraderServiceCallback
{
 public:
  TraderServer(ZoneOptions* zone_options, soil::Options* trader_options);
               
  virtual ~TraderServer();

  virtual void onRspError(int errord_id, const std::string& error_msg);
  
  virtual void onRspOrderInsert(int order_ref);

  virtual void onRtnOrder(int order_ref, const std::string& order_status, const std::string& status_msg);

  virtual void onRtnTrade(int order_ref,
                          double price, int volume);

  void run();
  
 private:

  ZoneOptions* zone_options_;
  
  std::unique_ptr<zero::TraderService> trader_service_;

  TimeStampRecords records_;

  std::unique_ptr<air::TimeStampDataFile> timestamp_file_;
};

};

#endif
