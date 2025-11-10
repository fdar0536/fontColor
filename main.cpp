#include "cstring"
#include "iostream"

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
    app.setWindowIcon(QIcon(":/fontColor.ico"));

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
