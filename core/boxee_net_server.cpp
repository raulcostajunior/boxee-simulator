#include "boxee_net_server.h"

#include <cstdint>
#include <mutex>
#include <QByteArray>
#include <QDebug>
#include <QString>
#include <QUdpSocket>

using namespace core;

const uint16_t BoxeeNetServer::kBoxeeScanPort = 2562;
const QString BoxeeNetServer::kBoxeeScanKey("b0xeeRem0tE!");

BoxeeNetServer::BoxeeNetServer()
    : _scanSocket(new QUdpSocket(this))
{}

const QString &BoxeeNetServer::password() const
{
    std::lock_guard<std::mutex> lock(_listenParamMutex);
    return _password;
}

uint16_t BoxeeNetServer::httpPort() const
{
    std::lock_guard<std::mutex> lock(_listenParamMutex);
    return _httpPort;
}

void BoxeeNetServer::setHttpPort(uint16_t port)
{
    if (port != _httpPort) {
        std::lock_guard<std::mutex> lock(_listenParamMutex);

        bool restartRequestListening = _requestListening;
        bool restartScanListening = _scanListening;
        if (restartRequestListening) {
            stopRequestListener();
        }
        if (restartScanListening) {
            stopScanListener();
        }
        _httpPort = port;
        if (restartScanListening) {
            startScanListener();
        }
        if (restartRequestListening) {
            startRequestListener();
        }
    }
}

void BoxeeNetServer::setPassword(const QString &pwd)
{
    if (pwd != _password) {
        std::lock_guard<std::mutex> lock(_listenParamMutex);

        bool restartRequestListening = _requestListening;
        bool restartScanListening = _scanListening;
        if (restartRequestListening) {
            stopRequestListener();
        }
        if (restartScanListening) {
            stopScanListener();
        }
        _password = pwd;
        if (restartScanListening) {
            startScanListener();
        }
        if (restartRequestListening) {
            startRequestListener();
        }
    }
}

void BoxeeNetServer::startScanListener()
{
    _scanSocket->bind(BoxeeNetServer::kBoxeeScanPort, QUdpSocket::ShareAddress);
    connect(_scanSocket, &QUdpSocket::readyRead, this, &BoxeeNetServer::processScanDatagrams);
    _scanListening = true;
}

void BoxeeNetServer::stopScanListener()
{
    disconnect(_scanSocket, &QUdpSocket::readyRead, this, &BoxeeNetServer::processScanDatagrams);
    _scanSocket->close();
    _scanListening = false;
}

void BoxeeNetServer::startRequestListener()
{
    // TODO: add method body

    _requestListening = true;
}

void BoxeeNetServer::stopRequestListener()
{
    // TODO: add method body

    _requestListening = false;
}

void BoxeeNetServer::processScanDatagrams()
{
    QByteArray scanDatagram;
    while (_scanSocket->hasPendingDatagrams()) {
        QHostAddress senderAddr;
        quint16 senderPort;
        scanDatagram.resize(static_cast<int>(_scanSocket->pendingDatagramSize()));
        _scanSocket->readDatagram(scanDatagram.data(),
                                  scanDatagram.size(),
                                  &senderAddr,
                                  &senderPort);
        const char *pPayload = scanDatagram.constData();
        // TODO: add payload validation before sending out response.
        qDebug() << "Payload received through UDP:\n" << pPayload << "\n";
        QByteArray respDatagram;
        // TODO: generate real response payload.
        respDatagram.append(QByteArray("TODO:// add real payload"));
        _scanSocket->writeDatagram(respDatagram, senderAddr, senderPort);
    }
}
