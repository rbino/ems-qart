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

        title: fileDialogTitle(saveButton.checked, romButton.checked);

        nameFilters: romButton.checked ? ["GB Roms (*.gb)", "GBC Roms (*.gbc)"] : ["GB/GBC Save files (*.sav)"];
        selectExisting: loadButton.checked;

        onAccepted: {
            pathTextField.text = decodeURIComponent(fileDialog.fileUrl.toString().replace(/^(file:\/{2})/,""));
        }
    }

    function fileDialogTitle(save, rom) {
        if (save) {
            if (rom) {
                return "Choose where the ROM will be saved";
            } else {
                return "Choose where the sav will be saved";
            }
        } else {
            if (rom) {
                return "Choose the ROM to be written to the cart";
            } else {
                return "Choose the sav to be written to the cart";
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
                id: saveLoadGroupbox;

                title: "In or out";

                RowLayout {
                    id: saveLoadRow;

                    spacing: 20;

                    ExclusiveGroup {
                        id: saveLoadExclusiveGroup;
                    }
                    RadioButton {
                        id: saveButton;

                        text: "Save from cart";
                        checked: true;
                        exclusiveGroup: saveLoadExclusiveGroup;
                    }
                    RadioButton {
                        id: loadButton;

                        text: "Load to cart";
                        exclusiveGroup: saveLoadExclusiveGroup;
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

                source: cartController.ready ? "qrc:///images/green_led.png" : "qrc:///images/red_led.png";

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


