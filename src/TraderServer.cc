#include "TraderServer.hh"
#include "ZoneLog.hh"

#include "soil/STimer.hh"

namespace zone
{

TraderServer::TraderServer(ZoneOptions* zone_options, soil::Options* trader_options):
    zone_options_(zone_options)
{
  ZONE_TRACE <<"TraderServer::TraderServer()";

  trader_service_.reset( zero::TraderService::createService(trader_options, this) );

  if( !zone_options->timestamp_file.empty() )
    timestamp_file_.reset( new air::TimeStampDataFile(zone_options->timestamp_file) );

}

TraderServer::~TraderServer()
{
  ZONE_TRACE <<"TraderServer::~TraderServer()";
}

void TraderServer::onRspError(int errord_id, const std::string& error_msg)
{
  ZONE_TRACE <<"TraderServer::onRspError()";
}

void TraderServer::onRspOrderInsert(int order_ref)
{
  ZONE_TRACE <<"TraderServer::onRspOrderInsert()";

  ZONE_DEBUG <<"order_ref is " <<order_ref;

  auto it = records_.find(order_ref);
  if (it != records_.end()) {
    it->second->updateT1();

    timestamp_file_->putData( it->second );
    records_.erase( it );
  }
}

void TraderServer::onRtnOrder(int order_ref, const std::string& order_status, const std::string& status_msg)
{
  ZONE_TRACE <<"TraderServer::onRtnOrder()";

  ZONE_DEBUG <<"order_ref = " <<order_ref
             <<"; order_status = " <<order_status
             <<"; status_msg = " <<status_msg;

  auto it = records_.find(order_ref);

  if( it!=records_.end() )
  {
    if( order_status=="a" )
    {
      it->second->updateT1();
    }
    else if( order_status=="5" )
    {
      it->second->updateT2();

      timestamp_file_->putData( it->second );

      records_.erase( it );
    }
    else
    {
      ZONE_ERROR <<"the order status is wrong, order_status " <<order_status;
    }
  }
  else
  {
    ZONE_ERROR <<"unexpected the rtn order, the order ref is " <<order_ref;
  }
    
}

void TraderServer::onRtnTrade(int order_ref, double price, int volume)
{
  ZONE_TRACE <<"TraderServer::onRtnTrade()";

  ZONE_DEBUG <<"order_ref = " <<order_ref
             <<"; price = " <<price
             <<"; volume = " <<volume;
}

void TraderServer::run()
{
  ZONE_TRACE <<"TraderServer::run()";

  int counter = 0;

  std::unique_ptr<soil::STimer> cond;
  cond.reset( soil::STimer::create() );


  do
  {
    int order_ref = -1;

    if( zone_options_->is_buy )
    {
      order_ref = trader_service_->orderOpenBuyFOK(
          zone_options_->instrument_id,
          zone_options_->price,
          zone_options_->volume);
    }
    else
    {
      order_ref = trader_service_->orderOpenSellFOK(
          zone_options_->instrument_id,
          zone_options_->price,
          zone_options_->volume);
    }

    records_[order_ref] = new air::TimeStampData(order_ref);
    
    ++counter;

    if( zone_options_->order_counter>0 && counter>=zone_options_->order_counter )
      break;
      
    cond->wait(zone_options_->order_interval);
      
  }while(true);

  cond->wait(1000);
}

};
