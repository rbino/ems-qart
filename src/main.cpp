#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "CartController.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<CartController>("EmsQart", 1, 0, "CartController");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine(QUrl(QStringLiteral("qrc:/qml/MainView.qml")));

    return app.exec();
}
