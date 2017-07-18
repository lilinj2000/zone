// Copyright (c) 2010
// All rights reserved.

#ifndef ZONE_SERVER_HH
#define ZONE_SERVER_HH

#include <map>
#include <string>
#include "zero/TraderService.hh"
#include "Config.hh"
#include "air/TimeStampDataFile.hh"

namespace zone {

typedef std::map<int, air::TimeStampData*> TimeStampRecords;

class Server :
      public zero::TraderServiceCallback {
 public:
  Server(Options* options,
         soil::Options* trader_options);

  virtual ~Server();

  virtual void onRspError(int errord_id, const std::string& error_msg);

  virtual void onRspOrderInsert(int order_ref);

  virtual void onRtnOrder(
      int order_ref,
      const std::string& order_status,
      const std::string& status_msg);

  virtual void onRtnTrade(
      int order_ref,
      double price, int volume);

  void run();

 private:
  Options* options_;
  std::unique_ptr<zero::TraderService> trader_service_;
  TimeStampRecords records_;
  std::unique_ptr<air::TimeStampDataFile> timestamp_file_;
};

}  // namespace zone

#endif
