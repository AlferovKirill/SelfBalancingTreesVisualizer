import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes

Item {
    id: root

    readonly property int k_node_size: 56
    readonly property real k_distance_coefficient: 1.618
    readonly property color k_nodeColor: "#34A853"

    // Model must provide information about the `width` and `height` of the tree in its nodes
    required property var model

    property int node_size: k_node_size
    property real distance_coefficient: k_distance_coefficient
    property color nodeColor: k_nodeColor

    implicitWidth: childrenRect.width
    implicitHeight: childrenRect.height

    function resetParameters() {
        node_size = k_node_size
        distance_coefficient = k_distance_coefficient
        nodeColor = k_nodeColor
    }

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

            x: horizontal_offset * root.node_size * root.distance_coefficient
            y: depth * root.node_size * root.distance_coefficient

            implicitHeight: root.node_size
            implicitWidth: root.node_size

            Shape {
                visible: left_exists

                ShapePath {
                    strokeWidth: 4
                    strokeColor: root.nodeColor

                    startX: root.node_size / 2
                    startY: root.node_size / 2

                    PathLine {
                        x: root.node_size / 2 - Math.abs(horizontal_offset - left_horizontal_offset) * root.node_size * root.distance_coefficient
                        y: root.node_size / 2 + Math.abs(depth - left_depth) * root.node_size * root.distance_coefficient
                    }
                }
            }
            Shape {
                visible: right_exists

                ShapePath {
                    strokeWidth: 4
                    strokeColor: root.nodeColor

                    startX: root.node_size / 2
                    startY: root.node_size / 2

                    PathLine {
                        x: root.node_size / 2 + Math.abs(horizontal_offset - right_horizontal_offset) * root.node_size * root.distance_coefficient
                        y: root.node_size / 2 + Math.abs(depth - right_depth) * root.node_size * root.distance_coefficient
                    }
                }
            }
            Rectangle {
                anchors.fill: parent

                radius: height / 2
                color: root.nodeColor

                border {
                    width: 4
                    color: Qt.darker(root.nodeColor, 1.5)
                }

                Text {
                    text: key
                    anchors.centerIn: parent
                }
            }
        }
    }
}