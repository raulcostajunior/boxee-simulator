#include "boxee_net_server.h"

#include <cstdint>
#include <mutex>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QString>
#include <QUdpSocket>
#include <QXmlStreamReader>

using namespace core;

const uint16_t BoxeeNetServer::kBoxeeScanPort = 2562;
const QString BoxeeNetServer::kBoxeeSharedKey("b0xeeRem0tE!");

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

void BoxeeNetServer::processScanDatagrams() const
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
        const QString payload = QString(scanDatagram.constData());

        qDebug() << "Payload received through UDP:\n" << payload << "\n";

        if (isScanDatagramValid(payload)) {
            qDebug() << "Scan datagram valid - will send the response..."
                     << "\n";
            // Builds and sends the response.
            QByteArray respDatagram;
            // TODO: generate real response payload.
            respDatagram.append(QByteArray("TODO:// add real payload"));
            _scanSocket->writeDatagram(respDatagram, senderAddr, senderPort);
        }
    }
}

bool BoxeeNetServer::isScanDatagramValid(const QString &payload) const
{
    bool validPayload = true;
    QByteArray signature;
    QByteArray challenge;
    QXmlStreamReader reader(payload);
    // Parses the xml looking for the "signature" attribute in the "BDP1" element.
    // If the attribute cannot be found, the payload is considered invalid.
    while (!reader.atEnd() && validPayload) {
        if (reader.hasError())
            validPayload = false;
        else if (reader.readNext() == QXmlStreamReader::StartElement) {
            if (reader.name() == "BDP1") {
                validPayload = (reader.attributes().hasAttribute("cmd")
                                && reader.attributes().value("cmd") == "discover");
                if (!validPayload)
                    continue;
                validPayload = (reader.attributes().hasAttribute("application")
                                && reader.attributes().value("application") == "iphone_remote");
                if (!validPayload)
                    continue;
                validPayload = reader.attributes().hasAttribute("challenge");
                if (!validPayload)
                    continue;
                challenge = reader.attributes().value("challenge").toUtf8();
                validPayload = reader.attributes().hasAttribute("signature");
                if (!validPayload)
                    continue;
                signature = reader.attributes().value("signature").toUtf8();
            }
        }
    }
    if (validPayload) {
        // Checks the signature against the challenge.
        QCryptographicHash md5Hash(QCryptographicHash::Algorithm::Md5);
        md5Hash.addData(challenge);
        md5Hash.addData(BoxeeNetServer::kBoxeeSharedKey.toUtf8());
        QString generatedSignature(md5Hash.result().toHex().constData());
        validPayload = (generatedSignature.toUpper() == signature.toUpper());
    }
    return validPayload;
}
