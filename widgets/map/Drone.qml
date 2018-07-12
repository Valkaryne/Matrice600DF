import QtQuick 2.5
import QtLocation 5.6
import QtPositioning 5.6

MapQuickItem {
    id: drone

    anchorPoint.x: image.width / 2
    anchorPoint.y: image.height / 2

    sourceItem: Image {
        id: image
        width: 22
        height: 22
        source: "resources/drone.png"
        opacity: 1.0
    }

    coordinate: QtPositioning.coordinate(53.9, 27.56)
}
