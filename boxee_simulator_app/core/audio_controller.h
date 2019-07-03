#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H

#include <QObject>

/**
 * @brief The AudioController class
 * 
 * Analogous of the video controller, but for audio.
 */
class AudioController : public QObject
{
    Q_OBJECT
public:
    explicit AudioController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // AUDIO_CONTROLLER_H
