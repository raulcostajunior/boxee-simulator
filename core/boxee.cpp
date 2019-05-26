#include "boxee.h"

#include <QString>
#include <QTimer>

using namespace core;

// TODO: evaluate if class must be thread-safe - as it currently uses the MainThread event loop for QTimer single shots,
//       it is not the case.

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
    _netServer.setPassword(password);
}

void Boxee::setHttpPort(uint16_t httpPort)
{
    _netServer.setHttpPort(httpPort);
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

    QTimer::singleShot(_bootTimeSecs * 1000, [this] {
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

    QTimer::singleShot(_shutdownTimeSecs * 1000, [this] {
        setState(State::OFF);
        _netServer.stopScanListener();
        _netServer.stopRequestListener();
    });
}
