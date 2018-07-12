import QtQuick 2.5
import QtLocation 5.6

MapQuickItem {
    id: marker

    anchorPoint.x: image.width / 2
    anchorPoint.y: image.height

    sourceItem: Image {
        id: image
        source: "resources/marker.png"
        opacity: 0.9
    }
}
