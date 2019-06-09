#include "log_view.h"

#include <QString>
#include <QTableView>
#include <QVector>

#include "core/boxee.h"

using namespace gui;
using namespace core;
using namespace model;

LogView::LogView(QWidget *parent)
    : QTableView(parent)
{
    // TODO: set table header and number of columns

    connect(&Boxee::instance(), &Boxee::onNetMessage, this, &LogView::onNetMessageHandler);
}

void LogView::saveLog(const QString &filePath)
{
    // TODO: write method body
}

void LogView::clearLog()
{
    logEntries.clear();
    emit logChanged();
}

void LogView::onNetMessageHandler(const NetMessage &netMsg)
{
    // TODO: add net message to log view.
}
