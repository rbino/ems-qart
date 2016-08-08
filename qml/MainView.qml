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

    width: 800;
    height: 600;
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

        spacing: (parent.height - (topPadding + bottomPadding) - (radioButtonsRow.height + cartIconsRow.height + theButton.height)) / 2;

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

            GroupBox {
                title: "Bank"

                enabled: romButton.checked;

                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;

                RowLayout {
                    spacing: 20;

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
        }

        RowLayout {
            id: cartIconsRow;

            anchors {
                left: parent.left;
                right: parent.right;
            }

            Item {
                id: localFileItem;

                implicitHeight: localFileIcon.height;
                implicitWidth: localFileIcon.width;

                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;

                Image {
                    id: localFileIcon;

                    opacity: 0.08;

                    source: romButton.checked ? "../images/gb_cart.svg" : "../images/save_icon.svg";
                    sourceSize.height: mainView.height / 2.3;
                }

                Text {
                    id: choseFileText;

                    text: "Click to " + fileDialogTitle(readButton.checked, romButton.checked);

                    anchors {
                        fill: localFileIcon;
                        topMargin: localFileIcon.height * 0.28;
                        bottomMargin: localFileIcon.height * 0.135;
                        leftMargin: localFileIcon.height * 0.120;
                        rightMargin: localFileIcon.height * 0.120;
                    }

                    font {
                        italic: true;
                        pointSize: 12;
                    }

                    wrapMode: Text.WordWrap;

                    horizontalAlignment: Text.AlignHCenter;
                    verticalAlignment: Text.AlignVCenter;
                }
            }

            Image {
                id: arrowIcon;

                source: "../images/arrow.svg";
                sourceSize.height: 70;

                rotation: writeButton.checked ? 0 : 180;

                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;
            }

            Item {
                id: emsCartItem;

                implicitWidth: emsCartIcon.width;
                implicitHeight: emsCartIcon.height;

                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;

                Image {
                    id: emsCartIcon;

                    source: "../images/gb_cart_usb.svg";
                    sourceSize.height: mainView.height / 2.3;

                    opacity: cartController.ready ? 1 : 0.08;
                }

                Item {
                    id: cartStatus;

                    anchors {
                        left: emsCartIcon.left;
                        right: emsCartIcon.right;
                        bottom: emsCartIcon.top;
                        bottomMargin: 5;
                    }

                    Image {
                        id: cartLed;

                        source: cartController.ready ? "qrc:///images/green_led.svg" : "qrc:///images/red_led.svg";

                        sourceSize.width: 16;
                        sourceSize.height: 16;

                        anchors {
                            bottom: parent.bottom;
                        }
                    }

                    Label {
                        id: cartStatusLabel;

                        text: cartController.ready ? "Cart connected" : "Cart not connected";

                        width: 135;
                        anchors {
                            top: cartLed.top;
                            bottom: cartLed.bottom;
                            left: cartLed.right;
                            leftMargin: 5;
                        }

                        verticalAlignment: Text.AlignBottom;
                    }
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


