#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QIcon>

#include "CartController.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<CartController>("EmsQart", 1, 0, "CartController");

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QStringLiteral("qrc:/images/ems-qart.icon")));

    QQmlApplicationEngine engine(QUrl(QStringLiteral("qrc:/qml/MainView.qml")));

    return app.exec();
}
