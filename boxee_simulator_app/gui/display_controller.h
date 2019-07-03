#ifndef DISPLAY_CONTROLLER_H
#define DISPLAY_CONTROLLER_H

#include <QObject>

/**
 * @brief The DisplayController class
 * 
 * Responsible for presenting the appropriate display contents depending on the state of the Boxee and
 * the input provided. Keeps a stack of displays internally. A display can call a method in the display
 * controller to close itself and put its caller in the first plane.
 */
class DisplayController : public QObject
{
    Q_OBJECT
public:
    explicit DisplayController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DISPLAY_CONTROLLER_H
