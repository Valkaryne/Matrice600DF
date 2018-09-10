import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

Dialog {
    id: dialogMission
    width: 250
    height: 150

    property var latitude
    property var longitude
    property var marker: Marker

    property alias dialogButtonCancel: dialogButtonCancel
    property alias dialogButtonOk: dialogButtonOk
    property alias missionSwitch: missionSwitch
    property alias velocityField: velocityField
    property alias altitudeField: altitudeField

    contentItem: Rectangle {
        width: 250
        height: 150
        color: "#f7f7f7"

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            color: "#f7f7f7"
        }

        Row {
            id: missionRow
            height: 40
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                id: wpLabel
                text: "Waypoint"
                anchors.margins: 5
                anchors.right: missionSwitch.left
                anchors.verticalCenter: missionSwitch.verticalCenter
            }

            Switch {
                id: missionSwitch
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
                onCheckedChanged: {
                    if (missionSwitch.checked === true) {
                        velocityLabel.text = "Angular velocity:"
                        velocityUnitsLabel.text = "°/s"
                        marker.path = marker.hotpath
                    } else {
                        velocityLabel.text = "Linear velocity:"
                        velocityUnitsLabel.text = "m/s"
                        marker.path = marker.waypath
                    }
                }
            }

            Label {
                id: hpLabel
                text: "Hotpoint"
                anchors.margins: 5
                anchors.left: missionSwitch.right
                anchors.verticalCenter: missionSwitch.verticalCenter
            }
        }

        Row {
            id: velocityRow
            height: 40
            anchors.top: missionRow.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                id: velocityLabel
                text: "Linear velocity:"
                anchors.right: velocityField.left
                anchors.margins: 5
                anchors.verticalCenter: velocityField.verticalCenter
            }

            TextField {
                id: velocityField
                text: "15"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: 30
            }

            Label {
                id: velocityUnitsLabel
                text: "m/s"
                anchors.left: velocityField.right
                anchors.margins: 5
                anchors.verticalCenter: velocityField.verticalCenter
            }
        }

        Row {
            id: altitudeRow
            height: 40
            anchors.top: velocityRow.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                id: altitudeLabel
                text: "Altitude:"
                anchors.right: altitudeField.left
                anchors.margins: 5
                anchors.verticalCenter: altitudeField.verticalCenter
            }

            TextField {
                id: altitudeField
                text: "30"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: 30
            }

            Label {
                id: altitudeUnitsLabel
                text: "m"
                anchors.left: altitudeField.right
                anchors.margins: 5
                anchors.verticalCenter: altitudeField.verticalCenter
            }
        }

        Rectangle {
            id: separatorHorizontal
            color: "#d7d7d7"
            height: 2
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: row.top
        }

        Row {
            id: row
            height: 40
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            Button {
                id: dialogButtonCancel
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width / 2 - 1
                style: ButtonStyle {
                    background: Rectangle {
                        color: control.pressed ? "#d7d7d7" : "#f7f7f7"
                        border.width: 0
                    }

                    label: Text {
                        text: qsTr("Cancel")
                        color: "#34aadc"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }

            Rectangle {
                id: separatorVertical
                width: 2
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                color: "#d7d7d7"
            }

            Button {
                id: dialogButtonOk
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width / 2 - 1

                style: ButtonStyle {
                    background: Rectangle {
                        color: control.pressed ? "#d7d7d7" : "#f7f7f7"
                        border.width: 0
                    }

                    label: Text {
                        text: qsTr("Ok")
                        color: "#34aadc"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }
    }
}
