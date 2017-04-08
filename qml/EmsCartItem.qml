import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import EmsQart 1.0

Item {
    id: emsCartItem;

    CartController {
        id: cartController;
    }

    implicitWidth: emsCartIcon.width;
    implicitHeight: emsCartIcon.height;

    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter;

    Image {
        id: emsCartIcon;

        anchors {
            centerIn: parent;
        }

        source: "qrc:///images/gb_cart_usb.svg";
        sourceSize.height: mainView.height / 2.3;

        opacity: cartController.ready ? 1 : 0.08;

        Behavior on opacity {
            NumberAnimation {
                duration: 100;
            }
        }

        MouseArea {
            id: refreshCartMouseArea;

            anchors {
                fill: parent;
            }

            onClicked: {
                cartController.refresh();
            }
        }

        BusyIndicator {
            id: busyIndicator;

            running: cartController.busy;

            anchors {
                centerIn: parent;
            }
        }
    }

    Text {
        id: refreshText;

        visible: !cartController.ready;

        text: "Click to refresh EMS cart status";

        anchors {
            fill: emsCartIcon;
            topMargin: emsCartIcon.height * 0.28;
            bottomMargin: emsCartIcon.height * 0.135;
            leftMargin: emsCartIcon.height * 0.120;
            rightMargin: emsCartIcon.height * 0.120;
        }

        font {
            italic: true;
            pointSize: 12;
        }

        wrapMode: Text.WordWrap;

        horizontalAlignment: Text.AlignHCenter;
        verticalAlignment: Text.AlignVCenter;
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

    ProgressBar {
        id: progressBar;

        visible: cartController.busy;
        value: cartController.progress;

        anchors {
            top: emsCartIcon.bottom;
            topMargin: 50;
            left: emsCartIcon.left;
            right: emsCartIcon.right;
        }
    }
}
