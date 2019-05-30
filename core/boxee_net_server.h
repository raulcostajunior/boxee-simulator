#ifndef BOXEE_NET_SERVER_H
#define BOXEE_NET_SERVER_H

#include <cstdint>
#include <mutex>
#include <QHostAddress>
#include <QObject>
#include <QString>

class QUdpSocket;

namespace core {

class BoxeeNetServer : public QObject
{
    Q_OBJECT

public:
    BoxeeNetServer();

    void startScanListener();

    void stopScanListener();

    void startRequestListener();

    void stopRequestListener();

    const QString &password() const;
    void setPassword(const QString &pwd);

    uint16_t httpPort() const;
    void setHttpPort(uint16_t port);

private:
    mutable std::mutex _listenParamMutex;
    QString _password; // QStrings are default initialized to the empty string.
    uint16_t _httpPort{8080};

    bool _scanListening = false;
    bool _requestListening = false;

    QUdpSocket *_scanSocket;

    /**
     * @brief Processes a datagram that is supposedly a scan request sent by an instance of a Boxee Remote
     * application. If the request is successfully validated, sends the response back to the requester.
     * 
     * @note The "protocol" specification for discovery of Boxee units by a remote party can be found at
     * https://web.archive.org/web/20130603035923/http://developer.boxee.tv/Remote_Control_Interface.
     */
    void processScanDatagrams() const;

    bool isScanDatagramValid(const QString &payload) const;

    QHostAddress localActiveAddress() const;

    // TODO: add http request handler

    static const uint16_t kBoxeeScanPort;
    static const QString kBoxeeSharedKey;
    static const QString kBoxeeResponseChallenge;
};

} // namespace core

#endif // BOXEE_NET_SERVER_H
