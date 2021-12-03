#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFont>
#include "place_dir_pairs_abstract_model.h"
#include "ui_data.h"
#include "abstract_file_entry_model.h"
#if defined Q_OS_ANDROID
#include <QJniObject>
#include <QtCore/private/qandroidextras_p.h>
#endif

const int DEFAULT_FONT_POINTSIZE = 11;

void RegisterGlobalContextQMLVariables(QQmlEngine& engine)
{
    engine.rootContext()->setContextProperty("ASFontPointSizes",
                                             QMap<QString,QVariant> {
                                                 {"Text",DEFAULT_FONT_POINTSIZE}
                                             });
    engine.rootContext()->setContextProperty("PlaceDirPairsModel",
                                             place_dir_pairs_abstract_model_instance());
    engine.rootContext()->setContextProperty("FileModel",
                                             abstract_file_entry_model_instance());
    engine.rootContext()->setContextProperty("UIData",
                                             ui_data_instance()
                                             );
}

#if defined Q_OS_ANDROID
void AskForAndroidPermissions()
{
  const QString per = "android.permission.READ_EXTERNAL_STORAGE";
  QtAndroidPrivate::requestPermission(per);
}
#endif

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QFont font = app.font();
    font.setPointSize(DEFAULT_FONT_POINTSIZE);
    app.setFont(font);
#if defined Q_OS_WIN
    qputenv("QSG_RHI_BACKEND","opengl");
#endif
#if defined Q_OS_ANDROID
    AskForAndroidPermissions();
#endif
    place_dir_pairs_abstract_model_instance();
    ui_data_instance();
    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/Explorer/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    RegisterGlobalContextQMLVariables(engine);
    engine.load(url);
    return app.exec();
}
