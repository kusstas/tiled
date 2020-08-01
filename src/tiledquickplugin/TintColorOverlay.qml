import QtQuick 2.12
import QtGraphicalEffects 1.12

Item {
    property var tiledLayer: null

    anchors.fill: tiledLayer

    ColorOverlay {
        id: colorOverlay

        anchors.fill: parent
        source: tiledLayer
        color: tiledLayer.tintColor
    }

    Blend {
        id: blend

        anchors.fill: parent
        mode: "multiply"
        source: tiledLayer
        foregroundSource: colorOverlay
    }
}
