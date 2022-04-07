#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "ccdimageprovider.h"
#include "datahandler.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    qmlRegisterType<AndorCCD>("AndorCCD.module", 1, 0, "AndorCCD");

    QQmlApplicationEngine engine;
    CCDImageProvider *ccdImageProvider = new CCDImageProvider;
    DataHandler *dataHandler = new DataHandler;
    dataHandler->setImageProvider(ccdImageProvider);
    engine.addImageProvider(QLatin1String("ccd"), ccdImageProvider);
    engine.rootContext()->setContextProperty("dataHandler", dataHandler);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
