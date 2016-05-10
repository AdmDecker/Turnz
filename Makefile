release:
	@ mingw32-g++ -std=c++11 -IHeaders                \
	-LC:/SDL2_ttf/i686-w64-mingw32/lib           \
	-LC:/SDL2/i686-w64-mingw32/lib    			\
	-lmingw32 -lSDL2 -lSDL2_ttf -mwindows        \
	-I C:\SDL2\i686-w64-mingw32\include\SDL2      \
	-I C:\SDL2_ttf\i686-w64-mingw32\include\SDL2  \
	main.cpp                                \
	Source/Game.cpp Source/GameSettings.cpp Source/Global.cpp Source/Highscores.cpp Source/Level_Edit.cpp Source/Level_Menu.cpp Source/MainMenu.cpp Source/TextInput.cpp   \
	-o bin/Turnz.exe                                \
	
debian:
	@ g++ -std=c++11 -IHeaders \
	  main.cpp -o out/main.o \
	  src/Game.cpp -o out/game.o \
	  src/GameSettings.cpp -o out/GameSettings.o \
	  src/Global.cpp -o out/GameSettings.o \
	  src/Highscores.cpp -o out/Highscores.o \
	  src/Level_Edit.cpp -o out/Level_Edit.o \
	  src/Level_Menu.cpp -o out/Level_Menu.o \
	  src/MainMenu.cpp -o out/MainMenu.o \
	  src/TextInput.cpp -o out/TextInput.o \
	  -lSDL2