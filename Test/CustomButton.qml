import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

Button {
    id: customButton
    width: 140
    height: 50
    font.bold: true
    focusPolicy: Qt.WheelFocus

    contentItem: Text {
            text: customButton.text
            font.family: "Arial"
            font.pixelSize: 18;
            font.bold: true
            opacity: enabled ? 1.0 : 0.3
            color:  "#ffffff"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

    background: Rectangle{
        anchors.fill: parent
        radius: 15
        property var normalColor: "#00A6E4"
        property var hoveredColor: "#07B3F2"
        property var pressedColor: "#00A6E4"
        color: customButton.pressed ? pressedColor :
               customButton.hovered ? hoveredColor :
                                    normalColor
        border.width: 1.5
        border.color:"#ffffff"
    }
}
