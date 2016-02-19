import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Particles 2.0
import QtQuick.Window 2.2
import AudioPlayer 1.0
import QtQuick.Dialogs 1.2
import AudioPlayer 1.0
import "qrc:Constant.js" as Constant

Rectangle {
    id: root
    color: "#1f1f1f"

    width: Constant.visulation_width
    height: Constant.visulation_height



    ParticleSystem {
        id: particleSystem
    }
    Repeater{
        id: repeater
        model: 25
        Emitter {
            id: emitter
            enabled: true
            x: parent.width/2
            y: parent.height/2
            width: 1; height: 1;
            system: particleSystem
            emitRate: 100
            lifeSpan: Constant.life_span_effect1;
            size: 32
            endSize: 50
            velocity: AngleDirection {
                id: velocity
                angle: index * 14.4
                angleVariation: 15
                magnitude: 600
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
                repeater.itemAt(i).emitRate = barData[i];
            }
        }
    }

}
