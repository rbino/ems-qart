import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import EmsQart 1.0

Tab {
    id: statusTabPage;

    CartController {
        id: cartController;
    }

    RowLayout {
        id: tabRowLayout;

        anchors {
            left: parent.left;
            right: parent.right;
        }

        Item {
            id: statusContent;

            implicitHeight: statusTabPage.height;
            implicitWidth: statusTabPage.width / 2;

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;

            ColumnLayout {
                id: statusLayout;

                anchors {
                    fill: parent;
                }

                BankWidget {
                    id: bankOneWidget;
                    title: "Bank One";

                    bank: EmsCart.BankOne;

                    anchors {
                        left: parent.left;
                        right: parent.right;
                    }
                }

                BankWidget {
                    id: bankTwoWidget;
                    title: "Bank Two";

                    bank: EmsCart.BankTwo;

                    anchors {
                        left: parent.left;
                        right: parent.right;
                    }
                }
            }
        }

        EmsCartItem {
            id: emsCartItem;

            implicitHeight: statusTabPage.height;
            implicitWidth: statusTabPage.width / 2;

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;
        }
    }

}
