OUT := chess
ZIP_OUT := chess.zip

CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic -Iinclude
LDFLAGS :=

ifeq (Windows_NT, $(OS))

RM := del
SRCS := Game.cpp main.cpp Player.cpp pieces/Piece.cpp pieces/Bishop.cpp pieces/King.cpp pieces/Knight.cpp pieces/Pawn.cpp pieces/Queen.cpp pieces/Rook.cpp
SUBMISSION := Game.cpp main.cpp Player.cpp pieces/Piece.cpp pieces/Bishop.cpp pieces/King.cpp pieces/Knight.cpp pieces/Pawn.cpp pieces/Queen.cpp pieces/Rook.cpp Game.h Player.h pieces/Piece.h pieces/Bishop.h pieces/King.h pieces/Knight.h pieces/Pawn.h pieces/Queen.h pieces/Rook.h
ZIP := zip.exe

else

RM := rm -f
SRCS := Game.cpp main.cpp Player.cpp pieces/Piece.cpp pieces/Bishop.cpp pieces/King.cpp pieces/Knight.cpp pieces/Pawn.cpp pieces/Queen.cpp pieces/Rook.cpp
SUBMISSION := Game.cpp main.cpp Player.cpp pieces/Piece.cpp pieces/Bishop.cpp pieces/King.cpp pieces/Knight.cpp pieces/Pawn.cpp pieces/Queen.cpp pieces/Rook.cpp Game.h Player.h pieces/Piece.h pieces/Bishop.h pieces/King.h pieces/Knight.h pieces/Pawn.h pieces/Queen.h pieces/Rook.h
ZIP := zip

endif

OBJS := $(patsubst %.cpp,%.o,$(SRCS))

.PHONY: all
all: $(OUT)


$(OUT): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(ZIP_OUT): $(SUBMISSION)
	$(RM) $(ZIP_OUT)
	$(ZIP) $@ $e

.PHONY: clean
clean:
	$(RM) $(OUT) $(OBJS) $(ZIP_OUT)
