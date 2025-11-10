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

#include "QApplication"
#include "QQmlApplicationEngine"
#include "QQuickStyle"
#include "QIcon"
#include "QMetaType"
#include "QQuickWindow"

#include "backend/fontdatabase.hpp"
#include "backend/clipboard.hpp"
#include "backend/utils.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("://fontColor.ico"));

    qRegisterMetaType<QList<QString>>("QList<QString>&");
    qRegisterMetaType<QList<int>>("QList<int>&");
    qmlRegisterType<FontDatabase>("backend.FontDatabase", 0, 1, "FontDatabase");
    qmlRegisterType<Clipboard>("backend.Clipboard", 0, 1, "Clipboard");
    qmlRegisterType<Utils>("backend.Utils", 0, 1, "Utils");

    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("Universal");
    engine.load(QUrl("qrc:/FC/main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    QQuickWindow *window = qobject_cast<QQuickWindow*>
        (engine.rootObjects().constFirst());
    if (window)
    {
        window->setIcon(app.windowIcon());
    }

    QObject::connect(&engine, &QQmlApplicationEngine::quit, &QApplication::quit);
    return app.exec();
}
