#include "QApplication"
#include "QQmlApplicationEngine"
#include "QQuickStyle"
#include "QIcon"

#include "backend/fontdatabase.hpp"
#include "backend/clipboard.hpp"
#include "backend/utils.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/fontColor.ico"));
    
    qmlRegisterType<FontDatabase>("backend.FontDatabase", 0, 1, "FontDatabase");
    qmlRegisterType<Clipboard>("backend.Clipboard", 0, 1, "Clipboard");
    qmlRegisterType<Utils>("backend.Utils", 0, 1, "Utils");
    
    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("Universal");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &QApplication::quit);
    return app.exec();
}
