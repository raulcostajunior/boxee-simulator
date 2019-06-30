#include "virtual_remote.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QString>
#include <QUrl>

#include "boxee.h"

using namespace core;

VirtualRemote &VirtualRemote::instance()
{
    static VirtualRemote _instance;
    return _instance;
}

void VirtualRemote::sendPlayPause()
{
    sendCommand(QString("SendKey(256)"));
}

void VirtualRemote::sendUp()
{
    sendCommand(QString("SendKey(270)"));
}

void VirtualRemote::sendRight()
{
    sendCommand(QString("SendKey(273)"));
}

void VirtualRemote::sendDown()
{
    sendCommand(QString("SendKey(271)"));
}

void VirtualRemote::sendLeft()
{
    sendCommand(QString("SendKey(272)"));
}

void VirtualRemote::sendEnter()
{
    sendCommand(QString("SendKey(256)"));
}

void VirtualRemote::sendMenu()
{
    sendCommand(QString("SendKey(257)"));
}

void VirtualRemote::sendCommand(const QString &cmd)
{
    QUrl urlReq(QString("http://%1:%2/xbmcCmds/xbmcHttp?command=%3")
                    .arg(Boxee::instance().boxeeAddress())
                    .arg(Boxee::instance().httpPort())
                    .arg(cmd));
    // TODO: add support for basic credentials when the Boxee is set to use password.

    // Shoot-and-forget request - no handling of reply.
    _netMngr.get(QNetworkRequest(urlReq));
}
