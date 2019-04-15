#include "QUrl"
#include "QGuiApplication"
#include "QDebug"

#include "clipboard.hpp"

#if !defined(Q_OS_WIN32) && !defined(Q_OS_DARWIN)
#define NOT_WIN_NOR_MAC
#include "QWidget"
#include "QFileDialog"
#endif

Clipboard::Clipboard(QObject *parent) :
    QObject(parent)
{
}

void Clipboard::copyString(QString input)
{
    QGuiApplication::clipboard()->clear();
    QGuiApplication::clipboard()->setText(input);
}

void Clipboard::copyFile(QString input)
{
#ifdef NOT_WIN_NOR_MAC
    //there is no standard way to copy and paste files between applications on X11
    QWidget *tmp = new QWidget(nullptr);
    QStringList list = input.split("/");
    QString tmp_str = list.at(list.count() - 1);
    list = tmp_str.split(".");
    QString ext = list.at(list.count() - 1);
    QString res(QFileDialog::getSaveFileName(tmp, "Select where to save font file", tmp_str, "*." + ext));
    delete tmp;
    if (res == "")
    {
        return;
    }
    QFile::copy(input, res);
#else
    QGuiApplication::clipboard()->clear();
    QMimeData *mimeData = new QMimeData();
    mimeData->setUrls({QUrl::fromLocalFile(input)});
    QGuiApplication::clipboard()->setMimeData(mimeData);
    
    //Don's free mimeData here, or crash.
#endif
}
