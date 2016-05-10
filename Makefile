
CC=g++
CFLAGS= -std=c++11 -c -Wall -IHeaders
LDFLAGS= -lSDL2
SOURCES= src/Game.cpp src/GameSettings.cpp src/Global.cpp src/Highscores.cpp src/Level_Edit.cpp src/Level_Menu.cpp src/MainMenu.cpp src/TextInput.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hello

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@