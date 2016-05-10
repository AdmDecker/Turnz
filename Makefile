release:
	@ mingw32-g++ -std=c++11 -IHeaders                \
	-LC:/SDL2_ttf/i686-w64-mingw32/lib           \
	-LC:/SDL2/i686-w64-mingw32/lib    			\
	-lmingw32 -lSDL2 -lSDL2_ttf -mwindows        \
	-I C:\SDL2\i686-w64-mingw32\include\SDL2      \
	-I C:\SDL2_ttf\i686-w64-mingw32\include\SDL2  \
	-static main.cpp                                \
	Source/Game.cpp Source/GameSettings.cpp Source/Global.cpp Source/Highscores.cpp Source/Level_Edit.cpp Source/Level_Menu.cpp Source/MainMenu.cpp Source/TextInput.cpp   \
	-o bin/Turnz.exe                                \
	