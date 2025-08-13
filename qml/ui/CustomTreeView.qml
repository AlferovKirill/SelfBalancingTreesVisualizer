import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes

Item {
    id: root

    readonly property real k_zoom: 1.0
    readonly property real min_zoom: 0.1
    readonly property real max_zoom: 4.0
    
    required property var model

    property real zoom: k_zoom

    anchors.fill: parent

    function resetParameters() {
        zoom = k_zoom
        treeDrawer.resetParameters()
    }

    Flickable {
        id: flickable

        contentWidth: Math.max(treeDrawer.implicitWidth * root.zoom, width)
        contentHeight: Math.max(treeDrawer.implicitHeight * root.zoom, height)

        boundsBehavior: Flickable.StopAtBounds
        clip: true

        anchors.fill: parent

        TreeDrawer {
            id: treeDrawer

            model: root.model
            scale: root.zoom

            anchors.centerIn: parent
        }
    }

    Pane {
        padding: 10

        background: Rectangle {
            color: "#202225"
            opacity: 0.8
            
            radius: 8

            border {
                color: "#40444b"
                width: 2
            }
        }

        anchors {
            top: parent.top
            right: parent.right
            margins: 12
        }

        ColumnLayout {
            spacing: 8
            width: 272

            Label {
                text: "Просмотр"
                font.bold: true

                Layout.fillWidth: true
            }
            RowLayout {
                spacing: 8

                Label {
                    text: "Зум"
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
                    text: Math.round(zoom * 100) + "%"
                    horizontalAlignment: Text.AlignRight

                    Layout.preferredWidth: 50
                }
            }
            RowLayout {
                spacing: 8

                Label {
                    text: "Размер узла"
                }
                Slider {
                    value: treeDrawer.node_size
                    from: 16
                    to: 160

                    stepSize: 1

                    Layout.fillWidth: true

                    onValueChanged: {
                        if (value !== treeDrawer.node_size) {
                            treeDrawer.node_size = Math.round(value)
                        }
                    }
                }
                Label {
                    text: treeDrawer.node_size + "px"
                    horizontalAlignment: Text.AlignRight

                    Layout.preferredWidth: 50
                }
            }
            RowLayout {
                spacing: 8

                Label {
                    text: "Коэфф. расстояния"
                }
                Slider {
                    value: treeDrawer.distance_coefficient
                    from: 0.25
                    to: 15.0

                    stepSize: 0.01
                    
                    Layout.fillWidth: true
                    
                    onValueChanged: {
                        if (value !== treeDrawer.distance_coefficient) {
                            treeDrawer.distance_coefficient = Number(value.toFixed(3))
                        }
                    }
                }
                Label {
                    text: treeDrawer.distance_coefficient.toFixed(3)
                    horizontalAlignment: Text.AlignRight

                    Layout.preferredWidth: 50
                }
            }
            RowLayout {
                spacing: 6

                Label {
                    text: "Цвет узлов"
                }
                Repeater {
                    model: [ "#34A853", "#3498db", "#e67e22", "#e74c3c", "#9b59b6", "#f1c40f", "#95a5a6" ]
                    delegate: Rectangle {
                        width: 20; height: 20; radius: 4

                        color: modelData
                        border {
                            width: treeDrawer.nodeColor === modelData ? 2 : 1
                            color: treeDrawer.nodeColor === modelData ? "white" : "#222"
                        }

                        HoverHandler {
                            id: hover
                        }

                        ToolTip.visible: hover.hovered
                        ToolTip.text: modelData

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                treeDrawer.nodeColor = modelData
                            }
                        }
                    }
                }
                TextField {
                    text: treeDrawer.nodeColor
                    validator: RegularExpressionValidator { regularExpression: /^#?[0-9a-fA-F]{6}$/ }

                    Layout.preferredWidth: 90
                    
                    onEditingFinished: {
                        let input_text = text.trim()

                        if (!input_text.startsWith("#")) {
                            input_text = "#" + input_text
                        }

                        if (/^#[0-9a-fA-F]{6}$/.test(input_text)) {
                            treeDrawer.nodeColor = input_text
                        }
                    }
                }
            }
            RowLayout {
                spacing: 8

                Item {
                    Layout.fillWidth: true
                }
                Button {
                    text: "Центр"
                    onClicked: {
                        flickable.contentX = (flickable.contentWidth - root.width) / 2
                        flickable.contentY = 0
                        
                        flickable.returnToBounds()
                    }
                }
                Button {
                    text: "Сброс"
                    onClicked: {
                        root.resetParameters()

                        flickable.contentX = (flickable.contentWidth - root.width) / 2
                        flickable.contentY = 0
                        
                        flickable.returnToBounds()
                    }
                }
            }
        }
    }
}