import QtQuick 2.5
import QtLocation 5.6

MapQuickItem {
    id: marker

    anchorPoint.x: image.width / 2
    anchorPoint.y: image.height

    property var hotpath: "resources/marker_hot.png"
    property var waypath: "resources/marker_way.png"
    property var path: "resources/marker_way.png"

    sourceItem: Image {
        id: image
        source: path
        opacity: 0.9
    }
}
