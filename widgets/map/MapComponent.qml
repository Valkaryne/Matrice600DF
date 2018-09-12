import QtQuick 2.5
import QtLocation 5.9
import QtPositioning 5.6

Map {
    id: map
    anchors.fill: parent
    center: QtPositioning.coordinate(53.9, 27.56) // Minsk
    zoomLevel: 10

    property int lastX: -1
    property int lastY: -1
    property int pressX: -1
    property int pressY: -1

    property var homePointCoordinate

    function setMarker() {
        marker.coordinate = mouseArea.lastCoordinate
        var endLat = marker.coordinate.latitude
        var endLng = marker.coordinate.longitude

        var stLat1 = drone.coordinate.latitude;
        var stLng1 = drone.coordinate.longitude;
        var stLat2 = homePoint.coordinate.latitude;
        var stLng2 = homePoint.coordinate.longitude;

        var x01 = stLat1 / dLat;
        var y01 = stLng1 / dLng;
        var x02 = stLat2 / dLat;
        var y02 = stLng2 / dLng;
        var x1 = endLat / dLat;
        var y1 = endLng / dLng;

        var range_dr = Math.sqrt(Math.pow(x1 - x01, 2) + Math.pow(y1 - y01, 2));
        var range_hp = Math.sqrt(Math.pow(x1 - x02, 2) + Math.pow(y1 - y02, 2));

        mainview.setPointOnMap(endLat, endLng, range_dr, range_hp);

        dialogMission.latitude = endLat;
        dialogMission.longitude = endLng;
    }

    function setHomePoint() {
        homePoint.coordinate = homePointCoordinate
        var stLat = drone.coordinate.latitude;
        var stLng = drone.coordinate.longitude;
        var endLat = homePoint.coordinate.latitude;
        var endLng = homePoint.coordinate.longitude;

        homeDirection.path = [ { latitude: stLat, longitude: stLng }, { latitude: endLat, longitude: endLng } ]

        var x0 = stLat / dLat;
        var y0 = stLng / dLng;
        var x1 = endLat / dLat;
        var y1 = endLng / dLng;

        var range = Math.sqrt(Math.pow(x1-x0,2) + Math.pow(y1-y0,2));
        var radAngle = Math.acos((x1-x0)/range);

        var azimuth = radAngle * 180.0 / Math.PI;
        if ((y1-y0) < 0)
            azimuth *= -1;

        console.log("Azimuth: " + azimuth);
        mainview.setHomePoint(azimuth, endLat, endLng, range);
    }

    MapPolyline {
        id: homeDirection
        line.width: 3
        line.color: 'white'
        path: []
    }

    MouseArea {
        id: mouseArea
        property variant lastCoordinate
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressed: {
            map.lastX = mouse.x
            map.lastY = mouse.y
            map.pressX = mouse.x
            map.pressY = mouse.y
            lastCoordinate = map.toCoordinate(Qt.point(mouse.x, mouse.y))
        }

        onPositionChanged: {
            if (mouse.button === Qt.LeftButton) {
                map.lastX = mouse.x
                map.lastY = mouse.y
            }
        }

        onDoubleClicked: {
            var mouseGeoPos = map.toCoordinate(Qt.point(mouse.x, mouse.y));
            var preZoomPoint = map.fromCoordinate(mouseGeoPos, false);
            if (mouse.button === Qt.LeftButton) {
                map.zoomLevel = Math.floor(map.zoomLevel + 1)
            } else if (mouse.button === Qt.RightButton) {
                map.zoomLevel = Math.floor(map.zoomLevel - 1)
            }
            var postZoomPoint = map.fromCoordinate(mouseGeoPos, false);
            var dx = postZoomPoint.x - preZoomPoint.x;
            var dy = postZoomPoint.y - preZoomPoint.y;

            var mapCenterPoint = Qt.point(map.width / 2.0 + dx, map.height / 2.0 + dy);
            map.center = map.toCoordinate(mapCenterPoint);

            lastX = -1;
            lastY = -1;
        }

        onClicked: {
            if (mouse.button === Qt.LeftButton) {
                map.setMarker()
                dialogMission.open()
            }
            else if (mouse.button === Qt.RightButton)
            {
                homePointCoordinate = QtPositioning.coordinate(mouseArea.lastCoordinate.latitude, mouseArea.lastCoordinate.longitude)
                map.setHomePoint()
            }

        }
    }
}
