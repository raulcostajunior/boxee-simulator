#include "log_view.h"

#include <QHeaderView>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QVector>

#include "core/boxee.h"

using namespace gui;
using namespace core;
using namespace model;

LogView::LogView(QWidget *parent)
    : QTableWidget(parent)
{
    setColumnCount(4);
    setHorizontalHeaderLabels(QStringList() << tr("Time") << tr("Remote") << tr("Message Type")
                                            << tr("Request Path / Message Body"));
    horizontalHeader()->setStretchLastSection(true);

    setAlternatingRowColors(true);

    connect(&Boxee::instance(), &Boxee::onNetMessage, this, &LogView::onNetMessageHandler);
}

void LogView::saveLog(const QString &filePath)
{
    // TODO: write method body
}

void LogView::clearLog()
{
    clearContents();
    setRowCount(0);
    emit logChanged();
}

void LogView::onNetMessageHandler(const NetMessage &netMsg)
{
    int nLines = rowCount();
    insertRow(nLines);

    auto timeItem = new QTableWidgetItem();
    timeItem->setText(netMsg.dateTime.toString("hh:mm:ss"));
    setItem(nLines, 0, timeItem);

    auto remoteNameItem = new QTableWidgetItem();
    remoteNameItem->setText(netMsg.boxeeRemoteName);
    remoteNameItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    setItem(nLines, 1, remoteNameItem);

    auto msgTypeItem = new QTableWidgetItem();
    msgTypeItem->setText(netMsg.messageDescription());
    setItem(nLines, 2, msgTypeItem);

    auto msgBodyItem = new QTableWidgetItem();
    msgBodyItem->setText(netMsg.payload);
    setItem(nLines, 3, msgBodyItem);

    resizeColumnToContents(0);
    resizeColumnToContents(1);
    resizeColumnToContents(2);
    resizeRowToContents(nLines);

    scrollToBottom();

    emit(logChanged());
}
