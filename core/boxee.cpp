#include "boxee.h"

#include <QString>
#include <QTimer>

using namespace core;

// TODO: make class thread-safe.

Boxee &Boxee::instance()
{
    static Boxee m_instance;
    return m_instance;
}

void Boxee::setBootTimeSecs(uint8_t bootTime)
{
    _bootTimeSecs = bootTime;
}

void Boxee::setShutdownTimeSecs(uint8_t shutdownTime)
{
    _shutdownTimeSecs = shutdownTime;
}

void Boxee::setPassword(const QString &password)
{
    if (password != _password) {
        if (_netServer.isListeningToRequests()) {
            _netServer.stopScanListener();
            _password = password;
            _netServer.startRequestListener(_httpPort, _password);
        }
    }
}

void Boxee::setHttpPort(uint16_t httpPort)
{
    if (httpPort != _httpPort) {
        if (_netServer.isListeningToRequests()) {
            _netServer.stopScanListener();
            _httpPort = httpPort;
            _netServer.startRequestListener(_httpPort, _password);
        }
    }
}

void Boxee::setState(State state)
{
    if (state != _state) {
        _state = state;
        emit(stateChanged(_state));
    }
}

void Boxee::setMediaType(MediaType mediaType)
{
    if (mediaType != _mediaType) {
        _mediaType = mediaType;
        emit(mediaTypeChanged(_mediaType));
    }
}

void Boxee::powerOn()
{
    if (_state != State::OFF) {
        // Boxee is not turned off; ignore the powerOn command.
        return;
    }

    setState(State::BOOTING);

    QTimer::singleShot(_bootTimeSecs * 1000, [this] { setState(State::ON_STANDARD); });
}

void Boxee::powerOff()
{
    if (_state == State::OFF || _state == State::BOOTING || _state == State::SHUTTING_DOWN) {
        return;
    }

    setState(State::SHUTTING_DOWN);

    QTimer::singleShot(_shutdownTimeSecs * 1000, [this] { setState(State::OFF); });
}
