#ifndef LOG_VIEW_H
#define LOG_VIEW_H

#include <QString>
#include <QTableWidget>
#include <QVector>

#include "model/net_message.h"

namespace gui {

class LogView : public QTableWidget
{
    Q_OBJECT
public:
    explicit LogView(QWidget *parent = nullptr);
    void saveLog(const QString &filePath);
    void clearLog();
    bool isEmpty() const { return rowCount() == 0; }

signals:
    void logChanged();

public slots:

private:
    void onNetMessageHandler(const model::NetMessage &netMsg);
};

} // namespace gui

#endif // LOG_VIEW_H
