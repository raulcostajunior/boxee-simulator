#ifndef NET_MESSAGE_H
#define NET_MESSAGE_H

#include <cstdint>
#include <QDateTime>
#include <QString>

namespace model {

enum class NetMessageType : uint8_t {
    SCN,     // Scan network for Boxees.
    CMD,     // Command sent from a remote to a Boxee.
    ERR_SCN, // Invalid scan request from a remote.
    ERR_CMD  // Invalund command sent from a remote.
};

enum class NetMessageDirection : uint8_t {
    FROM_REMOTE, // Message sent from a remote to a Boxee.
    TO_REMOTE // Message sent from a Boxee to a remote (reply to a previous message originated by a remote).
};

struct NetMessage
{
    // TODO: make all the data fields read-only; they must be ctor initialized;
    // TODO: provide a ctor with all fields, and one with all except dateTime (initializes with current datetime).
    // TODO: investigate the copy behavior of QDateTime and of QString (any need to define copy operators in here?)
    QDateTime dateTime;
    NetMessageType type;
    NetMessageDirection direction;
    QString boxeeRemoteName;
    QString payload;

    QString messageDescription() const;
};

} // namespace model

#endif // NET_MESSAGE_H
