TEMPLATE =  app
QT +=       qml widgets core quick quickcontrols2 network multimedia
TARGET =    BJOrder
CONFIG +=   c++11 qzxing_qml qzxing_multimedia
CONFIG -=   bitcode
DEFINES +=  QT_DEPRECATED_WARNINGS

HEADERS +=  ../flaticon/flaticon.h \
            ../../commons/config.h \
            ../../commons/comm.machine.pb.h \
            ../../commons/comm.pb.h \
            ../../commons/data.bj.pb.h \
            ../core/component.h \
            ../core/creator.h \
            ../core/define.h \
            ../core/engine.h \
            ../core/model.h \
            ../core/operation.h \
            ../core/utils.h \
            ../core/views.h \
            ../core/file/fileManager.h \
            ../core/file/fileOperations.h \
            ../core/tcp/tcpController.h \
            ../core/tcp/tcpOperation.h \
            ../../raspberry/ann/neuronnetwork.h \
            ../mobile/models.h \
            ../mobile/manager.h \
            ../mobile/operation/operations.h

SOURCES +=  main.cpp \
            ../flaticon/flaticon.cpp \
            ../../commons/comm.pb.cc \
            ../../commons/data.bj.pb.cc \
            ../../commons/comm.machine.pb.cc \
            ../core/engine.cpp \
            ../core/views.cpp \
            ../core/tcp/tcpController.cpp \
            ../core/file/downloadFile.cpp \
            ../core/tcp/tcpOperation.cpp \
            ../../raspberry/ann/neuronnetwork.cpp \
            ../mobile/operation/checkOrder.cpp \
            ../mobile/operation/login.cpp \
            ../mobile/operation/makeDrink.cpp \
            ../mobile/operation/orderStateChanged.cpp \
            ../mobile/models.cpp \
            ../mobile/manager.cpp

RESOURCES += ../flaticon/font.qrc \
            qml/qml.qrc \
            qml/images.qrc

INCLUDEPATH +=  .. \
            ../core \
            ../../commons \
            ../flaticon \
            ../mobile \
            ../../raspberry/ann

OTHER_FILES += $$PWD/ios/Info.plist

include(../include/QZXing/QZXing.pri)

android {
    INCLUDEPATH +=  $$PWD/../include
    QT += androidextras
    contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
        message("****************** android armeabi-v7a*******************")
        LIBS += -L$$PWD/../libs/android/armeabi-v7a -lprotobuf -lleveldb
    }
    contains(ANDROID_TARGET_ARCH,arm64-v8a) {
        message("****************** android arm64-v8a*******************")
        LIBS += -L$$PWD/../libs/android/arm64-v8a -lprotobuf -lleveldb
    }
    LIBS += -lstdc++ -lc -lgcc -llog -landroid -lOpenSLES -lEGL
}

macx:!iphoneos {
    message("****************** macx *******************")
    INCLUDEPATH +=  /usr/local/include
    LIBS += -L/usr/local/lib -lprotobuf -lleveldb
}

unix:!android:!macx:!iphoneos {
    message("****************** unix *******************")
    INCLUDEPATH +=  /usr/local/include
    LIBS += -L$$PWD/../libs/linux -lprotobuf -lleveldb
    LIBS += -lstdc++  -lm -lrt -lpthread
}

iphoneos {
    INCLUDEPATH +=  $$PWD/../include
        message("****************** iphone arm64*******************")
        LIBS += -L$$PWD/../libs/ios/arm64 -lprotobuf -lleveldb
    LIBS += -L$$PWD/../libs/ios -lprotobuf -lleveldb
    LIBS += -framework PushKit
    LIBS += -lstdc++ -lm -lpthread \
            -framework CoreAudio -framework CoreFoundation -framework AudioToolbox \
            -framework CFNetwork -framework UIKit -framework UIKit \
            -framework OpenGLES -framework AVFoundation -framework CoreGraphics \
            -framework QuartzCore -framework CoreVideo -framework CoreMedia -framework PushKit
    ios_icon.files = $$files($$PWD/ios/AppIcon*.png)
    QMAKE_BUNDLE_DATA += ios_icon
    QMAKE_INFO_PLIST = $$PWD/ios/Info.plist
}

target.path = ../../../..
INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    qml/home/HomeView.qml \

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

