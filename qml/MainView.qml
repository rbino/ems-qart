import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import EmsQart 1.0

ApplicationWindow {
    id: mainView;

    title: "ems-qart";

    visible: true;

    width: 1050;
    height: 700;

    minimumWidth: 1050;
    minimumHeight: 700;

    MessageDialog {
        id: messageDialog;
    }

    CartController {
        id: cartController;

        onTransferCompleted: {
            clearLocalFilePath();

            messageDialog.title = "Finished";
            messageDialog.text = "Transfer completed!";
            messageDialog.icon = StandardIcon.Information;
            messageDialog.open();
        }

        onError: {
            messageDialog.title = "Error";
            messageDialog.text = message;
            messageDialog.icon = StandardIcon.Critical;
            messageDialog.open();
        }
    }

    TabView {
        id: tabView;

        anchors {
            fill: parent;
        }

        StatusTabPage {
            id: statusTab;
            title: "Status";
        }
        TabPage {
            id: loadTab;
            title: "Load";
        }
        TabPage {
            id: saveTab;
            title: "Save";
        }
        TabPage {
            id: utilsTab;
            title: "Utils";
        }
    }
}


