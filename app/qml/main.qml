import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.4

import AudioPlayer 1.0
import "qrc:/../EffectList.js" as EffectList
import "qrc:Constant.js" as Constant

ApplicationWindow {    
    id:root
    visible: true
    width: Constant.window_width
    height: Constant.window_height
    title: qsTr(Constant.default_title)

    property alias mainWindow: root
    property alias mainForm: mainForm1

    MainForm1 {
        id:mainForm1
        anchors.fill: parent

        buttonPlay.onClicked:{
            AudioPlayer.startPlayback()
        }

        buttonPause.onClicked: {
            AudioPlayer.suspend()
        }

        buttonFileSelector.onClicked: {

            fileSelector.visible = true
        }

        function handleEffectChoosing(effectIndex ){
            console.log(effectIndex + " effect is chosen ...")
            stackView.clear();
            switch(effectIndex)
            {
            case 0:
                // show the default effect, so do nothing
                break;
            case 1:
            case 2:
            case 3:
                stackView.push(EffectList.effectArray[effectIndex -1]);
                break;
            default:
                break;
            }
        }
    }


    FileDialog {
        id:fileSelector
        property var audioFilePath: ""
        title: qsTr("选择一个音乐文件")
        selectMultiple: true;
        nameFilters: [  qsTr("*.wav *.mp3 *.wma *.ape *.aac")]
        onAccepted: {
            var path = fileSelector.fileUrl.toString();
            audioFilePath = path;
            console.log(path)

            AudioPlayer.suspend()
            AudioPlayer.loadFile(path)
        }
    }

    Connections {
        target: AudioPlayer
        onStateChanged:{
//            console.log("AudioPlayer state is " + state);
            switch(state)
            {
            case 0:
                mainForm1.buttonPlay.enabled = false;
                mainForm1.buttonPlay.visible = false;
                mainForm1.buttonPause.enabled = true;
                mainForm1.buttonPause.visible = true;

                // update title to show the state
                root.title = qsTr(Constant.default_title) + qsTr("\tplaying")
                        + "\t" +  fileSelector.audioFilePath;

                break;
            case 2:
                // set progressbar value to 1
                mainForm.progressBar.value = 1;
            case 1:
            case 3:
                //reset the title
                root.title = qsTr(Constant.default_title);
                mainForm1.buttonPlay.enabled = true;
                mainForm1.buttonPlay.visible = true;
                mainForm1.buttonPause.enabled = false;
                mainForm1.buttonPause.visible = false;
                break;
            }
        }
    }
}

