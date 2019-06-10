#include "boxee_net_server.h"

#include "model/net_message.h"

#include <cstdint>
#include <mutex>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QString>
#include <QUdpSocket>
#include <QXmlStreamReader>

using namespace core;
using namespace model;

const uint16_t BoxeeNetServer::kBoxeeScanPort = 2562;
const QString BoxeeNetServer::kBoxeeSharedKey("b0xeeRem0tE!");
const QString BoxeeNetServer::kBoxeeResponseChallenge("b0xeeResp0nsE");

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
    while (_scanSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = _scanSocket->receiveDatagram();
        QHostAddress senderAddr = datagram.senderAddress();
        quint32 senderAddrV4 = senderAddr.toIPv4Address();
        quint16 senderPort = static_cast<quint16>(datagram.senderPort());
        const QString payload = QString(datagram.data().constData());

        if (isScanDatagramValid(payload)) {
            // Emits signal for valid scan request received.
            NetMessage scanRequest;
            scanRequest.dateTime = QDateTime::currentDateTime();
            scanRequest.type = NetMessageType::SCN;
            scanRequest.direction = NetMessageDirection::FROM_REMOTE;
            scanRequest.boxeeRemoteName = QHostAddress(senderAddrV4).toString();
            scanRequest.payload = payload;
            emit(onNetMessage(scanRequest));

            // Builds and sends the response.
            QCryptographicHash md5Hash(QCryptographicHash::Algorithm::Md5);
            md5Hash.addData(BoxeeNetServer::kBoxeeResponseChallenge.toUtf8());
            md5Hash.addData(BoxeeNetServer::kBoxeeSharedKey.toUtf8());
            QString generatedSignature(md5Hash.result().toHex().constData());

            QString respPayload = QString(
                                      "<?xml version=\"1.0\"?>"
                                      "<BDP1 cmd=\"found\" application=\"boxee\" version=\"1.0\" "
                                      "name=\"%1\" response=\"%2\" "
                                      "httpPort=\"%3\" httpAuthRequired=\"%4\" "
                                      "signature=\"%5\"/>")
                                      .arg(localActiveAddress().toString())
                                      .arg(BoxeeNetServer::kBoxeeResponseChallenge)
                                      .arg(_httpPort)
                                      .arg(_password.isEmpty() ? "false" : "true")
                                      .arg(generatedSignature.toUpper());
            QByteArray respDatagram;
            respDatagram.append(QByteArray(respPayload.toUtf8()));
            _scanSocket->writeDatagram(respDatagram, senderAddr, senderPort);

            // Emits signal for scan reply sent.
            NetMessage scanReply;
            scanReply.dateTime = QDateTime::currentDateTime();
            scanReply.type = NetMessageType::SCN;
            scanReply.direction = NetMessageDirection::TO_REMOTE;
            // Need to rebuild IP from quint32 for the host address to be an IPv4 address; otherwise,
            // since Qt5 it will be an IPv6 address with the string prefix "::ffff:".
            scanReply.boxeeRemoteName
                = QString("%1:%2").arg(QHostAddress(senderAddrV4).toString()).arg(senderPort);
            scanReply.payload = payload;
            emit(onNetMessage(scanReply));

        } else {
            // Emits signal for invalid scan request received.
            NetMessage invalidScanRequest;
            invalidScanRequest.dateTime = QDateTime::currentDateTime();
            invalidScanRequest.type = NetMessageType::ERR_SCN;
            invalidScanRequest.direction = NetMessageDirection::FROM_REMOTE;
            invalidScanRequest.boxeeRemoteName = QHostAddress(senderAddrV4).toString();
            invalidScanRequest.payload = payload;
            emit(onNetMessage(invalidScanRequest));
        }
    }
}

bool BoxeeNetServer::isScanDatagramValid(const QString &payload) const
{
    bool validPayload = true;
    QByteArray signature;
    QByteArray challenge;
    QXmlStreamReader reader(payload);
    // Parses the xml looking for the "signature" and "challenge" attributes in the "BDP1" element.
    // Validates other attributes on its way. If any attribute can't be found with the expected
    // values, the payload is considered invalid.
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

QHostAddress BoxeeNetServer::localActiveAddress() const
{
    QHostAddress localAddr(QHostAddress::LocalHost);
    QNetworkInterface activeInterface;
    bool interfaceFound = false;
    for (QNetworkInterface interface : QNetworkInterface::allInterfaces()) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp)
            && (!interface.flags().testFlag(QNetworkInterface::IsLoopBack))) {
            activeInterface = interface;
            interfaceFound = true;
            break;
        }
    }
    if (interfaceFound) {
        for (auto &addr : activeInterface.allAddresses()) {
            if (addr.protocol() == QAbstractSocket::IPv4Protocol && !addr.isLoopback()) {
                localAddr = addr;
                break;
            }
        }
    }
    return localAddr;
}
