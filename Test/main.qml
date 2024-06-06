import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2

ApplicationWindow {
    visible: true
    visibility: Qt.WindowFullScreen
    title: "Histogram bars"
    color:"#1E1846";

    property int x1 : 20
    property int x2 : 180
    property int x3 : 340
    property int x4 : 500
    property int y1 : 20

    FileDialog{
        id: fileDialog
        onAccepted:{
            var file = fileDialog.fileUrl
            if (file !== "") {
                mainHandler.setFileName(file)
            }
        }
    }

    MessageDialog {
        id: messageDialog
        text: "Необходимо открыть файл!"
        modality: Qt.WindowModal
    }

    GridView {
        id: gridView
        height: 500
        anchors.fill: parent

        CustomButton {
            id: openId
            text: qsTr("Открыть")
            x: x1
            y: y1
            onClicked: {
                fileDialog.open();
            }
        }

        CustomButton {
            id: cancelId
            text: qsTr("Отмена")
            x: x2
            y: y1
            onClicked: {
                mainHandler.cancelRead();
            }
        }

        CustomButton {
            id: startId
            text: qsTr("Старт")
            x: x3
            y: y1
            onClicked: {
                mainHandler.startRead();
            }
        }

        Canvas {
            id: progressBar
            anchors.right: parent.right
            anchors.rightMargin: 30
            anchors.left: parent.left
            anchors.leftMargin: 45
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 475

            property var needToRepaint: false
            property var progress: 0
            property var total: 0

            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height)
                if (needToRepaint === true)
                {
                    // Update progress on form
                    ctx.fillStyle = "#312c58";
                    ctx.fillRect(0, 0, histogram.width, histogram.height);
                    ctx.fillStyle = "#7e79a4";
                    ctx.fillRect(0, 0, histogram.width * progress / total, histogram.height);
                }
            }
        }


        Canvas {
            id: histogram
            anchors.right: parent.right
            anchors.rightMargin: 30
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.top: parent.top
            anchors.topMargin: 95
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 75

            property var needToRepaint: false
            property var data
            property var maxValue

            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height)
                if (needToRepaint === true)
                {
                    ctx.fillStyle = "#1E1846"
                    ctx.fillRect(0, 0, width, height);

                    var barWidth = width / (15 * 2)
                    var maxBarHeight = height - 20

                    let i = 0;
                    for (var key in data ) {
                        // Paint bars
                        let value = data[key]
                        let label = key

                        let barHeight = (value / maxValue) * maxBarHeight
                        let x = i * (barWidth * 2) + barWidth/2
                        let y = height - barHeight

                        ctx.fillStyle = "#00A6E4"
                        ctx.fillRect(x, y, barWidth, barHeight - 20)

                        // Add words labels
                        ctx.fillStyle = "white"
                        ctx.font = "16px Arial"
                        var textX = x + barWidth/2 - ctx.measureText(label).width/2
                        ctx.fillText(label, textX, height - 5)
                        ++i;
                    }
                }
            }
        }

    }

    Connections {
        target: mainHandler
        onPaintHist: {
            histogram.needToRepaint = true
            histogram.data = data
            histogram.maxValue = maxValue
            histogram.requestPaint()
        }

        onSetProgress: {
            if (stop === false)
            {
                progressBar.needToRepaint = true
            }
            else
            {
                progressBar.needToRepaint = false
            }

            progressBar.progress = currentProg
            progressBar.total = totalProg
            progressBar.requestPaint()
        }

        onClearHist: {
            progressBar.needToRepaint = false
            progressBar.requestPaint()

            histogram.needToRepaint = false
            histogram.requestPaint()
        }

        onNeedChooseFile: {
            messageDialog.open()
        }
    }

}
