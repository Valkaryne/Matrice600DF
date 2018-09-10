import QtQuick 2.5
import QtLocation 5.9
import QtPositioning 5.6

Item {
    id: mainItem
    //width: parent.width
    //height: parent.height
    anchors.fill: parent
    visible: true

    property var map
    property var marker
    property var homePoint
    property var drone

    property MapPolyline beam
    property MapPolyline droneVector
    property variant beams : []

    property real dLng : 0.000008983152841195396 * 1.69;
    property real dLat : 0.000009031055490301464;

    Plugin {
        id: osmPlugin
        name: "osm"
        PluginParameter { name: "osm.mapping.cache.directory"; value: "F:/maposm_cache/" }
    }

    Plugin {
        id: esriPlugin
        name: "esri"
        PluginParameter { name: "esri.mapping.cache.directory"; value: "F:/mapesri_cache/" }
    }

    MissionDialog {
        id: dialogMission
        dialogButtonCancel.onClicked: dialogMission.close()
        dialogButtonOk.onClicked: {
            console.log("Lat: " + dialogMission.latitude)
            console.log("Lng: " + dialogMission.longitude)
            printParameters(dialogMission.missionSwitch.checked, dialogMission.velocityField.text,
                            dialogMission.altitudeField.text)
            dialogMission.close()
        }
    }

    function printParameters(type, velocity, altitude)
    {
        var missionType, missionVelocity, missionAltitude
        if (type === true) {
            missionType = 1
            //marker.path = marker.hotpath
        } else {
            missionType = 0
            //marker.path = marker.waypath
        }
    }

    function initializeMapItems()
    {
        console.log("Cat here")
        map = Qt.createQmlObject('MapComponent {}', mainItem)
        map.plugin = osmPlugin
        marker = Qt.createQmlObject('Marker {}', map)
        drone = Qt.createQmlObject('Drone {}', map)
        homePoint = Qt.createQmlObject('Home {}', map)
        droneVector = Qt.createQmlObject('import QtLocation 5.11; MapPolyline {}', map)
        map.addMapItem(marker)
        map.addMapItem(drone)
        map.addMapItem(homePoint)
        map.addMapItem(droneVector)
        marker.z = map.z + 1
        dialogMission.marker = marker
        console.log("Map is initilized")
    }

    function updateDroneLocation(lat, lng, heading)
    {
        drone.coordinate.latitude = lat;
        drone.coordinate.longitude = lng;
        drone.rotation = heading;

        var stLat = lat;
        var stLng = lng;
        var x0 = stLat / dLat;
        var y0 = stLng / dLng;
        var angle = heading * Math.PI / 180;
        var x1 = Math.cos(angle)*3000 + x0;
        var y1 = Math.sin(angle)*3000 + y0;
        var endLat = x1 * dLat;
        var endLng = y1 * dLng;
        droneVector.path = [ { latitude: stLat, longitude: stLng }, { latitude: endLat, longitude: endLng } ]
    }

    function makeBeam(direction)
    {
        var count = beams.length
        var stLat = drone.coordinate.latitude;
        var stLng = drone.coordinate.longitude;
        var x0 = stLat / dLat;
        var y0 = stLng / dLng;
        var angle = (direction + 2.5) * Math.PI / 180;
        var x1 = Math.cos(angle)*3000 + x0;
        var y1 = Math.sin(angle)*3000 + y0;
        var endLat = x1 * dLat;
        var endLng = y1 * dLng;
        beam = Qt.createQmlObject('import QtLocation 5.11; MapPolyline {}', map)
        beam.line.width = 3
        beam.line.color = 'red'
        beam.path = [ { latitude: stLat, longitude: stLng }, { latitude: endLat, longitude: endLng } ]
        map.addMapItem(beam)

        var myArray = new Array
        for (var i = 0; i < count; i++) {
            myArray.push(beams[i])
        }
        myArray.push(beam)
        beams = myArray
    }

    function clearMap() {
        var count = beams.length
        for (var i = 0; i < count; i++) {
            map.removeMapItem(beams[i])
            beams[i].destroy()
        }
        beams = []
    }

    function switchProvider()
    {
        var name = map.plugin.name
        var center = map.center
        var zoomLevel = map.zoomLevel
        map = Qt.createQmlObject('MapComponent {}', mainItem)
        if (name === "esri")
            map.plugin = osmPlugin
        else if (name === "osm") {
            map.plugin = esriPlugin
            map.activeMapType = map.supportedMapTypes[1]
        }
        map.center = center
        map.zoomLevel = zoomLevel

        marker = Qt.createQmlObject('Marker {}', map)
        map.addMapItem(marker)
        dialogMission.marker = marker

        drone = Qt.createQmlObject('Drone {}', map)
        map.addMapItem(drone)

        homePoint = Qt.createQmlObject('Home {}', map)
        map.addMapItem(homePoint)

        droneVector = Qt.createQmlObject('import QtLocation 5.11; MapPolyline {}', map)
        map.addMapItem(droneVector)

        console.log("Provider was changed to " + map.plugin.name)
    }

    function updateHomeDirection()
    {
        map.setHomePoint()
    }

    function getMapProvider()
    {
        var name = map.plugin.name;
        if (name === "osm")
            return 0;
        else if (name === "esri")
            return 1;
    }

    function getMapCenter()
    {
        var center = map.center;
        var centerCoordinates = Qt.vector2d(center.latitude, center.longitude);
        return centerCoordinates;
    }

    function getMapZoomLevel()
    {
        var zoomLevel = map.zoomLevel;
        return zoomLevel;
    }

    function getMapHomePoint()
    {
        var hp = homePoint.coordinate;
        var hpCoordinates = Qt.vector2d(hp.latitude, hp.longitude);
        return hpCoordinates;
    }

    function setMapProvider(provider)
    {
        var name;
        if (provider == 0)
            name = "osm";
        else if (provider == 1)
            name = "esri";

        if (map.plugin.name !== name)
            switchProvider();
    }

    function setMapCenter(centerLat, centerLng)
    {
        map.center.latitude = centerLat;
        map.center.longitude = centerLng;
    }

    function setMapZoomLevel(zoomLevel)
    {
        map.zoomLevel = zoomLevel;
    }

    function setMapHomePoint(hpLat, hpLng)
    {
        homePoint.coordinate.latitude = hpLat;
        homePoint.coordinate.longitude = hpLng;
    }
}
