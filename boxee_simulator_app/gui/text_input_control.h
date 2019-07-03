#ifndef TEXT_INPUT_CONTROL_H
#define TEXT_INPUT_CONTROL_H

#include <QObject>
#include <QWidget>

/**
 * @brief The TextInputControl class
 * 
 * Simulates the keyboard on the Boxee remote. Sends keystrokes through the 
 * methods inside the VirtualRemote.
 */
class TextInputControl : public QWidget
{
    Q_OBJECT
public:
    explicit TextInputControl(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // TEXT_INPUT_CONTROL_H
