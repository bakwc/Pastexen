import QtQuick 2.0
import QtQml 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.1

Window {
    id: notifyWindow
    title: "Pastexen notify"
    width:  260
    height: 160

//    x: Screen.width - width - 30;
    y: 50;

    color: "lightgrey"

    flags: Qt.Tool | Qt.FramelessWindowHint
//    opacity: 0.9

    property int timeoutInterval: 2000
    property int animateDuration: 500

    property int animXStart: Screen.width
    property int animXEnd: Screen.width - width - 70

    Timer {
        id: timerId
        interval: timeoutInterval; running: false; repeat: false
        onTriggered: {
            outOpacityAnimation.start()
            outAnimation.start()
        }
    }

    onVisibleChanged: if (visible == true) {
                          inOpacityAnimation.start()
                          inAnimation.start()
//                          timerId.start();
                      }

    NumberAnimation {
        id: inOpacityAnimation
        target: notifyWindow
        property: "opacity"
        easing.type: Easing.InCubic
        from: 0
        to: 0.9
        duration: animateDuration
        alwaysRunToEnd: true
        onStopped: timerId.start()
    }

    NumberAnimation {
        id: outOpacityAnimation
        target: notifyWindow
        property: "opacity"
        easing.type: Easing.OutCubic
        from: 0.9
        to: 0
        duration: animateDuration
        alwaysRunToEnd: true
        onStopped: notifyWindow.hide()
    }

    NumberAnimation {
        id: inAnimation
        target: notifyWindow
        property: "x"
        easing.type: Easing.OutCubic
        from: animXStart
        to: animXEnd
        duration: animateDuration
    }

    NumberAnimation {
        id: outAnimation
        target: notifyWindow
        property: "x"
        easing.type: Easing.InCubic
        from: animXEnd
        to: animXStart
        duration: animateDuration
    }

}
