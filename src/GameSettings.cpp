#include <fstream>
#include <string.h>
#include <iostream>
#include "../Headers/Global.h"
#include "SDL2/SDL.h"
#include "../Headers/States.h"
#include <vector>
#include <sstream>

int RES_HEIGHT;
int RES_WIDTH;

extern bool QuitAll;

TX_STACK *Op_Stack;
TX_STACK *Op_TStack;

B_STACK *Op_BStack;

extern SDL_Window *win;
extern SDL_Renderer *ren;
extern SDL_Texture *wall;
extern std::vector<void (*)(void)> Exit_Sequence;

SDL_Texture *bg_Options;
SDL_Texture *T_Resolution = nullptr;
SDL_Texture *arrow = nullptr;
SDL_Texture *button2 = nullptr;
SDL_Texture *restext = nullptr;

std::string Resolutions[] = {"640x360", "640x480", "1024x576", "1280x1024", "1366x768", "1600x900", "1920x1080"};
int Sel_Resolution;

void Write_Settings(void);
void Op_CheckButton(double x, double y);
void Op_Render(void);
void Update_restext(void);
void decResolution(void);
void incResolution(void);
void B_Op_Back(void);

SDL_Texture *GS_Words[2];



int read_Settings(void)
{
    std::ifstream inFile("Options.txt");
    if(!inFile.is_open())
        LogSDLError("Error loading Options.txt");

    char input[80];

    while(inFile.getline(input, 80))
    {
        if(strstr(input, "Resolution: "))
        {


            for(int i = 0; i < 80; i++)
                input[i] = 'p';
            inFile.getline(input, 80);

            for(int x = 0; x < sizeof Resolutions / sizeof Resolutions[0]; x++)
            {
                if(strcmp(Resolutions[x].c_str(), input) == 0)
                {
                    Sel_Resolution = x;
                    break;
                }
            }

            std::vector<int> prcing;
            int z;
            for(z = 0; z < 80 && input[z] != 'x'; z++)
            {
                prcing.push_back(input[z] - '0');
            }

            std::stringstream swag;

            for(int x = 0; x < prcing.size(); x++)
                swag << prcing[x];

            swag >> RES_WIDTH;

            prcing.clear();
            std::stringstream swag2;

            for(++z; z < 80 && input[z] != 'p' && input[z] != '\n'; z++)
                prcing.push_back(input[z] - '0');

            for(int x = 0; x < prcing.size(); x++)
                swag2 << prcing[x];
            swag2 >> RES_HEIGHT;
        }

    }
    return 0;
}

void Op_Load()
{
    Op_Stack = new TX_STACK;
    Op_TStack = new TX_STACK;
    Op_BStack = new B_STACK;

    T_Resolution = LoadText("Resolution", 26, "Options: Resolution");
    bg_Options = LoadTexture("OPTIONS.png", "Options Background");

    GS_Words[0] = LoadText("BACK", 26, "GS: Text");
    GS_Words[1] = LoadText("SAVE", 26, "GS: Text");


    restext = LoadText(Resolutions[Sel_Resolution], 26, "Options: Resolutions");

    Stack_Button(Op_BStack, 360, 345, 145, 120, decResolution); //Left Arrow
    Stack_Button(Op_BStack, 1400, 345, 145, 120, incResolution); //Right Arrow
    Stack_Button(Op_BStack, 1075, 930, 280, 105, Write_Settings); //Save Button
    Stack_Button(Op_BStack, 548, 926, 280, 105, B_Op_Back); //Back Button

    Stack_Texture(Op_Stack, bg_Options, NULL, NULL); //Background

    Stack_Text(Op_TStack, T_Resolution, 950, 300);

    Stack_Text(Op_TStack, restext, 950, 400);

    Stack_Text(Op_TStack, GS_Words[0], 688, 980);
    Stack_Text(Op_TStack, GS_Words[1], 1215, 980);


    Stack_Stack(Op_Stack);
    Stack_TStack(Op_TStack);
    Stack_BStack(Op_BStack);

    Exit_Sequence.push_back(Op_Unload);

    return;
}


void Write_Settings(void)
{
    std::ofstream outFile("Options.txt");
    outFile << "Resolution: \n";
    outFile << Resolutions[Sel_Resolution] << std::endl;
}


void Op_Unload(void)
{
    RemoveTexture(T_Resolution, "Options: Resolution");
    RemoveTexture(bg_Options, "Options Background");
    RemoveTexture(restext, "Options: Resolutions");

    RemoveTexture(GS_Words[0], "GS: Text");
    RemoveTexture(GS_Words[1], "GS: Text");


    Exit_Sequence.erase(Exit_Sequence.end() - 1);

    Clear_BStack(Op_BStack);
    Clear_Tx_Stack(Op_Stack);
    Clear_Tx_Stack(Op_TStack);

    Unstack_Stack(Op_Stack);
    Unstack_BStack(Op_BStack);
    Unstack_TStack(Op_TStack);
    return;
}


void incResolution()
{
    if(Sel_Resolution < (sizeof(Resolutions)/sizeof(*Resolutions)) - 1)
    {
        Sel_Resolution++;
        Update_restext();
    }
    return;
}


void decResolution()
{
    if(Sel_Resolution > 0)
    {
        Sel_Resolution--;
        Update_restext();
    }
    return;
}


void Update_restext(void)
{
    RemoveTexture(restext, "Options: Resolutions");
    Unstack_Texture(Op_TStack, restext);
    restext = LoadText(Resolutions[Sel_Resolution], 26, "Options: Resolutions");
    Stack_Text(Op_TStack, restext, 950, 400);
}


void B_Op_Back(void)
{
    Op_Unload();
    MM_Load();
}
