#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QIcon>

#include "BankModel.h"
#include "CartController.h"
#include "EmsCart.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<BankModel>("EmsQart", 1, 0, "BankModel");
    qmlRegisterType<CartController>("EmsQart", 1, 0, "CartController");

    qmlRegisterUncreatableType<EmsCart>("EmsQart", 1, 0, "EmsCart", QStringLiteral("EmsCart is not creatable in QML. use CartController"));

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QStringLiteral("qrc:/images/ems-qart.icon")));

    QQmlApplicationEngine engine(QUrl(QStringLiteral("qrc:/qml/MainView.qml")));

    return app.exec();
}
