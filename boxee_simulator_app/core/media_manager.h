#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <QObject>

/**
 * @brief The MediaManager class
 * 
 * Knows how to play a video on a given surface, a sound file or a 
 * photo gallery. Allows a client to query and retrieve available 
 * videos, sounds and photos galleries. 
 */
class MediaManager : public QObject
{
    Q_OBJECT
public:
    explicit MediaManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MEDIA_MANAGER_H
