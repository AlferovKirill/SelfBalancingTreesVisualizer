import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes

Item {
    id: root

    // Model must provide information about the `width` and `height` of the tree in its nodes
    required property var model

    required property int node_size
    required property color node_color

    required property real distance_coefficient_by_x
    required property real distance_coefficient_by_y

    implicitWidth: childrenRect.width
    implicitHeight: childrenRect.height

    Repeater {
        model: root.model
        delegate: Item {
            id: nodeDelegate

            required property int key
            required property int sum
            required property int depth
            required property int horizontal_offset

            required property bool left_exists
            required property int left_depth
            required property int left_horizontal_offset

            required property bool right_exists
            required property int right_depth
            required property int right_horizontal_offset

            x: horizontal_offset * root.node_size * root.distance_coefficient_by_x
            y: depth * root.node_size * root.distance_coefficient_by_y

            implicitHeight: root.node_size
            implicitWidth: root.node_size

            Shape {
                visible: left_exists

                ShapePath {
                    strokeWidth: 4
                    strokeColor: root.node_color

                    startX: root.node_size / 2
                    startY: root.node_size / 2

                    PathLine {
                        x: root.node_size / 2 - Math.abs(horizontal_offset - left_horizontal_offset) * root.node_size * root.distance_coefficient_by_x
                        y: root.node_size / 2 + Math.abs(depth - left_depth) * root.node_size * root.distance_coefficient_by_y
                    }
                }
            }
            Shape {
                visible: right_exists

                ShapePath {
                    strokeWidth: 4
                    strokeColor: root.node_color

                    startX: root.node_size / 2
                    startY: root.node_size / 2

                    PathLine {
                        x: root.node_size / 2 + Math.abs(horizontal_offset - right_horizontal_offset) * root.node_size * root.distance_coefficient_by_x
                        y: root.node_size / 2 + Math.abs(depth - right_depth) * root.node_size * root.distance_coefficient_by_y
                    }
                }
            }
            Rectangle {
                anchors.fill: parent

                radius: height / 2
                color: root.node_color

                border {
                    width: 4
                    color: Qt.darker(root.node_color, 1.5)
                }

                Text {
                    text: key
                    anchors.centerIn: parent
                }
            }
        }
    }
}