import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import EmsQart 1.0

Tab {
    id: writeROMTabPage;

    RowLayout {
        id: tabRowLayout;

        anchors {
            left: parent.left;
            right: parent.right;
        }

        CartController {
            id: cartController;
        }

        Item {
            id: writeROMContent;

            implicitHeight: writeROMTabPage.height;
            implicitWidth: writeROMTabPage.width / 2;

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;

            ColumnLayout {
                id: writeROMLayout;

                anchors {
                    fill: parent;
                }

                GroupBox {
                    title: "Choose bank";

                    anchors {
                        horizontalCenter: parent.horizontalCenter;
                    }

                    RowLayout {
                        ExclusiveGroup {
                            id: bankGroup;
                        }

                        RadioButton {
                            id: bankOneButton;

                            text: "Bank 1";
                            checked: true;
                            exclusiveGroup: bankGroup;
                        }

                        RadioButton {
                            id: bankTwoButton;

                            text: "Bank 2";
                            exclusiveGroup: bankGroup;
                        }
                    }
                }

                BankWidget {
                    id: bankOneWidget;
                    title: "Bank One";

                    visible: bankOneButton.checked;

                    bank: EmsCart.BankOne;

                    anchors {
                        left: parent.left;
                        right: parent.right;
                    }

                    onSelected: {
                        selectedCartIndex = index;
                        selectedCartBank = EmsCart.BankOne;
                        bankTwoWidget.deselect();
                    }
                }

                BankWidget {
                    id: bankTwoWidget;
                    title: "Bank Two";

                    visible: bankTwoButton.checked;

                    bank: EmsCart.BankTwo;

                    anchors {
                        left: parent.left;
                        right: parent.right;
                    }

                    onSelected: {
                        selectedCartIndex = index;
                        selectedCartBank = EmsCart.BankTwo;
                        bankOneWidget.deselect();
                    }
                }

                Item {
                    id: freeSpaceItem;
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;
                    Layout.fillWidth: true;
                    Layout.leftMargin: 20;
                    Layout.rightMargin: 20;
                    Layout.preferredHeight: spaceIndicator.height;

                    Label {
                        id: spaceLabel;

                        text: "Remaining space: "
                              + allocationController.freeSpace + " bytes"

                        anchors {
                            bottom: spaceIndicator.top;
                            bottomMargin: 4;
                            horizontalCenter: parent.horizontalCenter;
                        }
                    }

                    ProgressBar {
                        id: spaceIndicator;

                        // Bank size
                        value: 1 - (allocationController.freeSpace / 4194304);

                        anchors {
                            horizontalCenter: parent.horizontalCenter;
                        }
                    }

                }

                Item {
                    id: chooseFileItem;

                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;
                    Layout.fillWidth: true;
                    Layout.leftMargin: 20;
                    Layout.rightMargin: 20;
                    Layout.preferredHeight: buttonsRow.height + fileNamesView.height;

                    AllocationController {
                        id: allocationController;

                        bank: bankOneButton.checked ? EmsCart.BankOne : EmsCart.BankTwo;
                    }

                    TableView {
                        id: fileNamesView;

                        height: 200;

                        anchors {
                            left: parent.left;
                            right: parent.right;
                        }

                        model: allocationController.romsModel;

                        TableViewColumn {
                            title: "#";
                            role: "index";
                            width: 20;
                        }
                        TableViewColumn {
                            title: "File name";
                            role: "sourceFile";
                            width: 200;
                        }
                        TableViewColumn {
                            title: "Size (in bytes)";
                            role: "size";
                        }
                        TableViewColumn {
                            title: "Offset";
                            role: "offset";
                            width: 100;
                        }
                     }

                    FileDialog {
                        id: fileDialog;

                        title: "Choose the ROM(s)";
                        nameFilters: ["Gameboy ROMs (*.gb *.gbc)"];
                        selectMultiple: true;

                        onAccepted: {
                            for (var i=0; i < fileUrls.length; i++) {
                                allocationController.allocate(fileUrls[i]);
                            }
                        }
                    }

                    RowLayout {
                        id: buttonsRow;

                        height: 70;

                        anchors {
                            left: parent.left;
                            right: parent.right;
                            top: fileNamesView.bottom;
                            topMargin: 10;
                        }

                        Button {
                            id: selectButton;

                            enabled: !cartController.busy;

                            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;

                            text: "Select file";

                            onClicked: {
                                fileDialog.open();
                            }
                        }

                        Button {
                            id: readButton;

                            enabled: fileDialog.fileUrl != ""
                                     && cartController.ready
                                     && !cartController.busy;

                            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;

                            text: "Start";

                            onClicked: {
                            }
                        }
                    }
                }
            }
        }

        EmsCartItem {
            id: emsCartItem;

            implicitHeight: writeROMTabPage.height;
            implicitWidth: writeROMTabPage.width / 2;

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;
        }
    }
}
