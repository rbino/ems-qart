import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import EmsQart 1.0

Item {
    id: bankWidget;

    property alias bank: bankModel.bank;
    property alias title: bankLabel.text;
    property bool selectable: true;

    implicitHeight: bankLabel.height + bankView.height;

    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;

    BankModel {
        id: bankModel;
    }

    Text {
        id: bankLabel;

        anchors {
            horizontalCenter: parent.horizontalCenter;
            bottom: bankView.top;
            bottomMargin: 15;
        }

        font.pixelSize: 24;
    }

    TableView {
        id: bankView;

        selectionMode: bankWidget.selectable ? SelectionMode.SingleSelection
                                             : SelectionMode.NoSelection;

        anchors {
            left: parent.left;
            right: parent.right;
            leftMargin: 20;
            rightMargin: 20;
            verticalCenter: parent.verticalCenter;
        }

        TableViewColumn {
            title: "#";
            role: "index";
            width: 20;
        }
        TableViewColumn {
            title: "Title";
            role: "title";
        }
        TableViewColumn {
            title: "Size (bytes)";
            role: "size";
        }
        TableViewColumn {
            title: "Offset";
            role: "offset";
            width: 100;
        }

        model: bankModel;
    }
}

