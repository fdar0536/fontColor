/*
 * Font Color
 * Copyright (c) 2019-present fdar0536
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

    //Don't free mimeData here, or crash.
#endif
}
