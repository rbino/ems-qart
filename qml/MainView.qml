import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.2
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

        spacing: (parent.height - (topPadding + bottomPadding) - (radioButtonsRow.height + cartStatusRow.height + theButton.height)) / 3;

        anchors {
            fill: parent;
        }

        RowLayout {
            id: radioButtonsRow;

            anchors {
                left: parent.left;
                right: parent.right;
            }

            GroupBox {
                id: readWriteGroupbox;

                title: "I/O direction";

                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;

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

                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;

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

                        text: "SRAM (.sav files)";
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

        Button {
            id: theButton;

            text: "Start";

            height: parent.height / 8;
            width: height * 2;

            style: ButtonStyle {
				label: Text {
					renderType: Text.NativeRendering;
					verticalAlignment: Text.AlignVCenter;
					horizontalAlignment: Text.AlignHCenter;
					font.pointSize: 18;
					text: control.text;
				}
			}

            anchors {
                horizontalCenter: parent.horizontalCenter;
            }
        }
    }
}


