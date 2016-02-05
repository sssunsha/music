import QtQuick 2.0
import QtQuick.Particles 2.0
import AudioPlayer 1.0

Rectangle {
    width: 824
    height: 404
    color: "black"
    ParticleSystem {
        anchors.fill: parent
        id: syssy
        //! [0]
        ParticleGroup {
            name: "fire"
            duration: 2000
            durationVariation: 2000
            to: {"splode":1}
        }
        //! [0]
        //! [1]
        ParticleGroup {
            name: "splode"
            duration: 400
            to: {"dead":1}
            TrailEmitter {
                group: "works"
                emitRatePerParticle: 100
                lifeSpan: 1000
                maximumEmitted: 1200
                size: 8
                velocity: AngleDirection {angle: 270; angleVariation: 45; magnitude: 20; magnitudeVariation: 20;}
                acceleration: PointDirection {y:100; yVariation: 20}
            }
        }
        //! [1]
        //! [2]
        ParticleGroup {
            name: "dead"
            duration: 1000
            Affector {
                once: true
                onAffected: worksEmitter.burst(400,x,y)
            }
        }
        //! [2]

        Timer {
            interval: 1000
            running: true
            triggeredOnStart: true
            repeat: true
            onTriggered:startingEmitter.pulse(100);
        }
        Emitter {
            id: startingEmitter
            group: "fire"
            width: parent.width
            y: parent.height
            enabled: false
            emitRate: 80
            lifeSpan: 6000
            velocity: PointDirection {y:-100;}
            size: 32
        }

        Emitter {
            id: worksEmitter
            group: "works"
            enabled: false
            emitRate: 100
            lifeSpan: 1600
            maximumEmitted: 6400
            size: 8
            velocity: CumulativeDirection {
                PointDirection {y:-100}
                AngleDirection {angleVariation: 360; magnitudeVariation: 80;}
            }
            acceleration: PointDirection {y:100; yVariation: 20}
        }

        ImageParticle {
            groups: ["works", "fire", "splode"]
            source: "qrc:/../glowdot.png"
            alpha: 0
            colorVariation: 0.9
            entryEffect: ImageParticle.Scale
        }
    }
}

