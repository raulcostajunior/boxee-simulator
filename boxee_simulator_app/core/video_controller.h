#ifndef VIDEO_CONTROLLER_H
#define VIDEO_CONTROLLER_H

#include <QObject>

/**
 * @brief The VideoController class
 * 
 * Controls a video playing session acting as a middle element between the netmessages 
 * received and applying their semantics to an attached video player (probably Qt specific)
 * received at construction time. A friend class of the Boxee class, being able to set
 * its state and the current media playing parameters (current media position and current
 * media duration should be exposed as read-only parameters by the Boxee and will be 
 * maintaned by the VideoController through private methods.
 * 
 * Analog classes will exist for musics and photos.
 */
class VideoController : public QObject
{
    Q_OBJECT
public:
    explicit VideoController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // VIDEO_CONTROLLER_H
