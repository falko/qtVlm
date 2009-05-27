CONFIG += qt
TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += bzip2 zlib-1.2.3 qextserialport
LIBS += -Lbzip2 -lbz2 -Lzlib-1.2.3 -lz -Lqextserialport/build -lqextserialport
MOC_DIR = objs
OBJECTS_DIR = objs
SOURCES_DIR = src
DESTDIR = ../

QT += network xml

CODECFORTR = UTF-8
CODECFORSRC = UTF-8
TRANSLATIONS = ../tr/qtVlm_en.ts

RC_FILE = qtVlm.rc

HEADERS += \
           BoardVLM.h \
           BoardVLM_tools.h \
           boatAccount_dialog.h \
           boatAccount.h \
           DialogGraphicsParams.h \
           DialogLoadGrib.h \
           DialogProxy.h \
           DialogServerStatus.h \
           DialogUnits.h \
           GshhsRangsReader.h \
           GshhsReader.h \
           GisReader.h \
           Grib.h \
           GribRecord.h \
           inetConnexion.h \
           LoadGribFile.h \
           MainWindow.h \
           MenuBar.h \
           Orthodromie.h \
           opponentBoat.h \
           paramVLM.h \
           Pilototo.h \
           Pilototo_param.h \
           POI.h \
           POI_editor.h \
           POI_input.h \
           Polar.h \
           Projection.h \
           race_dialog.h \
           sha1/sha1.h \
           Terrain.h \
           Util.h \
           Version.h \
           xmlBoatData.h \
           xmlPOIData.h \
           zuFile.h

FORMS += boatAccount_dialog.ui BoardVLM.ui \
            paramVLM.ui POI_input.ui POI_editor.ui Pilototo_param.ui instructions.ui \
            Pilototo.ui race_dialog.ui WP_dialog.ui
SOURCES += \
           BoardVLM.cpp \
           boatAccount_dialog.cpp \
           boatAccount.cpp \
           DialogGraphicsParams.cpp \
           DialogLoadGrib.cpp \
           DialogProxy.cpp \
           DialogServerStatus.cpp \
           DialogUnits.cpp \
           GshhsRangsReader.cpp \
           GshhsReader.cpp \
           GisReader.cpp \
           Grib.cpp \
           GribRecord.cpp \
           inetConnexion.cpp \
           LoadGribFile.cpp \
           main.cpp \
           MainWindow.cpp \
           MenuBar.cpp \
           Orthodromie.cpp \
           opponentBoat.cpp \
           paramVLM.cpp \
           Pilototo.cpp \
           Pilototo_param.cpp \
           POI.cpp \
           POI_editor.cpp \
           POI_input.cpp \
           Polar.cpp \
           Projection.cpp \
           race_dialog.cpp \
           sha1/sha1.cpp \
           Terrain.cpp \
           Util.cpp \
           xmlBoatData.cpp \
           xmlPOIData.cpp \
           zuFile.cpp

DEFINES = QT_$$[QT_VERSION]

DEFINES ~= s/\./_
DEFINES ~= s/-.*/

DEFINES ~= s/QT_4_5_[01]/QT_4_5

unix:DEFINES   += _TTY_POSIX_
win32:DEFINES  += _TTY_WIN_ QWT_DLL QT_DLL

