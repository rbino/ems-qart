import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Tab {
    id: tabPage;

    RowLayout {
        id: tabRowLayout;

        anchors {
            left: parent.left;
            right: parent.right;
        }

        Item {
            id: leftContent;

            implicitHeight: tabPage.height;
            implicitWidth: tabPage.width / 2;

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;
        }

        EmsCartItem {
            id: emsCartItem;

            implicitHeight: tabPage.height;
            implicitWidth: tabPage.width / 2;

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;
        }
    }

}
