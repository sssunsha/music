﻿import QtQuick 2.0

import AudioPlayer 1.0
import "qrc:Constant.js" as Constant


Rectangle {
    id:root
//    width: 200
//    height: 100
    width: Constant.visulation_width
    height: Constant.visulation_height
    color: "black";

    property var barObject: [];
    property var component: Qt.createComponent("BarItem.qml");
//    property real spacing: 5;
//    property real barWidth: 20;
//    property real barInteral: spacing+barWidth;
//    property real barCount: 25;
    property real spacing: Constant.bar_spacing;
    property real barWidth: Constant.bar_width;
    property real barInteral: Constant.bar_interal;
    property real barCount: Constant.bar_count;

    property bool initialized: false;


    function resetBarArea(){
        if(barObject.length <= 0)
            return;
        for(var i in barObject){
            barObject[i].destoryBarItem();
        }
        barObject.length = 0;
    }

    function cleanBar() {
        for(var i = 0; i < barCount; i++){
            barObject[i].barHeight = 0;
        }
    }

    function createBarArea(){
        console.log("barCount:", barCount);
        barCount = Math.round(barCount);
        for(var i = 0; i < barCount; i++){
            var object = component.createObject(root,
                                                {
                                                    "x": i*barInteral,
                                                    "width":root.barWidth,
                                                    "height":root.height,
                                                    "anchors.bottom":root.bottom,
                                                    "barHeight":0,
                                                });
            barObject.push(object);
        }
    }

    function secondsToHumanString(secondsInreal){
        var seconds = parseInt(secondsInreal, 10);
        var minutes = parseInt(seconds / 60);
        var hours   = parseInt(minutes / 60);

        minutes %= 60;
        seconds %= 60;

        var result = "";
        result += hours > 0 ? hours.toString() + ":" : "";

        if (minutes > 9){
            result += minutes.toString();
        }
        else if (minutes >= 0){
            result += ("0" + minutes.toString())
        }
        result += ":";

        if (seconds > 9){
            result += seconds.toString();
        }
        else if (seconds >= 0){
            result += ("0" + seconds.toString())
        }

        return result;
    }

    Connections {
        target: AudioPlayer
        onBarDataChanged:{
            console.log("AudioPlayer.onBarDataChanged: enter");

            if(!initialized) {
                createBarArea();
                initialized = true;
            }

            for(var i = 0; i < barCount; i++){
                barObject[i].barHeight = barData[i];
            }

            console.log("AudioPlayer.onBarDataChanged: exit");
        }

        onProgressChanged:{
            console.log("AudioPlayer.onProgressChanged: enter");
            progressBar.value = position;
            console.log("AudioPlayer.onProgressChanged: exit");
        }

        onElapsedTimeChanged:{
            console.log("AudioPlayer.onElapsedTimeChanged: enter");

            var curr = secondsToHumanString(current);
            var len  = secondsToHumanString(duration);
            elapsedTime.text = curr + " / " + len;

            console.log("AudioPlayer.onElapsedTimeChanged: exit");
        }

    }

}

