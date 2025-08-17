import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Pane {
    id: root

    readonly property real k_zoom: 1.0
    readonly property real min_zoom: 0.1
    readonly property real max_zoom: 4.0

    readonly property int k_node_size: 56
    readonly property color k_node_color: colorScheme[0]
    
    readonly property real k_distance_coefficient_by_x: 1.618
    readonly property real k_distance_coefficient_by_y: 2

    readonly property var colorScheme: [ "#34A853", "#4285F4", "#EA4335", "#FBBC05", "#dd88ff", "#c5c5c5" ]

    property real zoom: k_zoom

    property int node_size: k_node_size
    property color node_color: k_node_color
    
    property real distance_coefficient_by_x: k_distance_coefficient_by_x
    property real distance_coefficient_by_y: k_distance_coefficient_by_y

    signal centeringRequest

    function resetParameters() {
        zoom = k_zoom
        
        node_size = k_node_size
        node_color = k_node_color
        
        distance_coefficient_by_x = k_distance_coefficient_by_x
        distance_coefficient_by_y = k_distance_coefficient_by_y
    }

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

    ColumnLayout {
        id: mainLayout

        spacing: 8
        anchors.fill: parent

        Label {
            text: qsTr("View")
            font.bold: true

            Layout.fillWidth: true
        }
        RowLayout {
            spacing: 8
            Layout.fillWidth: true

            Label {
                text: qsTr("Zoom")
            }
            Slider {
                value: root.zoom

                from: root.min_zoom
                to: root.max_zoom

                stepSize: 0.01
                
                Layout.fillWidth: true
                
                onValueChanged: {
                    if (value !== root.zoom) {
                        root.zoom = value
                    }
                }
            }
            Label {
                text: Math.round(root.zoom * 100) + "%"
                horizontalAlignment: Text.AlignRight

                Layout.preferredWidth: 50
            }
        }
        RowLayout {
            spacing: 8
            Layout.fillWidth: true

            Label {
                text: qsTr("Node size")
            }
            Slider {
                value: root.node_size
                from: 16
                to: 160

                stepSize: 1

                Layout.fillWidth: true

                onValueChanged: {
                    if (value !== root.node_size) {
                        root.node_size = Math.round(value)
                    }
                }
            }
            Label {
                text: root.node_size + "px"
                horizontalAlignment: Text.AlignRight

                Layout.preferredWidth: 50
            }
        }
        RowLayout {
            spacing: 8
            Layout.fillWidth: true

            Label {
                text: qsTr("Distance coefficient by X")
            }
            Slider {
                value: root.distance_coefficient_by_x
                from: 0.25
                to: 15.0

                stepSize: 0.01
                
                Layout.fillWidth: true
                
                onValueChanged: {
                    if (value !== root.distance_coefficient_by_x) {
                        root.distance_coefficient_by_x = Number(value.toFixed(3))
                    }
                }
            }
            Label {
                text: root.distance_coefficient_by_x.toFixed(3)
                horizontalAlignment: Text.AlignRight

                Layout.preferredWidth: 50
            }
        }
        RowLayout {
            spacing: 8
            Layout.fillWidth: true

            Label {
                text: qsTr("Distance coefficient by Y")
            }
            Slider {
                value: root.distance_coefficient_by_y
                from: 0.25
                to: 15.0

                stepSize: 0.01
                
                Layout.fillWidth: true
                
                onValueChanged: {
                    if (value !== root.distance_coefficient_by_y) {
                        root.distance_coefficient_by_y = Number(value.toFixed(3))
                    }
                }
            }
            Label {
                text: root.distance_coefficient_by_y.toFixed(3)
                horizontalAlignment: Text.AlignRight

                Layout.preferredWidth: 50
            }
        }
        RowLayout {
            spacing: 8
            Layout.fillWidth: true

            Label {
                text: qsTr("Node color")
            }
            Item {
                Layout.fillWidth: true
            }
            Repeater {
                model: root.colorScheme
                delegate: Rectangle {
                    width: 20; height: 20; radius: 4

                    color: modelData
                    border {
                        color: root.node_color === Qt.color(modelData) ? Qt.lighter(modelData, 1.25) : Qt.darker(modelData, 1.25)
                        width: 2
                    }

                    HoverHandler {
                        id: hover
                    }

                    ToolTip.visible: hover.hovered
                    ToolTip.text: modelData

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            root.node_color = modelData
                        }
                    }
                }
            }
            TextField {
                text: root.node_color
                validator: RegularExpressionValidator { regularExpression: /^#?[0-9a-fA-F]{6}$/ }

                Layout.preferredWidth: 90
                
                onEditingFinished: {
                    let input_text = text.trim()

                    if (!input_text.startsWith("#")) {
                        input_text = "#" + input_text
                    }

                    if (/^#[0-9a-fA-F]{6}$/.test(input_text)) {
                        root.node_color = input_text
                    }
                }
            }
        }
        RowLayout {
            spacing: 8
            Layout.fillWidth: true

            Item {
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("Center")
                onClicked: {
                    root.centeringRequest()
                }
            }
            Button {
                text: qsTr("Reset")
                onClicked: {
                    root.resetParameters()
                    root.centeringRequest()
                }
            }
        }
    }
}