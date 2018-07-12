import QtQuick 2.5
import QtLocation 5.6

MapQuickItem {
    id: homePoint

    anchorPoint.x: image.width / 2
    anchorPoint.y: image.height / 2

    sourceItem: Image {
        id: image
        height: 15
        width: 15
        source: "resources/home.png"
        opacity: 0.9
    }
}

