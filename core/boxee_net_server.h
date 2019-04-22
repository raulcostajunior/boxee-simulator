#ifndef BOXEE_NET_SERVER_H
#define BOXEE_NET_SERVER_H

#include <cstdint>
#include <QString>

namespace core {

class BoxeeNetServer
{
public:
    void startScanListener();

    void stopScanListener();

    void startRequestListener(uint16_t port, const QString &password);

    void stopRequestListener();

private:
    // TODO: add udp scan handler

    // TODO: add http request handler
};

} // namespace core

#endif // BOXEE_NET_SERVER_H
