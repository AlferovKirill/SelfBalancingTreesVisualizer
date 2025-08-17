import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Pane {
    id: root

    // Model must provide `insert`, `remove` and `find` methods
    required property var model

    width: mainLayout.contentWidth
    height: mainLayout.contentHeight

    padding: 12

    background: Rectangle {
        color: systemPalette.window
        opacity: 0.8
        
        radius: 8

        border {
            color: systemPalette.dark
            width: 2
        }
    }

    SystemPalette {
        id: systemPalette
        colorGroup: SystemPalette.Active
    }

    RowLayout {
        id: mainLayout

        spacing: 12
        anchors.fill: parent

        ToolTile {
            buttonText: "+"
            labelText: qsTr("Add node")
            placeholderText: qsTr("Value")

            activeColor: "#34A853"

            onClicked: {
                const value = parseInt(textFieldText)

                if (!root.model.find(value)) {
                    root.model.insert(value)
                    textFieldText.text = ""

                    statusLabel.showSuccess("Узел " + value + " добавлен")
                } else {
                    statusLabel.showError("Узел " + value + " уже существует")
                }
            }
        }
        ToolTile {
            buttonText: "-"
            labelText: qsTr("Remove node")
            placeholderText: qsTr("Value")

            activeColor: "#EA4335"

            onClicked: {
                const value = parseInt(textFieldText)

                if (root.model.find(value)) {
                    root.model.remove(value)
                    textFieldText.text = ""

                    statusLabel.showSuccess("Узел " + value + " удален")
                } else {
                    statusLabel.showError("Узел " + value + " не найден")
                }
            }
        }
        ToolTile {
            buttonText: "?"
            labelText: qsTr("Find node")
            placeholderText: qsTr("Value")

            activeColor: "#4285F4"

            onClicked: {
                const value = parseInt(textFieldText)
                const found = root.model.find(value)
                
                if (found) {
                    statusLabel.showSuccess("Узел " + value + " найден")
                    // TODO: Можно добавить подсветку найденного узла в дереве
                } else {
                    statusLabel.showError("Узел " + value + " не найден")
                }
            }
        }
        Item {
            Layout.fillWidth: true
        }
        ColumnLayout {
            spacing: 4
            Layout.preferredWidth: 150

            Label {
                id: statusLabel
                
                text: "Готов к работе"
                color: "#AAAAAA"
                font.pixelSize: 11
                horizontalAlignment: Text.AlignRight
                Layout.fillWidth: true
                
                property var hideTimer: Timer {
                    interval: 3000
                    onTriggered: {
                        statusLabel.text = "Готов к работе"
                        statusLabel.color = "#AAAAAA"
                    }
                }
                
                function showSuccess(message) {
                    text = message
                    color = "#34A853"
                    hideTimer.restart()
                }
                
                function showError(message) {
                    text = message
                    color = "#EA4335"
                    hideTimer.restart()
                }
            }

            Label {
                text: "Узлов: " + root.model.rowCount()
                color: "#AAAAAA"
                font.pixelSize: 10
                horizontalAlignment: Text.AlignRight
                Layout.fillWidth: true
            }
        }
    }
}