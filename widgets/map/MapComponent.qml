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
        var lat = marker.coordinate.latitude
        var lng = marker.coordinate.longitude
        mainview.setPointOnMap(lat, lng)
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
        mainview.setHomePoint(azimuth);
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
            if (mouse.button === Qt.LeftButton)
                map.setMarker()
            else if (mouse.button === Qt.RightButton)
            {
                homePointCoordinate = mouseArea.lastCoordinate
                map.setHomePoint()
            }

        }
    }
}
