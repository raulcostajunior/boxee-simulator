#ifndef BOXEE_NET_SERVER_H
#define BOXEE_NET_SERVER_H

#include <cstdint>
#include <string>

namespace sim {

class BoxeeNetServer
{
public:
    void startScanListener();

    void stopScanListener();

    void startRequestListener(uint16_t port, std::string password);

    void stopRequestListener();

private:
    // TODO: add udp scan handler

    // TODO: add http request handler
};

} // namespace sim

#endif // BOXEE_NET_SERVER_H
