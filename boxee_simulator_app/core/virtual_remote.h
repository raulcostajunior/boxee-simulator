#ifndef VIRTUAL_REMOTE_H
#define VIRTUAL_REMOTE_H

#include <QNetworkAccessManager>
#include <QString>

namespace core {

class VirtualRemote
{
public:
    static VirtualRemote &instance();

    void sendPlayPause();
    void sendUp();
    void sendRight();
    void sendDown();
    void sendLeft();
    void sendEnter();
    void sendMenu();

    // TODO add methods to send remote keyboard presses.

private:
    VirtualRemote() = default;
    virtual ~VirtualRemote() = default;
    VirtualRemote(const VirtualRemote &) = delete;
    VirtualRemote operator=(const VirtualRemote &) = delete;
    VirtualRemote(VirtualRemote &&) = delete;
    VirtualRemote operator=(VirtualRemote &&) = delete;

    void sendCommand(const QString &cmd);

    QNetworkAccessManager _netMngr;
};

} // namespace core

#endif // VIRTUAL_REMOTE_H
