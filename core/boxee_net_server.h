#ifndef BOXEE_NET_SERVER_H
#define BOXEE_NET_SERVER_H

#include <cstdint>
#include <mutex>
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

    void processScanDatagrams();

    // TODO: add http request handler

    static const uint16_t kBoxeeScanPort;
    static const QString kBoxeeScanKey;
};

} // namespace core

#endif // BOXEE_NET_SERVER_H
