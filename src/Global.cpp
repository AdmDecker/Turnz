#include "../Headers/Global.h"
#include <fstream>
#include <string>
#include <iostream>
#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
#include "../Headers/States.h"
#include <vector>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <sstream>

void(*func)(void);
void Exit_ErrorMessage();

extern SDL_Window *win;
extern SDL_Renderer *ren;
extern SDL_Texture *wall;

extern int RES_WIDTH;
extern int RES_HEIGHT;
extern std::vector<B_STACK *> Button_Stack;
extern std::vector<TX_STACK *> Tx_Stack;
extern std::vector<TX_STACK *> Text_Stack;
extern std::vector<void(*)(void)> Funct_Stack;
extern std::vector<Key *> Key_Stack;

extern int par;
extern int setmode;
extern std::vector<std::string> HighScorers;
extern std::vector<int> HighScores;
extern std::string Lvl_Bg;
extern std::string Lvl_name;
extern int LevelDatatx[121];
extern int LevelData[121];

std::vector<void (*)(void)> Exit_Sequence;


std::vector<std::string> Mensajes;
std::vector<std::string> LoadedTextures;

int loadmode;

TX_STACK *T_Error;
TX_STACK *Error_Stack;
SDL_Texture *T_message;
SDL_Texture *buttony;
B_STACK *B_Error;

void LogSDLError(std::string Error)
{
    Mensajes.push_back(Error + "  SDL_Error: " + SDL_GetError());
    SDL_ClearError();
    std::cout << "Error Logged: " << Mensajes.back() << "\n";
}


void App_Quit()
{
    for(int x = Exit_Sequence.size() - 1; x >= 0; x--)
    {
        Exit_Sequence[x];
    }

    std::ofstream Papel("Error Log.txt");
    for(int x = 0; x < Mensajes.size(); x++)
        Papel << Mensajes[x] << std::endl;

    Papel << "\nTextures Not Unloaded:\n";

    for(int x = 0; x < LoadedTextures.size(); x++)
        Papel << LoadedTextures[x] << std::endl;

    Papel << "End Report";
    Papel.close();

    SDL_DestroyTexture(wall);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return;
}


void RenderTexture(Texture *tx)
{
    if(!tx->text) //If texture is not supposed to be rendered as text
    {
        if (nullptr == tx->target)
        {
            SDL_SetRenderTarget(ren, NULL);
        } else
        {
            if(SDL_SetRenderTarget(ren, tx->target) != 0)
                LogSDLError("Can't Set Render Target");
        }

        int w, h;
        if(tx->w != -1 && tx->h != -1)
        {
            w = tx->w;
            h = tx->h;
        }else
        {
            SDL_QueryTexture(tx->tx, NULL, NULL, &w, &h);
        }

        if(tx->x != -1 && tx->y != -1)
        {
            SDL_Rect tempRect;
            tempRect.x = tx->x;
            tempRect.y = tx->y;
            tempRect.h = h;
            tempRect.w = w;

            SDL_RenderCopyEx(ren, tx->tx, NULL, &tempRect, tx->rotation, NULL, SDL_FLIP_NONE);
        }
        else
        {
            SDL_RenderCopyEx(ren, tx->tx, NULL, NULL, tx->rotation, NULL, SDL_FLIP_NONE);
        }
    }
    else //if texture is supposed to be rendered as text
    {
        SDL_SetRenderTarget(ren, NULL);
        int w,h;
        SDL_QueryTexture(tx->tx, NULL, NULL, &w, &h);

        SDL_Rect temp;
        temp.w = w;
        temp.h = h;

        if(!(tx->V_CENTER))
            h = 0;
        if(!(tx->H_CENTER))
            w = 0;

        double x = tx->x;
        double y = tx->y;

        temp.x = ((x / 1920) * RES_WIDTH) - w/2;
        temp.y = ((y / 1080) * RES_HEIGHT) - h/2;

        SDL_RenderCopy(ren, tx->tx, NULL, &temp);
    }

    return;
}


