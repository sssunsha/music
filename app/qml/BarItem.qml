import QtQuick 2.4

Rectangle {
    id:root
    width: 40
    height: 100
    color: "transparent";
    property alias barHeight: barFg.height;

    function destoryBarItem(){
        root.destroy();
    }

    Rectangle{
        id:barFg;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        height: 50;
        color:"transparent";
        clip: true;

        Rectangle{
            id:barBgColor
            anchors.left: parent.left;
            anchors.right: parent.right;
            anchors.bottom: parent.bottom;
            height: root.height;
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: "#db0707";
                }
                GradientStop {
                    position: 1.00;
                    color: "#06be3a";
                }
            }
        }
    }
}
