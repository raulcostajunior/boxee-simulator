#ifndef BOXEE_H
#define BOXEE_H

#include <cstdint>
#include <QObject>
#include <QString>

#include "boxee_net_server.h"
#include "dto/net_message.h"

namespace core {

class Boxee : public QObject
{
    Q_OBJECT

public:
    static Boxee &instance();

    enum class State : uint8_t {
        OFF, // Powered off.
        BOOTING,
        ON_STANDARD,   // On and in standard mode (neither playing media nor handling text input).
        ON_PLAYING,    // On and playing some kind of media.
        ON_TEXT_ENTRY, // On and handling text input.
        SHUTTING_DOWN
    };

    enum class MediaType : uint8_t {
        NONE, // No media playing (while not in playing mode).
        AUDIO,
        VIDEO,
        IMAGE
    };

    enum class RemoteKeyCode : uint16_t {
        RIGHT = 273,
        LEFT = 272,
        UP = 270,
        DOWN = 271,
        OK = 256, // OK is the center button in the group of navegational
        CANCEL = 257
        // TODO: add the key codes for the keyboard
    };

    uint8_t bootTimeSecs() const { return _bootTimeSecs; }
    void setBootTimeSecs(uint8_t);

    uint8_t shutdownTimeSecs() const { return _shutdownTimeSecs; }
    void setShutdownTimeSecs(uint8_t);

    const QString &password() const { return _netServer.password(); }
    void setPassword(const QString &);

    uint16_t httpPort() const { return _netServer.httpPort(); }
    void setHttpPort(uint16_t);

    State state() const { return _state; }

    MediaType mediaType() const { return _mediaType; }

    void powerOn();

    void powerOff();

signals:
    void stateChanged(State newState);

    void mediaTypeChanged(MediaType newMediaType);

    void onRemoteKeyPress(uint16_t keyCode);

    void onNetMessage(const dto::NetMessage &netMsg);

private:
    Boxee() = default;
    virtual ~Boxee() = default;

    Boxee(const Boxee &) = delete;
    Boxee &operator=(const Boxee &) = delete;
    Boxee(Boxee &&) = delete;
    Boxee &operator=(const Boxee &&) = delete;

    State _state = State::OFF;
    MediaType _mediaType = MediaType::NONE;

    BoxeeNetServer _netServer;

    uint8_t _bootTimeSecs = 3;
    uint8_t _shutdownTimeSecs = 2;

    // State changes should be called internally by higher level public methods like playVideo, promptForText, ... Those methods should be called primarly by screen controllers.
    // The Boxee FSM should be kept internal to this class.
    // IDEA: those public methods should be part of a base screen controller class which should be a friend of the Boxee class - this is coherent as the screen is part of the Boxee.
    void setState(State);

    // Media types changes are analogous to state changes and should be triggered by higher level public methods.
    void setMediaType(MediaType);
};

} // namespace core

#endif // BOXEE_H
