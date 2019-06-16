#ifndef BOXEE_NET_SERVER_H
#define BOXEE_NET_SERVER_H

#include <cstdint>
#include <mutex>
#include <QHostAddress>
#include <QObject>
#include <QString>

#include "model/net_message.h"
#include "qhttpserver.hpp"

class QUdpSocket;

namespace core {

class BoxeeNetServer : public QObject
{
    Q_OBJECT

public:
    BoxeeNetServer();

    ~BoxeeNetServer();

    void startScanListener();

    void stopScanListener();

    void startRequestListener();

    void stopRequestListener();

    const QString &password() const;
    void setPassword(const QString &pwd);

    uint16_t httpPort() const;
    void setHttpPort(uint16_t port);

    QHostAddress localActiveAddress() const;

signals:
    void onNetMessage(const model::NetMessage &);

private:
    QString _password; // QStrings are default initialized to the empty string.
    uint16_t _httpPort{8080};

    bool _scanListening = false;
    bool _requestListening = false;

    QUdpSocket *_scanSocket;
    qhttp::server::QHttpServer *_httpServer;

    /**
     * @brief Processes a datagram that is supposedly a scan request sent by an instance of a Boxee Remote
     * application. If the request is successfully validated, sends the response back to the requester.
     * 
     * @note The "protocol" specification for discovery of Boxee units by a remote party can be found at
     * https://web.archive.org/web/20130603035923/http://developer.boxee.tv/Remote_Control_Interface.
     */
    void processScanDatagrams();

    bool isScanDatagramValid(const QString &payload) const;

    // TODO: add request handler delegate - it can be a class on its own or simply a method of the Boxee;
    //       If it comes to a point where the Boxee class gets too big (or with too many responsabilities)
    //       the delegate should be a class on its own, instantiated by the boxee and passed to the Boxee
    //       Net Server at construction time.

    static const uint16_t kBoxeeScanPort;
    static const QString kBoxeeSharedKey;
    static const QString kBoxeeResponseChallenge;
};

} // namespace core

#endif // BOXEE_NET_SERVER_H
