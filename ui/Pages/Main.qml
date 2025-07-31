import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root

    visible: true
    width: 400
    height: 600

    title: qsTr("Program")

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
        }
    }
    Component {
        id: greenPage
        Rectangle {
            color: "green"
            width: parent.width
            height: parent.height
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
