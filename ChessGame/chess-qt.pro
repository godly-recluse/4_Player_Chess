QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Game.cpp \
    Player.cpp \
    chessclientobj.cpp \
    chesswidget.cpp \
    gameoverwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    menuwindow.cpp \
    pieces/Bishop.cpp \
    pieces/King.cpp \
    pieces/Knight.cpp \
    pieces/Pawn.cpp \
    pieces/Piece.cpp \
    pieces/Queen.cpp \
    pieces/Rook.cpp

HEADERS += \
    Game.h \
    Player.h \
    chessclientobj.h \
    chesswidget.h \
    gameoverwindow.h \
    mainwindow.h \
    menuwindow.h \
    pieces/Bishop.h \
    pieces/King.h \
    pieces/Knight.h \
    pieces/Pawn.h \
    pieces/Piece.h \
    pieces/Queen.h \
    pieces/Rook.h

FORMS += \
    gameoverwindow.ui \
    mainwindow.ui \
    menuwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc \
    cmake-build-debug/CMakeFiles/clion-log.txt \
    img/Activated_Square.png \
    img/Black_Possible_Move.png \
    img/White_Possible_Move.png \
    img/White_Square.png \
    img/Black_Square.png \
    img/Player0/bishop.png \
    img/Player0/king.png \
    img/Player0/knight.png \
    img/Player0/pawn.png \
    img/Player0/queen.png \
    img/Player0/rook.png \
    img/Player1/bishop.png \
    img/Player1/king.png \
    img/Player1/knight.png \
    img/Player1/pawn.png \
    img/Player1/queen.png \
    img/Player1/rook.png \
    img/Player2/bishop.png \
    img/Player2/king.png \
    img/Player2/knight.png \
    img/Player2/pawn.png \
    img/Player2/queen.png \
    img/Player2/rook.png \
    img/Player3/bishop.png \
    img/Player3/king.png \
    img/Player3/knight.png \
    img/Player3/pawn.png \
    img/Player3/queen.png \
    img/Player3/rook.png \
    img/DeactivatedPlayer/bishop.png \
    img/DeactivatedPlayer/king.png \
    img/DeactivatedPlayer/knight.png \
    img/DeactivatedPlayer/pawn.png \
    img/DeactivatedPlayer/queen.png \
    img/DeactivatedPlayer/rook.png


QT_QUICK_CONTROLS_STYLE = universal ./app
