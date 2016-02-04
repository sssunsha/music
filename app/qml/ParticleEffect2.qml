import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Particles 2.0
import QtQuick.Window 2.2
import AudioPlayer 1.0
import QtQuick.Dialogs 1.2
import AudioPlayer 1.0

Rectangle {
    id: root
    color: "#1f1f1f"

    width: 824
    height: 404



    ParticleSystem {
        id: particleSystem
    }
    Row {
        width: parent.width;
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        Repeater{
            id: repeater
            model: 25
            Rectangle {
                width: root.width/25; height: 1;
                Emitter {
                    id: emitter
                    anchors.centerIn: parent
                    enabled: true
                    width: 1; height: 1;
                    anchors.bottomMargin: 1
                    system: particleSystem
                    emitRate: 100
                    lifeSpan: 700
                    size: 32
                    endSize: 32
                    velocity: AngleDirection {
                        id: velocity
                        angle: 270
                        magnitude: 600
                    }
                }
            }
        }
    }

    ImageParticle {
        source: "qrc:/../star.png"
        system: particleSystem
        alpha: 0
        colorVariation: 0.9
    }

    Connections {
        target: AudioPlayer
        onBarDataChanged:{
            var length = barData.length;
            for (var i=0;i<length;i++) {
                repeater.itemAt(i).children[0].emitRate = barData[i];
            }
        }
    }

}
