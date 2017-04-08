import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import EmsQart 1.0

Tab {
    id: readTabPage;

    property int selectedCartIndex: -1;
    property int selectedCartBank: -1;

    property int memory: EmsCart.SRAM;
    property bool isSRAM: memory == EmsCart.SRAM;

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
            id: readContent;

            implicitHeight: readTabPage.height;
            implicitWidth: readTabPage.width / 2;

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;

            ColumnLayout {
                id: readLayout;

                anchors {
                    fill: parent;
                }

                BankWidget {
                    id: bankOneWidget;
                    title: "Bank One";

                    visible: !isSRAM;

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

                    visible: !isSRAM;

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
                    id: chooseFileItem;

                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;
                    Layout.fillWidth: true;
                    Layout.leftMargin: 40;
                    Layout.rightMargin: 40;
                    Layout.preferredHeight: buttonsRow.height + fileNameTextEdit.height;

                    TextField {
                        id: fileNameTextEdit;

                        readOnly: true;

                        text: cartController.urlToLocalPath(fileDialog.fileUrl);

                        anchors {
                            left: parent.left;
                            right: parent.right;
                        }

                    }

                    FileDialog {
                        id: fileDialog;

                        title: "Choose where to save the ROM";
                        nameFilters: isSRAM ? ["Gameboy SAVs (*.sav)"]
                                            : ["Gameboy ROMs (*.gb *.gbc)"];
                        selectExisting: false;
                    }

                    RowLayout {
                        id: buttonsRow;

                        height: 70;

                        anchors {
                            left: parent.left;
                            right: parent.right;
                            top: fileNameTextEdit.bottom;
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
                                     && (isSRAM || selectedCartIndex >= 0)
                                     && cartController.ready
                                     && !cartController.busy;

                            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;

                            text: "Start";

                            onClicked: {
                                if (isSRAM) {
                                    cartController.readSRAM(fileDialog.fileUrl);
                                } else {
                                    cartController.readROM(fileDialog.fileUrl,
                                                           selectedCartBank,
                                                           selectedCartIndex);
                                }
                            }
                        }
                    }
                }
            }
        }

        EmsCartItem {
            id: emsCartItem;

            implicitHeight: readTabPage.height;
            implicitWidth: readTabPage.width / 2;

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;
        }
    }
}
