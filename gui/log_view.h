#ifndef LOG_VIEW_H
#define LOG_VIEW_H

#include <QString>
#include <QTableView>
#include <QVector>

#include "model/net_message.h"

namespace gui {

class LogView : public QTableView
{
    Q_OBJECT
public:
    explicit LogView(QWidget *parent = nullptr);
    void saveLog(const QString &filePath);
    void clearLog();

signals:
    void logChanged();

public slots:

private:
    QVector<model::NetMessage> logEntries;

    void onNetMessageHandler(const model::NetMessage &netMsg);
};

} // namespace gui

#endif // LOG_VIEW_H
