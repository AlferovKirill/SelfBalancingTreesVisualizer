import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root

    visible: true
    width: 400
    height: 600

    title: qsTr("SelfBalancingTreesVisualizer")

    header: TabBar {
        id: tabBar

        currentIndex: 0

        width: parent.width

        TabButton {
            text: qsTr("Red")
        }
        TabButton {
            text: qsTr("Green")
        }
        TabButton {
            text: qsTr("Blue")
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")

            Action {
                text: qsTr("New");
                shortcut: StandardKey.New
            }
            Action {
                text: qsTr("Open");
                shortcut: StandardKey.Open
            }
            Action {
                text: qsTr("Save");
                shortcut: StandardKey.Save
            }
            MenuSeparator {}
            Action {
                text: qsTr("Exit");
                shortcut: StandardKey.Quit;
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: qsTr("Edit")
            
            Action {
                text: qsTr("Redo");
                shortcut: StandardKey.Redo
            }
            Action {
                text: qsTr("Undo");
                shortcut: StandardKey.Undo
            }
            MenuSeparator {}
            Action {
                text: qsTr("Copy");
                shortcut: StandardKey.Copy
            }
            Action {
                text: qsTr("Paste");
                shortcut: StandardKey.Paste
            }
            Action {
                text: qsTr("Cut");
                shortcut: StandardKey.Cut
            }
        }
        Menu {
            title: qsTr("About")

            Action {
                text: qsTr("Help")
            }
            Action {
                text: qsTr("About program")
            }
        }
    }

    footer: Rectangle {
        color: "grey"

        height: 48
        width: parent.width
    }
    
    Loader {
        sourceComponent: tabBar.currentIndex === 0 ? redPage : tabBar.currentIndex === 1 ? greenPage : bluePage

        anchors.fill: parent
    }

    Component {
        id: redPage
        Rectangle {
            color: "red"
            width: parent.width
            height: parent.height

            Image {
                source: "qrc:/images/ForestGreenPlantIcon.svg"

                width: 60
                height: 60
            }
        }
    }
    Component {
        id: greenPage
        Rectangle {
            color: "green"
            width: parent.width
            height: parent.height

            Image {
                source: "qrc:/images/forest-green-plant-icon.png"

                width: 60
                height: 60
            }
        }
    }
    Component {
        id: bluePage
        Rectangle {
            color: "blue"
            width: parent.width
            height: parent.height
        }
    }
}
