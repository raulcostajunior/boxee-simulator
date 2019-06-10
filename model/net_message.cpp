#include "net_message.h"

#include <QObject>
#include <QString>

using namespace model;

QString NetMessage::messageDescription() const
{
    QString desc;

    switch (type) {
    case NetMessageType::SCN:
        desc.append(QObject::tr("Scan "));
        break;
    case NetMessageType::ERR_SCN:
        desc.append(QObject::tr("Invalid Scan "));
        break;
    case NetMessageType::CMD:
        desc.append(QObject::tr("Command "));
        break;
    case NetMessageType::ERR_CMD:
        desc.append(QObject::tr("Invalid Command "));
        break;
    }

    switch (direction) {
    case NetMessageDirection::TO_REMOTE:
        desc.append(QObject::tr("Reply"));
        break;
    case NetMessageDirection::FROM_REMOTE:
        desc.append(QObject::tr("Request"));
        break;
    }

    return desc;
}
