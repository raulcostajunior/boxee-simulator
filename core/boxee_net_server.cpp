#include "boxee_net_server.h"

using namespace core;

void BoxeeNetServer::startScanListener()
{
    // TODO: add method body
}

void BoxeeNetServer::stopScanListener()
{
    // TODO: add method body
}

void BoxeeNetServer::startRequestListener(uint16_t /*port*/, const QString & /*password*/)
{
    // TODO: add method body

    _requestListenerActive = true;
}

void BoxeeNetServer::stopRequestListener()
{
    // TODO: add method body

    _requestListenerActive = false;
}
