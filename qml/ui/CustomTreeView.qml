import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    
    // Model must provide information about the `width` and `height` of the tree in its nodes
    required property var model

    anchors.fill: parent

    Flickable {
        id: flickable

        contentWidth: Math.max(treeDrawer.implicitWidth * treeViewEditor.zoom, width)
        contentHeight: Math.max(treeDrawer.implicitHeight * treeViewEditor.zoom, height)

        boundsMovement: Flickable.StopAtBounds
        clip: true

        anchors.fill: parent

        TreeDrawer {
            id: treeDrawer

            model: root.model

            scale: treeViewEditor.zoom

            node_size: treeViewEditor.node_size
            node_color: treeViewEditor.node_color

            distance_coefficient_by_x: treeViewEditor.distance_coefficient_by_x
            distance_coefficient_by_y: treeViewEditor.distance_coefficient_by_y

            anchors.centerIn: parent
        }
    }

    TreeViewEditor {
        id: treeViewEditor

        anchors {
            top: parent.top
            right: parent.right
            margins: 12
        }

        onCenteringRequest: {
            flickable.contentX = (flickable.contentWidth - root.width) / 2
            flickable.contentY = 0
                        
            flickable.returnToBounds()
        }
    }
}