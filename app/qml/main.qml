import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

import AudioPlayer 1.0
import "qrc:/../EffectList.js" as EffectList

ApplicationWindow {
    visible: true
    width: 840
    height: 480
    title: qsTr("Music")

    MainForm {
        anchors.fill: parent
        button1.onClicked: AudioPlayer.startPlayback()
        button2.onClicked: {
            AudioPlayer.suspend()
            barArea.cleanBar()
        }
        button3.onClicked: fileSelector.visible = true
        buttonPre.onClicked: {

            if(stackView.depth > 1) {
                stackView.pop();
                EffectList.index -=1;
            } else if (stackView.depth == 1) {
                stackView.clear();
                EffectList.index = 0;
            }
        }
        buttonNext.onClicked: {
            if (EffectList.effectArray.length > EffectList.index) {
                stackView.push(EffectList.effectArray[EffectList.index]);
                EffectList.index +=1;
            }
        }
    }


    FileDialog {
        id:fileSelector
        title: qsTr("选择一个音乐文件")
        selectMultiple: true;
        //        nameFilters: [  qsTr("*.wav") ]
        onAccepted: {
            var path = fileSelector.fileUrl.toString();
            // remove prefixed "file:///"
            path = path.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,"");
            // unescape html codes like '%23' for '#'
            console.log(path)
            AudioPlayer.loadFile(path)
        }
    }

}
