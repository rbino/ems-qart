import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import EmsQart 1.0

ApplicationWindow {
    id: mainView;

    title: "ems-qart";

    visible: true;

    width: 600;
    height: 200;

    CartController {
        id: cartController;
    }

    FileDialog {
        id: fileDialog;

        property string selectedNameFilterExtension: selectedNameFilter.split("*")[1].replace(")","");

        title: fileDialogTitle(readButton.checked, romButton.checked);

        nameFilters: romButton.checked ? ["GB Roms (*.gb)", "GBC Roms (*.gbc)"] : ["GB/GBC Save files (*.sav)"];
        selectExisting: writeButton.checked;

        onAccepted: {
            pathTextField.text = decodeURIComponent(fileDialog.fileUrl.toString().replace(/^(file:\/{2})/,""));
        }
    }

    function fileDialogTitle(read, rom) {
        if (read) {
            if (rom) {
                return "choose where the ROM will be saved";
            } else {
                return "choose where the SAV will be saved";
            }
        } else {
            if (rom) {
                return "choose the ROM to be written to the cart";
            } else {
                return "choose the SAV to be written to the cart";
            }
        }
    }

    Column {
        id: mainColumn;

        topPadding: 20;
        bottomPadding: 20;

        spacing: (parent.height - (topPadding + bottomPadding) - (radioButtonsRow.height + cartStatusRow.height + filePathRow.height + theButton.height)) / 3;

        anchors {
            fill: parent;
        }

        RowLayout {
            id: radioButtonsRow;

            anchors {
                horizontalCenter: parent.horizontalCenter;
            }

            GroupBox {
                id: readWriteGroupbox;

                title: "I/O direction";

                RowLayout {
                    id: readWriteRow;

                    spacing: 20;

                    ExclusiveGroup {
                        id: readWriteExclusiveGroup;
                    }
                    RadioButton {
                        id: readButton;

                        text: "Read from cart";
                        checked: true;
                        exclusiveGroup: readWriteExclusiveGroup;
                    }
                    RadioButton {
                        id: writeButton;

                        text: "Write to cart";
                        exclusiveGroup: readWriteExclusiveGroup;
                    }
                }
            }

            GroupBox {
                title: "Memory"

                RowLayout {
                    spacing: 20;

                    ExclusiveGroup {
                        id: sourceGroup;
                    }
                    RadioButton {
                        id: romButton;

                        text: "ROM (.gb/.gbc files)";
                        checked: true;
                        exclusiveGroup: sourceGroup;
                    }
                    RadioButton {
                        id: ramButton;

                        text: "RAM (.sav files)";
                        exclusiveGroup: sourceGroup;
                    }
                }
            }
        }

        Row {
            id: cartStatusRow;

            spacing: 5;

            anchors {
                horizontalCenter: parent.horizontalCenter;
            }

            Image {
                id: cartLed;

                source: cartController.ready ? "qrc:///images/green_led.svg" : "qrc:///images/red_led.svg";

                sourceSize.width: 16;
                sourceSize.height: 16;

                anchors {
                    verticalCenter: parent.verticalCenter;
                }
            }

            Label {
                id: cartStatusLabel;

                text: cartController.ready ? "Cart connected" : "Cart not connected";

                width: 135;
                anchors {
                    top: cartLed.top;
                    bottom: cartLed.bottom;
                }

                verticalAlignment: Text.AlignBottom;
            }

            Button {
                id: refreshCartButton;

                tooltip: "Refresh";

                width: height;
                anchors {
                    verticalCenter: parent.verticalCenter;
                }

                Image {
                    id: refreshIcon;

                    source: "qrc:///images/refresh.svg";

                    sourceSize.width: 16;
                    sourceSize.height: 16;

                    anchors {
                        centerIn: parent;
                    }
                }

                onClicked: {
                    cartController.refresh();
                }
            }
        }

        Row {
            id: filePathRow;

            anchors {
                horizontalCenter: parent.horizontalCenter;
            }

            TextField {
                id: pathTextField;

                text: fileDialogTitle(saveButton.checked, romButton.checked);
                readOnly: true;

                height: 25;
                width: mainView.width / 2;
            }

            Button {
                text: "...";

                width: 30;
                anchors {
                    top: pathTextField.top;
                    bottom: pathTextField.bottom;
                }
                onClicked: {
                    fileDialog.open();
                }
            }
        }

        Button {
            id: theButton;

            text: "Start";

            anchors {
                horizontalCenter: parent.horizontalCenter;
            }
        }
    }
}


