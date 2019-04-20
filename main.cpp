#include "cstring"
#include "iostream"

#include "QApplication"
#include "QQmlApplicationEngine"
#include "QQuickStyle"
#include "QIcon"
#include "QMetaType"

#include "backend/fontdatabase.hpp"
#include "backend/clipboard.hpp"
#include "backend/utils.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
        {
            cout << argv[0] << " usage: " << endl;
            cout << "No argument: just execute this program" << endl;
            cout << "\"--debug\": print qDebug() message to console" << endl;
            cout << "\"-h\" or \"--help\": print is message and exit" << endl;
            return 0;
        }
        
        if (strcmp(argv[1], "--debug") != 0)
        {
            cerr << "Unknown argument: " << argv[1];
            return 1;
        }
    }
    
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/fontColor.ico"));
    
    qRegisterMetaType<QList<QString>>("QList<QString>&");
    qRegisterMetaType<QList<int>>("QList<int>&");
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
