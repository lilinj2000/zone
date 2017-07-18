// Copyright (c) 2010
// All rights reserved.

#include "Server.hh"
#include "Log.hh"
#include "soil/STimer.hh"

namespace zone {

Server::Server(Options* options,
               soil::Options* trader_options) :
    options_(options) {
  ZONE_TRACE <<"Server::Server()";

  trader_service_.reset(
      zero::TraderService::createService(
          trader_options,
          this));

  if (!options_->timestamp_file.empty() )
    timestamp_file_.reset(
        new air::TimeStampDataFile(
            options_->timestamp_file));
}

Server::~Server() {
  ZONE_TRACE <<"Server::~Server()";
}

void Server::onRspError(
    int errord_id,
    const std::string& error_msg) {
  ZONE_TRACE <<"Server::onRspError()";
}

void Server::onRspOrderInsert(int order_ref) {
  ZONE_TRACE <<"Server::onRspOrderInsert()";

  ZONE_DEBUG <<"order_ref is " <<order_ref;

  auto it = records_.find(order_ref);
  if (it != records_.end()) {
    it->second->updateT1();

    timestamp_file_->putData(it->second);
    records_.erase(it);
  }
}

void Server::onRtnOrder(
    int order_ref,
    const std::string& order_status,
    const std::string& status_msg) {
  ZONE_TRACE <<"Server::onRtnOrder()";

  ZONE_DEBUG <<"order_ref = " <<order_ref
             <<"; order_status = " <<order_status
             <<"; status_msg = " <<status_msg;

  auto it = records_.find(order_ref);

  if (it != records_.end()) {
    if (order_status == "a") {
      it->second->updateT1();
    } else if (order_status == "5") {
      it->second->updateT2();
      timestamp_file_->putData(it->second);
      records_.erase(it);
    } else {
      ZONE_ERROR <<"the order status is wrong, order_status " <<order_status;
    }
  } else {
    ZONE_ERROR <<"unexpected the rtn order, the order ref is " <<order_ref;
  }
}

void Server::onRtnTrade(
    int order_ref,
    double price,
    int volume) {
  ZONE_TRACE <<"Server::onRtnTrade()";

  ZONE_DEBUG <<"order_ref = " <<order_ref
             <<"; price = " <<price
             <<"; volume = " <<volume;
}

void Server::run() {
  ZONE_TRACE <<"Server::run()";

  int counter = 0;

  std::unique_ptr<soil::STimer> cond;
  cond.reset(soil::STimer::create());

  do {
    int order_ref = -1;

    if (options_->is_buy) {
      order_ref = trader_service_->orderOpenBuyFOK(
          options_->instrument_id,
          options_->price,
          options_->volume);
    } else {
      order_ref = trader_service_->orderOpenSellFOK(
          options_->instrument_id,
          options_->price,
          options_->volume);
    }

    records_[order_ref] = new air::TimeStampData(order_ref);

    ++counter;

    if (options_->order_counter > 0
        && counter >= options_->order_counter)
      break;

    cond->wait(options_->order_interval);
  }while(true);

  cond->wait(1000);
}

}  // namespace zone
