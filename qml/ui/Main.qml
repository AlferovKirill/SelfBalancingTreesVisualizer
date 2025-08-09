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

            TextField {
                id: valueField

                placeholderText: "value (int)"
                width: 120
                inputMethodHints: Qt.ImhDigitsOnly
            }
            Button {
                text: "Add"
                enabled: valueField.text.length > 0
                onClicked: {
                    flattened_tree_model.append(parseInt(valueField.text))
                    valueField.text = ""
                }
            }
            Item {
                Layout.fillWidth: true
            }
        }
    }

    ListView {
        id: list

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        
        model: flattened_tree_model
        delegate: Rectangle {
            required property var model

            required property int key
            required property int sum
            required property int node_height
            required property int size
            required property int top_offset
            required property int right_offset

            width: list.width
            height: 80

            RowLayout {
                spacing: 8
                anchors.fill: parent
            
                Text {
                    text: key
                    Layout.fillWidth: true
                }
                Button {
                    text: "x"
                    onClicked: flattened_tree_model.remove(key)
                }
            }
        }
    }

    // Loader {
    //     sourceComponent: tabBar.currentIndex === 0 ? redPage : tabBar.currentIndex === 1 ? greenPage : bluePage

    //     anchors.fill: parent
    // }

    Component {
        id: redPage
        Rectangle {
            color: "white"
            width: parent.width
            height: parent.height
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