SDL_Texture* LoadTexture(std::string file, std::string name)
{
    file = "resources/Graphic/" + file;
    SDL_Texture *temp = IMG_LoadTexture(ren, file.c_str());
    if(nullptr == temp)
    {
        std::string mensaje = "Error in LoadTexture(): Error Loading Texture: " + file;
        LogSDLError(mensaje);
    }

    LoadedTextures.push_back(name);

    return temp;
}


SDL_Texture* LoadText(std::string words, int font_size, std::string name)
{
    TTF_Font *f_impact = TTF_OpenFont("resources/Fonts/impact.ttf", font_size);
    if(f_impact == nullptr)
    {
        LogSDLError("Error in LoadText(): Could not open font");
    }

    SDL_Color clrFg = {255,255,255,0};

    SDL_Surface *temp = TTF_RenderText_Solid(f_impact, words.c_str(), clrFg);
    if(nullptr == temp)
    {
        LogSDLError("Error in LoadText(): Error Creating Temp");
    }

    TTF_CloseFont(f_impact);
    f_impact = NULL;

    SDL_Texture *text = SDL_CreateTextureFromSurface(ren, temp);
    if(nullptr == text)
    {
        LogSDLError("Error in LoadText(): Error Creating \'Text\' Texture");
    }

    SDL_FreeSurface(temp);
    temp = NULL;

    LoadedTextures.push_back(name);

    return text;
}



void RemoveTexture(SDL_Texture *texture, std::string name)
{
    SDL_DestroyTexture(texture);

    for(int x = LoadedTextures.size() - 1; x >= 0; x--)
    {
        if(LoadedTextures[x] == name)
        {
            LoadedTextures.erase(LoadedTextures.begin() + x);
            return;
        }
    }

    LogSDLError("Texture" + name + "not properly loaded");
}


void Stack_Button(B_STACK *stacky, int x, int y, int w, int h, void(*funct)(void))
{
    button *tempbutton = new button;
    tempbutton->x = x;
    tempbutton->y = y;
    tempbutton->w = w;
    tempbutton->h = h;
    tempbutton->funct = funct;
    tempbutton->type = BASIC;
    stacky->push_back(tempbutton);
    return;
}


void Stack_Button(B_STACK *stacky, int x, int y, int w, int h, void(*funct)(int), int L_Pos)
{
    button *tempbutton = new button;
    tempbutton->x = x;
    tempbutton->y = y;
    tempbutton->w = w;
    tempbutton->h = h;
    tempbutton->funct2 = funct;
    tempbutton->type = LIST;
    tempbutton->L_Pos = L_Pos;
    stacky->push_back(tempbutton);
    return;
}


void Stack_Button(B_STACK *stacky, int x, int y, int w, int h, void(*funct)(int, int), int G_PosX, int G_PosY)
{
    button *tempbutton = new button;
    tempbutton->x = x;
    tempbutton->y = y;
    tempbutton->w = w;
    tempbutton->h = h;
    tempbutton->funct3 = funct;
    tempbutton->type = GRID;
    tempbutton->G_PosX = G_PosX;
    tempbutton->G_PosY = G_PosY;
    stacky->push_back(tempbutton);
    return;
}

void Clear_BStack(B_STACK *stacky)
{
    if(stacky == NULL)
        return;

    for(int x = 0; x < stacky->size(); x++)
    {
        delete stacky->at(x);
    }
    stacky->clear();
    return;
}


void Unstack_BStack(B_STACK *stacky)
{
    for(int x = 0; x < Button_Stack.size(); x++)
    {
        if(stacky == Button_Stack[x])
        {
            Clear_BStack(stacky);
            Button_Stack.erase(Button_Stack.begin() + x);
        }
    }
}


void Empty_BStack()
{
    for(int x = Button_Stack.size() - 1; x >= 0; x--)
    {
        Unstack_BStack(Button_Stack[x]);
    }
}


void Stack_BStack(B_STACK *stacky)
{
    Button_Stack.push_back(stacky);
}


void Clear_Tx_Stack(std::vector<Texture *> *input)
{
    for(int x = 0; x < input->size(); x++) //
    {
        delete input->at(x);
    }

    input->clear();
}


