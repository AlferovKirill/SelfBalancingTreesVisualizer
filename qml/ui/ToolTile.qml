import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Pane {
    id: root

    property color activeColor: systemPalette.highlight
    property color backgroundColor: systemPalette.window

    property alias labelText: label.text
    property alias buttonText: button.text
    property alias placeholderText: textField.placeholderText
    property alias textFieldText: textField.text

    signal clicked

    width: mainLayout.contentWidth
    height: mainLayout.contentHeight

    padding: 12

    background: Rectangle {
        color: root.backgroundColor
        radius: 8
        
        border {
            color: textField.activeFocus ? root.activeColor : Qt.darker(root.backgroundColor, 1.5)
            width: 2
        }
    }

    SystemPalette {
        id: systemPalette
        colorGroup: SystemPalette.Active
    }

    ColumnLayout {
        id: mainLayout

        spacing: 6
        anchors.fill: parent

        Label {
            id: label

            color: root.activeColor

            font.bold: true
            Layout.fillWidth: true
        }
        RowLayout {
            spacing: 8
            Layout.fillWidth: true

            TextField {
                id: textField
                
                inputMethodHints: Qt.ImhDigitsOnly
                Layout.fillWidth: true
                
                background: Rectangle {
                    color: Qt.darker(root.backgroundColor, 1.5)
                    radius: 8

                    border {
                        color: parent.activeFocus ? root.activeColor : Qt.darker(root.backgroundColor, 2)
                        width: 1
                    }
                }
                
                onAccepted: {
                    if (button.enabled) {
                        root.clicked()
                    }
                }
            }
            Button {
                id: button
                
                text: "+"
                enabled: textField.text.length > 0 && textField.acceptableInput
                
                onClicked: {
                    root.clicked()
                }
            }
        }
    }
}