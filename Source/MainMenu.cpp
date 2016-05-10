#include <SDL2/SDL.h>
#include "../Headers/Global.h"
#include <iostream>
#include "../Headers/States.h"
#include <vector>
#include <string>


std::vector<TX_STACK *> Tx_Stack;
std::vector<TX_STACK *> Text_Stack;
std::vector<B_STACK *> Button_Stack;
std::vector<void(*)(void)> Funct_Stack;
std::vector<Key *> Key_Stack;

extern std::string Returntext;


TX_STACK *MM_Stack;
TX_STACK *MM_TStack;

B_STACK *MM_BStack;

extern int RES_HEIGHT;
extern int RES_WIDTH;
bool Quit = false;
extern bool TI_FLAG;

SDL_Window *win;
SDL_Texture *wall;
SDL_Renderer *ren;

SDL_Texture *bg_MainMenu = nullptr;

SDL_Texture *MM_Btx[4];

Texture* wally;

bool Init_Game();
void B_OptionsMenu(void);
void B_LevelEdit(void);
void B_Level_Menu(void);

int RefreshRate = 60;



int GameLoop()
{

    Init_Game();

    SDL_Event e;

    MM_Load();

    while(!Quit)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_TEXTINPUT && TI_FLAG == true)
            {
                Returntext.push_back(*e.text.text);
                TI_Update_Text();
                break;
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                double x = e.button.x;
                double y = e.button.y;
                double ax = (x / RES_WIDTH) * 1920;
                double ay = (y / RES_HEIGHT) * 1080;

                for(int y = Button_Stack.size() - 1; y >= 0; y--)
                {
                    for(int x = Button_Stack[y]->size() - 1; x >= 0; x--)
                    {
                        if(ax > Button_Stack.at(y)->at(x)->x && ax < Button_Stack.at(y)->at(x)->x + Button_Stack.at(y)->at(x)->w)
                        {
                            if(ay > Button_Stack.at(y)->at(x)->y && ay < Button_Stack.at(y)->at(x)->y + Button_Stack.at(y)->at(x)->h)
                            {
                                if(Button_Stack.at(y)->at(x)->type == BASIC)
                                {
                                    Button_Stack.at(y)->at(x)->funct();
                                    x = 0;
                                    y = 0;
                                }
                                else if(Button_Stack.at(y)->at(x)->type == LIST)
                                {
                                    Button_Stack.at(y)->at(x)->funct2(Button_Stack.at(y)->at(x)->L_Pos);
                                    x = 0;
                                    y = 0;
                                }
                                else if(Button_Stack.at(y)->at(x)->type == GRID)
                                {
                                    Button_Stack.at(y)->at(x)->funct3(Button_Stack.at(y)->at(x)->G_PosX, Button_Stack.at(y)->at(x)->G_PosY);
                                    x = 0;
                                    y = 0;
                                }
                            }
                        }
                    }
                }
            }
            else if(e.type == SDL_QUIT)
            {
                Quit = true;
            }
            else if(e.type == SDL_KEYDOWN && Key_Stack.size() > 0)
            {
                for(int x = 0; x < Key_Stack.size(); x++)
                {
                    if(Key_Stack.at(x)->input == e.key.keysym.sym)
                        Key_Stack.at(x)->funct();
                }
            }
        }

        for(int x = 0; x < Tx_Stack.size(); x++)
        {
            for(int y = 0; y < Tx_Stack[x]->size(); y++)
            {
                RenderTexture(Tx_Stack[x]->at(y));
            }
        }

        RenderTexture(wally);

        for(int x = 0; x < Text_Stack.size(); x++)
        {
            for(int y = 0; y < Text_Stack[x]->size(); y++)
            {
                RenderTexture(Text_Stack[x]->at(y));
            }
        }

        for(int x = 0; x < Funct_Stack.size(); x++)
        {
            Funct_Stack[x]();
        }

        SDL_RenderPresent(ren);
        SDL_RenderClear(ren);

        SDL_Delay(16);
    }


    App_Quit();


    return 0;
}


bool Init_Game()
{
    win = SDL_CreateWindow("Iron Monkey: Turnz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RES_WIDTH, RES_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == nullptr)
    {
        LogSDLError("CreateWindow");
        return false;
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr)
    {
        LogSDLError("CreateRenderer");
        return false;
    }

    wall = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1920, 1080);
    if (wall == nullptr)
    {
        LogSDLError("Create rendering wall");
        return false;
    }

    wally = CreateTexture(wall);

    return true;
}


void MM_Load(void)
{
    MM_Stack = new TX_STACK;
    MM_TStack = new TX_STACK;
    MM_BStack = new B_STACK;


    bg_MainMenu = LoadTexture("bg_MainMenu.png", "Main Menu: Background");

    MM_Btx[0] = LoadText("OPTIONS", 26, "Main Menu: OPTIONS text");
    MM_Btx[1] = LoadText("EDITOR", 26, "Main Menu: EDITOR text");
    MM_Btx[2] = LoadText("PLAY", 26, "Main Menu: PLAY text");


    Stack_Texture(MM_Stack, bg_MainMenu, 0, 0);

    Stack_Text(MM_TStack, MM_Btx[2], 970, 565); //LEVEL MENU
    Stack_Text(MM_TStack, MM_Btx[1], 970, 725); //LEVEL EDIT
    Stack_Text(MM_TStack, MM_Btx[0], 970, 895); //OPTIONS BUTTON

    Stack_Button(MM_BStack, 620, 500, 700, 130, B_Level_Menu); //LEVEL MENU
    Stack_Button(MM_BStack, 620, 660, 700, 130, B_LevelEdit); //LEVEL EDIT
    Stack_Button(MM_BStack, 620, 830, 700, 130, B_OptionsMenu); //OPTIONS BUTTON

    Stack_Stack(MM_Stack);
    Stack_TStack(MM_TStack);
    Stack_BStack(MM_BStack);

    Exit_Sequence_Add(MM_Unload);

}


void MM_Unload(void)
{
    RemoveTexture(bg_MainMenu, "Main Menu: Background");

    RemoveTexture(MM_Btx[0], "Main Menu: OPTIONS text"); //Button 1
    RemoveTexture(MM_Btx[1], "Main Menu: EDITOR text"); //Button 2
    RemoveTexture(MM_Btx[2], "Main Menu: PLAY text"); //Button 3


    Clear_Tx_Stack(MM_Stack);
    Clear_Tx_Stack(MM_TStack);
    Clear_BStack(MM_BStack);

    Unstack_Stack(MM_Stack);
    Unstack_TStack(MM_TStack);
    Unstack_BStack(MM_BStack);


    Exit_Sequence_Remove(MM_Unload);
}


void B_OptionsMenu(void)
{
    MM_Unload();
    Op_Load();
}


void B_LevelEdit(void)
{
    MM_Unload();
    LE_Load();
}


void B_Level_Menu()
{
    MM_Unload();
    LM_Load();
}
