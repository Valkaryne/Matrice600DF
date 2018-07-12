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
    property variant beams : []

    property real dLng : 0.000008983152841195396 * 1.69;
    property real dLat : 0.000009031055490301464;

    Plugin {
        id: osmPlugin
        name: "osm"
        PluginParameter { name: "osm.mapping.cache.directory"; value: "D:/maposm_cache/" }
    }

    Plugin {
        id: esriPlugin
        name: "esri"
        PluginParameter { name: "esri.mapping.cache.directory"; value: "D:/mapesri_cache/" }
    }

    function initializeMapItems()
    {
        console.log("Cat here")
        map = Qt.createQmlObject('MapComponent {}', mainItem)
        map.plugin = osmPlugin
        marker = Qt.createQmlObject('Marker {}', map)
        drone = Qt.createQmlObject('Drone {}', map)
        homePoint = Qt.createQmlObject('Home {}', map)
        map.addMapItem(marker)
        map.addMapItem(drone)
        map.addMapItem(homePoint)
        marker.z = map.z + 1
        console.log("Map is initilized")
    }

    function updateDroneLocation(lat, lng, heading)
    {
        drone.coordinate.latitude = lat;
        drone.coordinate.longitude = lng;
        drone.rotation = heading;
    }

    function makeBeam(direction)
    {
        var count = beams.length
        var stLat = drone.coordinate.latitude;
        var stLng = drone.coordinate.longitude;
        var x0 = stLat / dLat;
        var y0 = stLng / dLng;
        var angle = direction * Math.PI / 180;
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

        drone = Qt.createQmlObject('Drone {}', map)
        map.addMapItem(drone)

        homePoint = Qt.createQmlObject('Home {}', map)
        map.addMapItem(homePoint)

        console.log("Provider was changed to " + map.plugin.name)
    }

    function updateHomeDirection()
    {
        map.setHomePoint()
    }
}