void Stack_Texture(TX_STACK *stacky, SDL_Texture *texture, int x, int y, int rotation)
{
    Texture *temp = new Texture;
    temp->x = x;
    temp->y = y;
    temp->w = -1;
    temp->h = -1;
    temp->tx = texture;
    temp->rotation = rotation;
    temp->text = false;
    temp->target = wall;

    stacky->push_back(temp);

    return;
}


void Stack_TextureEx(TX_STACK *stacky, SDL_Texture *texture, int x, int y, int w, int h, int rotation)
{
    Texture *temp = new Texture;
    temp->x = x;
    temp->y = y;
    temp->w = w;
    temp->h = h;
    temp->tx = texture;
    temp->rotation = rotation;
    temp->text = false;
    temp->target = wall;

    stacky->push_back(temp);

    return;
}

void Stack_Text(TX_STACK *stacky, SDL_Texture *texture, int x, int y, bool H_CENTER, bool V_CENTER)
{
    Texture *temp = new Texture;
    temp->x = x;
    temp->y = y;
    temp->tx = texture;
    temp->rotation = 0;
    temp->text = true;
    temp->target = NULL;
    temp->V_CENTER = V_CENTER;
    temp->H_CENTER = H_CENTER;

    stacky->push_back(temp);

    return;
}


void Stack_TextEx(TX_STACK *stacky, SDL_Texture *texture, int x, int y, int w, int h, bool H_CENTER, bool V_CENTER)
{
    Texture *temp = new Texture;
    temp->x = x;
    temp->y = y;
    temp->h = h;
    temp->w = w;
    temp->tx = texture;
    temp->rotation = 0;
    temp->text = true;
    temp->target = NULL;
    temp->V_CENTER = V_CENTER;
    temp->H_CENTER = H_CENTER;

    stacky->push_back(temp);

    return;
}


Texture *CreateTexture(SDL_Texture *texture, int x, int y, SDL_Texture *target, int rotation)
{
    Texture *temp = new Texture;
    temp->x = x;
    temp->y = y;
    temp->h = -1;
    temp->w = -1;
    temp->tx = texture;
    temp->text = false;
    temp->rotation = rotation;
    temp->target = target;
    return temp;
}


void Stack_Stack(TX_STACK *stacky)
{
    Tx_Stack.push_back(stacky);
    return;
}


void Stack_TStack(TX_STACK *stacky)
{
    Text_Stack.push_back(stacky);
    return;
}


void Unstack_Stack(TX_STACK *swag)
{
    for(int x = Tx_Stack.size() - 1; x >= 0; x--)
    {
        if(swag == Tx_Stack.at(x))
        {
            Clear_Tx_Stack(swag);
            Tx_Stack.erase(Tx_Stack.begin() + x);
        }
    }
    delete swag;
}


void Unstack_TStack(TX_STACK *swag)
{
    for(int x = Text_Stack.size() - 1; x >= 0; x--)
    {
        if(swag == Text_Stack.at(x))
        {
            Clear_Tx_Stack(swag);
            Text_Stack.erase(Text_Stack.begin() + x);
        }
    }
    delete swag;
}


void Unstack_Texture(TX_STACK *stacky, SDL_Texture *scrap)
{
    if(stacky->size() > 0)
    {
        for(int x = stacky->size() - 1; x >= 0; x--)
        {
            if(scrap == stacky->at(x)->tx)
            {
                stacky->erase(stacky->begin() + x);
            }
        }
    }
}


void Exit_Sequence_Remove(void(*funct)(void))
{
    if(Exit_Sequence.size() < 1)
    {
        return;
    }

    for(int x = Exit_Sequence.size() - 1; x >= 0; x--)
    {
        if(Exit_Sequence[x] == funct)
        {
            Exit_Sequence.erase(Exit_Sequence.begin() + x);
            return;
        }
    }
    LogSDLError("Exit_Sequence_Remove: Exit function not found");

}


