//
//  States.h
//  TEst2
//
//  Created by Adam Decker on 12/17/13.
//  Copyright (c) 2013 Adam Decker. All rights reserved.
//

#ifndef TEst2_States_h
#define TEst2_States_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

int GameLoop(void);
bool Options(void);
bool LevelEdit(void);

void Op_Load(void);
void Op_Unload(void);

void MM_Load(void);
void MM_Unload(void);

void LE_Load(void);
void LE_Unload(void);

void GP_Load(int);

void LM_Load(void);

void Display_HighScores(void(*)(void));

void TextInput(void(*funct)(std::string));


#endif
