//
//  Global.h
//  TEst2
//
//  Created by Adam Decker on 12/17/13.
//  Copyright (c) 2013 Adam Decker. All rights reserved.
//

#ifndef TEst2_Global_h
#define TEst2_Global_h
#include <string>
#include <SDL.h>
#include <vector>

#define BASIC 0
#define LIST 1
#define GRID 2
#define TX_STACK std::vector<Texture *>
#define B_STACK std::vector<button *>

#define EDIT 0
#define PLAY 1

SDL_Texture* LoadTexture(std::string file, std::string name);
SDL_Texture* LoadText(std::string words, int font_size, std::string name);


struct Level
{
    std::string name;
    int pos;
    SDL_Texture *t_Name;
};


struct button
{
    int x,y,w,h,type, L_Pos, G_PosX, G_PosY;
    void (*funct)(void);
    void (*funct2)(int);
    void (*funct3)(int, int);
};

struct Texture
{
    int x,y,w,h,rotation = -1;
    SDL_Texture *tx;
    SDL_Texture *target;
    bool text, H_CENTER, V_CENTER;
};

struct Key
{
    SDL_Keycode input;
    void(*funct)(void);
};


void LogSDLError(std::string);
void App_Quit();

void RenderTexture(Texture *);
void RemoveTexture(SDL_Texture*, std::string name);

int read_Settings(void);


void Stack_Button(B_STACK *stacky, int x, int y, int w, int h, void (*funct)(void));
void Stack_Button(B_STACK *stacky, int x, int y, int w, int h, void(*funct)(int), int L_Pos);
void Stack_Button(B_STACK *stacky, int x, int y, int w, int h, void(*funct)(int, int), int G_PosX, int G_PosY);
void Clear_BStack(B_STACK *stacky);
void Unstack_BStack(B_STACK *stacky);
void Stack_BStack(B_STACK *stacky);

void Stack_Texture(TX_STACK *stacky, SDL_Texture *texture, int x = -1, int y = -1, int rotation = 0);
void Stack_TextureEx(TX_STACK *stacky, SDL_Texture *texture, int x, int y, int w, int h, int rotation = 0);
void Stack_Text(TX_STACK *stacky, SDL_Texture *texture, int x, int y, bool H_CENTER = true, bool V_CENTER = true);
void Stack_TextEx(TX_STACK *stacky, SDL_Texture *texture, int x, int y, int w, int h, bool H_CENTER = true, bool V_CENTER = true);
Texture *CreateTexture(SDL_Texture *texture, int x = -1, int y = -1, SDL_Texture *target = NULL, int rotation = 0);
void Unstack_last(TX_STACK *);

void Stack_Function(void(*funct)(void));
void Unstack_Function(void(*funct)(void));

void Stack_Stack(TX_STACK *);
void Stack_TStack(TX_STACK *);
void Unstack_Stack(TX_STACK *);
void Unstack_TStack(TX_STACK *);
void Unstack_Texture(TX_STACK *, SDL_Texture *);
void Clear_Tx_Stack(TX_STACK *);

void Change_Angle(TX_STACK *, SDL_Texture *, int);

void Exit_Sequence_Remove(void(*funct)(void));
void Exit_Sequence_Add(void(*funct)(void));

void ErrorMessage(std::string message, void(*fallback)());

void Backout(void);
void Unstack_Key(SDL_Keycode input);
void Stack_Key(SDL_Keycode input, void(*funct)(void));

void GP_Init(void);

bool Load_Level();
void TI_Update_Text();

#endif // Global_h_



