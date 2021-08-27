#include "boxee.h"

#include <QString>
#include <QTimer>

#include "boxee_net_server.h"
#include "model/net_message.h"

using namespace core;
using namespace model;

// TODO: evaluate if class must be thread-safe - as it currently uses the MainThread event loop for QTimer single shots,
//       it is not the case.

// Private default constructor.
Boxee::Boxee()
{
    // Connects the NetServer onNetMessage for Boxee to relay to interested parties outside 'core'.
    connect(&_netServer, &BoxeeNetServer::onNetMessage, [this](const model::NetMessage &netMsg) {
        emit onNetMessage(netMsg);
    });
}

Boxee &Boxee::instance()
{
    static Boxee _instance;
    return _instance;
}

void Boxee::setPassword(const QString &password)
{
    _netServer.setPassword(password);
}

void Boxee::setHttpPort(const uint16_t httpPort)
{
    _netServer.setHttpPort(httpPort);
}

void Boxee::setState(State state)
{
    if (state != _state) {
        _state = state;
        emit stateChanged(_state);
    }
}

QString Boxee::stateAsString() const
{
    QString stateStr;

    switch (_state) {
    case State::OFF:
        stateStr = tr("Off");
        break;
    case State::BOOTING:
        stateStr = tr("Booting...");
        break;
    case State::SHUTTING_DOWN:
        stateStr = tr("Shutting down...");
        break;
    case State::ON_STANDARD:
        stateStr = tr("Active");
        break;
    case State::ON_TEXT_ENTRY:
        stateStr = tr("Active (text input)");
        break;
    case State::ON_MEDIA_PLAYING:
        stateStr = tr("Active (playing media)");
        break;
    case State::ON_MEDIA_NOT_PLAYING:
        stateStr = tr("Active (media not playing)");
        break;
    }

    return stateStr;
}

QString Boxee::boxeeAddress() const
{
    return _netServer.localActiveAddress().toString();
}

void Boxee::setMediaType(MediaType mediaType)
{
    if (mediaType != _mediaType) {
        _mediaType = mediaType;
        emit mediaTypeChanged(_mediaType);
    }
}

void Boxee::powerOn()
{
    if (_state != State::OFF) {
        // Boxee is not turned off; ignore the powerOn command.
        return;
    }

    setState(State::BOOTING);

    QTimer::singleShot(_bootTimeSecs * 1000, this, [this] {
        setState(State::ON_STANDARD);
        _netServer.startScanListener();
        _netServer.startRequestListener();
    });
}

void Boxee::powerOff()
{
    if (_state == State::OFF || _state == State::BOOTING || _state == State::SHUTTING_DOWN) {
        return;
    }

    setState(State::SHUTTING_DOWN);

    QTimer::singleShot(_shutdownTimeSecs * 1000, this, [this] {
        setState(State::OFF);
        _netServer.stopScanListener();
        _netServer.stopRequestListener();
    });
}
