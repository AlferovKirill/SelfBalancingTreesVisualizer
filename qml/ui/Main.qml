import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root

    visible: true

    width: 800
    height: 600

    minimumWidth: 196
    minimumHeight: 128

    title: Qt.application.name

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
            title: qsTr("Settings")
            
            Menu {
                title: qsTr("Language")
                
                Repeater {
                    model: translation_controller.supportedLanguagesList
                    delegate: MenuItem {
                        text: modelData
                        checked: translation_controller.currentLanguage === modelData
                        checkable: true
                        
                        onTriggered: {
                            translation_controller.setLanguage(modelData)
                        }
                    }
                }
            }
        }
        Menu {
            title: qsTr("About")

            Action {
                text: qsTr("Help")
            }
            Action {
                text: qsTr("About program")
                onTriggered: {
                    aboutDialog.open()
                }
            }
        }
    }

    footer: Rectangle {
        color: "grey"
        height: 48
        width: parent.width

        RowLayout {
            spacing: 8
            anchors.fill: parent

            Button {
                text: "click to expandRecursively"
                onClicked: {
                    treeView.expandRecursively()
                }
            }
            Text {
                text: "text " + treeView.rows
                Layout.fillWidth: true
            }
        }
    }

    TreeView {
        anchors.fill: parent
        // The model needs to be a QAbstractItemModel
        model: tree_view_model

        delegate: Item {
            id: treeDelegate

            implicitWidth: padding + label.x + label.implicitWidth + padding
            implicitHeight: label.implicitHeight * 1.5

            readonly property real indent: 20
            readonly property real padding: 5

            // Assigned to by TreeView:
            required property TreeView treeView
            required property bool isTreeNode
            required property bool expanded
            required property int hasChildren
            required property int depth

            TapHandler {
                onTapped: treeView.toggleExpanded(row)
            }

            Text {
                id: indicator
                visible: treeDelegate.isTreeNode && treeDelegate.hasChildren
                x: padding + (treeDelegate.depth * treeDelegate.indent)
                anchors.verticalCenter: label.verticalCenter
                text: "▸"
                rotation: treeDelegate.expanded ? 90 : 0
            }

            Text {
                id: label
                x: padding + (treeDelegate.isTreeNode ? (treeDelegate.depth + 1) * treeDelegate.indent : 0)
                width: treeDelegate.width - treeDelegate.padding - x
                clip: true
                text: model.key
            }
        }
    }

    // Loader {
    //     sourceComponent: tabBar.currentIndex === 0 ? redPage : tabBar.currentIndex === 1 ? greenPage : bluePage

    //     anchors.fill: parent
    // }

    // Component {
    //     id: redPage
    //     Rectangle {
    //         color: "white"
    //         width: parent.width
    //         height: parent.height
    //     }
    // }
    // Component {
    //     id: greenPage
    //     Rectangle {
    //         color: "green"
    //         width: parent.width
    //         height: parent.height

    //         Image {
    //             source: "qrc:/images/forest-green-plant-icon.png"

    //             width: 60
    //             height: 60
    //         }
    //     }
    // }
    // Component {
    //     id: bluePage
    //     Rectangle {
    //         color: "blue"
    //         width: parent.width
    //         height: parent.height
    //     }
    // }

    Dialog {
        id: aboutDialog

        title: qsTr("About program")
        standardButtons: Dialog.Ok
        
        modal: true
        anchors.centerIn: Overlay.overlay

        padding: 16

        ColumnLayout {
            spacing: 8
            anchors.fill: parent

            Text {
                text: qsTr("Author: ") + Qt.application.organization
                Layout.fillWidth: true
            }
            Text {
                text: qsTr("Version: ") + Qt.application.version
                Layout.fillWidth: true
            }
            Text {
                text: qsTr("Homepage:") + ' <a href="' + Qt.application.domain + '">' + Qt.application.domain + '</a>'
                textFormat: Text.RichText
                wrapMode: Text.Wrap

                Layout.fillWidth: true

                onLinkActivated: function(link) {
                    Qt.openUrlExternally(link)
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }
}