void Exit_Sequence_Add(void(*funct)(void))
{
    Exit_Sequence.push_back(funct);
}




void ErrorMessage(std::string message, void(*fallback)())
{
    SDL_Texture *T_message = LoadText(message, 15, "ErrorMessage");
    SDL_Texture *buttony = LoadTexture("Button.png", "ErrorMessage: Button");

    Error_Stack = new TX_STACK;
    Stack_Stack(Error_Stack);

    T_Error = new TX_STACK;
    Stack_TStack(T_Error);

    Stack_Text(T_Error, T_message, 960, 540, true, true);

    Stack_Texture(Error_Stack, buttony, 860, 690);

    Empty_BStack();
    B_Error = new B_STACK;
    Stack_BStack(B_Error);

    Stack_Button(B_Error, 860, 690, 200, 100, Exit_ErrorMessage);

    func = fallback;
}


void Exit_ErrorMessage()
{
    Unstack_Stack(Error_Stack);
    Unstack_TStack(T_Error);
    Unstack_BStack(B_Error);
    RemoveTexture(T_message, "ErrorMessage");
    RemoveTexture(buttony, "ErrorMessage: Button");
    Backout();

    func();
}


void Stack_Function(void(*funct)(void))
{
    Funct_Stack.push_back(funct);
}


void Unstack_Function(void(*funct)(void))
{
    for(int x = 0; x < Funct_Stack.size(); x++)
    {
        if(funct == Funct_Stack[x])
        {
            Funct_Stack.erase(Funct_Stack.begin() + x);
            return;
        }
    }
}


void Stack_Key(SDL_Keycode input, void(*funct)(void))
{
    Key *Swag = new Key;
    Swag->input = input;
    Swag->funct = funct;
    Key_Stack.push_back(Swag);
}


void Unstack_Key(SDL_Keycode input)
{
    for(int x = 0; x < Key_Stack.size(); x++)
    {
        if(input == Key_Stack.at(x)->input)
        {
            Key_Stack.erase(Key_Stack.begin() + x);
        }
    }
}


void Backout(void)
{
    Exit_Sequence.back()();
}


void Change_Angle(TX_STACK *stacky, SDL_Texture *swags, int angle)
{
    for(int x = stacky->size() - 1; x >= 0; x--)
    {
        if(swags == stacky->at(x)->tx)
        {
            stacky->at(x)->rotation = angle;
        }
    }
}


bool Load_Level()
{
    HighScorers.clear();
    HighScores.clear();

    std::ifstream File;
    if(loadmode == EDIT)
        File.open("User/Level Editor/" + Lvl_name);
    else
        File.open("Resources/Levels/" + Lvl_name);

    if(File.is_open())
    {
        std::string FileData;
        getline(File, FileData);
        for(int x = 0; x < 121; x++)
        {
            LevelDatatx[x] = FileData[x] - '0';
        }

        for(int x = 0; x < 121; x++)
        {
            LevelData[x] = FileData[x + 121] - '0';
        }

        std::stringstream swag;
        int x;
        for(x = 242; FileData[x] != '~' && x < FileData.size(); x++)
        {
            swag << FileData[x];
        }

        swag >> Lvl_Bg;

        swag.str("");
        swag.clear();


        for(int y = 0; y < 10; y++)
        {
            std::string timp;
            for(++x; FileData[x] != '~' && x < FileData.size(); x++)
            {
                timp.push_back(FileData[x]);
            }
            HighScorers.push_back(timp);
        }


        for(int y = 0; y < 10; y++)
        {
            for(++x; FileData[x] != '~' && x < FileData.size(); x++)
            {
                swag << FileData[x];
            }
            int tiemp;
            swag >> tiemp;
            HighScores.push_back(tiemp);
            swag.str("");
            swag.clear();
        }

        swag.str("");
        swag.clear();

        for(++x; FileData[x] != '~' && x < FileData.size(); x++)
        {
            swag << FileData[x];
        }
        if(par < 3)
            swag >> par;
        return true;
    }else
    {
        LogSDLError("Could not open Level" + Lvl_name);
        return false;
    }
}

