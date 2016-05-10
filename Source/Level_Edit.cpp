#include "../Headers/States.h"
#include "../Headers/Global.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>

#define LEVEL_SELECT 0
#define LEVEL_SETTINGS 1
#define LEVEL_EDIT 2


TX_STACK *LE_LS_Stack;
TX_STACK *LE_LSet_Stack;
TX_STACK *LE_LSet_Stack_Bg;

TX_STACK *LE_LS_TStack;
TX_STACK *LE_LSet_TStack;

B_STACK *LE_LS_BStack = NULL;
B_STACK *LE_LS_LVLBStack = NULL;
B_STACK *LE_LSet_BStack;
B_STACK *LE_LSet_BStack_Bg;


int read_Settings(void);
void DS_LE_LS(void);
void DS_LE_LSet(void);

SDL_Texture *bg_LevelEdit = nullptr;
SDL_Texture *LE_Highlight = nullptr;
SDL_Texture *bg_LELSet = nullptr;

int dis_state = LEVEL_SELECT;
const int dis_LEVELS_MAX = 7;
int Sel_Level = -1;
int Level_Page = 0;

int setmode = PLAY;

int Sel_bg = 0;
bool NewLevel = false;

const int BG_SELECT_Y = 500;

extern bool QuitAll;
extern int par;

extern SDL_Window *win;
extern SDL_Renderer *ren;
extern SDL_Texture *wall;
extern int RES_WIDTH;
extern int RES_HEIGHT;

std::vector<Level> Levels;
std::vector<SDL_Texture *> Backgrounds;
std::vector<std::string> BgNames;

std::string Lvl_Bg;
std::string Lvl_name;


void LE_Stack_LVLButtons(void);
void LE_Stack_LVLNames(void);
void LE_Stack_SelBG(int x);
void LE_Stack_BGs(void);

void LE_LSet_Unstack(void);
void LE_LS_Unstack(void);

void B_LE_LSet_Back(void);
void B_LE_LSet_LeftArrow();
void B_LE_LSet_RightArrow();
void B_LE_LSet_Continue(void);

void B_LE_LS_NewLevel(void);
void B_LE_LS_RightArrow(void);
void B_LE_LS_LeftArrow(void);
void B_LE_SelLevel(int x);
void B_LE_LS_Load();
void B_LE_LS_Delete();

void SaveLevels();
void B_LE_LSet_Return();
void B_LE_Exit();



void LE_Load(void)
{

    std::ifstream File("User/Level Editor/Levels.txt");
    std::string temp;
    while(getline(File, temp))
    {
        if(temp != "")
        {
            Level templevel;
            templevel.name = temp;
            Levels.push_back(templevel);
        }

    }

    File.close();

    for(unsigned int x = 0; x < Levels.size(); x++)
    {
        Levels[x].t_Name = LoadText(Levels.at(x).name, 26, "Level Edit: Level Names");
        Levels[x].pos = x;
    }

    File.open("resources/Graphic/Backgrounds/Backgrounds.txt");
    while(getline(File, temp))
    {
        if(temp != "")
            BgNames.push_back(temp);
    }

    File.close();

    for(int x = 0; x < Backgrounds.size() - 1; x++)
    {
        Backgrounds.push_back(LoadTexture("Backgrounds/" + BgNames[x], "Level Edit Menu: Levels[]"));
        //if(Backgrounds.at(Backgrounds.size() - 1) == NULL)
            //Backgrounds.erase(Backgrounds.end() - 1);
    }

    bg_LevelEdit = LoadTexture("LELSMenu.png", "Level Edit: Background");
    bg_LELSet = LoadTexture("Level-Settings.png", "Level Settings: Background");
    LE_Highlight = LoadTexture("Highlight.png", "Level Edit: Highlight");

    Exit_Sequence_Add(LE_Unload);

    if(setmode != EDIT)
    {
        DS_LE_LS();
    }
    else
    {
        DS_LE_LSet();
    }

    Sel_Level = -1;

    return;
}


void LE_Unload()
{
    RemoveTexture(bg_LevelEdit, "Level Edit: Background");
    RemoveTexture(LE_Highlight, "Level Edit: Highlight");
    RemoveTexture(bg_LELSet, "Level Settings: Background");

    for(int x = 0; x < Levels.size(); x++)
    {
        RemoveTexture(Levels[x].t_Name, "Level Edit: Level Names");
    }

    for(int x = 0; x < Backgrounds.size(); x++)
    {
        RemoveTexture(Backgrounds[x], "Level Edit Menu: Levels[]");
    }


    Levels.clear();
    Backgrounds.clear();


    Exit_Sequence_Remove(LE_Unload);

    Sel_Level = -1;

    return;
}

SDL_Texture *DSLE_Tx[10];

void DS_LE_LS(void) //Stack: Level Select Menu
{
    par = 3;
    LE_LS_BStack = new B_STACK;
    LE_LS_LVLBStack = new B_STACK;
    LE_LS_Stack = new TX_STACK;
    LE_LS_TStack = new TX_STACK;

    Stack_Texture(LE_LS_Stack, bg_LevelEdit); //Background

    //Stack Level Edit Buttons

    LE_Stack_LVLButtons();

    LE_Stack_LVLNames();

    Stack_Stack(LE_LS_Stack);
    Stack_TStack(LE_LS_TStack);
    Stack_BStack(LE_LS_BStack);
    Stack_BStack(LE_LS_LVLBStack);

    DSLE_Tx[0] = LoadText("New", 26, "Level Edit: DSLE_Tx");
    DSLE_Tx[1] = LoadText("Load", 26, "Level Edit: DSLE_Tx");
    DSLE_Tx[2] = LoadText("Delete", 26, "Level Edit: DSLE_Tx");
    DSLE_Tx[3] = LoadText("Back", 26, "Level Edit: DSLE_Tx");

    Stack_Text(LE_LS_TStack, DSLE_Tx[0], 446, 1005);
    Stack_Text(LE_LS_TStack, DSLE_Tx[1], 966, 1005);
    Stack_Text(LE_LS_TStack, DSLE_Tx[2], 1476, 1005);
    Stack_Text(LE_LS_TStack, DSLE_Tx[3], 125, 57);

    Stack_Button(LE_LS_BStack, 125, 515, 115, 140, B_LE_LS_LeftArrow);
    Stack_Button(LE_LS_BStack, 1680, 510, 115, 145, B_LE_LS_RightArrow);
    Stack_Button(LE_LS_BStack, 836, 951, 250, 115, B_LE_LS_Load);
    Stack_Button(LE_LS_BStack, 331, 951, 250, 115, B_LE_LS_NewLevel);
    Stack_Button(LE_LS_BStack, 1340, 950, 250, 115, B_LE_LS_Delete);
    Stack_Button(LE_LS_BStack, 0, 0, 250, 115, B_LE_Exit);
}

void B_LE_Exit()
{
    LE_LS_Unstack();
    LE_Unload();
    MM_Load();
}

void Update_partext();
SDL_Texture *partext = NULL;
void B_partextR();
void B_partextL();

void DS_LE_LSet(void) //Stack: Level EDIT menu
{
    LE_LSet_Stack = new TX_STACK;
    LE_LSet_TStack = new TX_STACK;
    LE_LSet_Stack_Bg = new TX_STACK;
    Stack_TStack(LE_LSet_TStack);
    Stack_Stack(LE_LSet_Stack);
    Stack_Stack(LE_LSet_Stack_Bg);

    LE_LSet_BStack = new B_STACK;
    LE_LSet_BStack_Bg = new B_STACK;
    Stack_BStack(LE_LSet_BStack);
    Stack_BStack(LE_LSet_BStack_Bg);

    Stack_Texture(LE_LSet_Stack, bg_LELSet);

    Stack_Button(LE_LSet_BStack, 300, BG_SELECT_Y, 100, 100, B_LE_LSet_LeftArrow); //Left Arrow
    Stack_Button(LE_LSet_BStack, 1520, BG_SELECT_Y, 100, 100, B_LE_LSet_RightArrow); //Right Arrow
    Stack_Button(LE_LSet_BStack, 550, 926, 806, 108, B_LE_LSet_Continue);
    if(setmode == EDIT)
    {
        //Stack_Button(LE_LSet_BStack, 550, 926, 806, 108, B_LE_LSet_Return); //Back Button
    }
    else
    {

    }

    Stack_Button(LE_LSet_BStack, 353, 741, 152, 115, B_partextL);
    Stack_Button(LE_LSet_BStack, 1398, 741, 152, 115, B_partextR);

    Update_partext();

    LE_Stack_BGs();
}


void Update_partext()
{
    Unstack_Texture(LE_LSet_TStack, partext);
    if(partext != NULL)
        RemoveTexture(partext, "LE: Partext");
    std::stringstream swaggit;
    std::string swagging;
    swaggit << par;
    swaggit >> swagging;

    partext = LoadText(swagging, 26, "LE: Partext");
    Stack_Text(LE_LSet_TStack, partext, 947, 805);
}

void B_partextR()
{
    if(par < 999)
        par++;
    Update_partext();
}


void B_partextL()
{
    if(par > 3)
    {
        par--;
    }
    Update_partext();
}

void LE_Stack_LVLNames(void)
{
    Clear_Tx_Stack(LE_LS_TStack);
    for(int x = Level_Page * dis_LEVELS_MAX; x < Levels.size() && x < dis_LEVELS_MAX + (Level_Page * dis_LEVELS_MAX); x++)
    {
        Stack_Text(LE_LS_TStack, Levels[x].t_Name, 960,((x - (Level_Page * dis_LEVELS_MAX)) + 1) * 100 + 120, true, false);
    }
}


void LE_Stack_LVLButtons()
{
    Clear_BStack(LE_LS_LVLBStack);
    for(int x = Level_Page * dis_LEVELS_MAX; x < Levels.size() && x < dis_LEVELS_MAX + (Level_Page * dis_LEVELS_MAX); x++)
    {
        Stack_Button(LE_LS_LVLBStack, 360, ((x - (Level_Page * dis_LEVELS_MAX)) + 1) * 100 + 120, 900, 100, *B_LE_SelLevel, x);
    }
}


void B_LE_SelLevel(int x) //Highlight Clicked Level
{
    if(Sel_Level >= 0)
        Unstack_Texture(LE_LS_Stack, LE_Highlight);
    if(x >= 0)
    {
        Sel_Level = x + 1;
        Stack_Texture(LE_LS_Stack, LE_Highlight, 310, (x+1) * 100 + 125);
    }
}


void B_LE_LS_RightArrow(void)
{
    if(Level_Page < Levels.size() / dis_LEVELS_MAX)
        Level_Page++;
    LE_Stack_LVLNames();
}


void LE_LS_Unstack(void)
{
    for(int x = 0; x < 4; x++)
    {
        RemoveTexture(DSLE_Tx[x], "Level Edit: DSLE_Tx");
    }

    Unstack_Stack(LE_LS_Stack);
    Unstack_TStack(LE_LS_TStack);
    Unstack_BStack(LE_LS_BStack);
    Unstack_BStack(LE_LS_LVLBStack);
}


void B_LE_LS_NewLevel(void)
{
    LE_LS_Unstack();
    Lvl_name = "New Level";
    DS_LE_LSet();
}


void B_LE_LS_LeftArrow()
{
    if(Level_Page > 0)
        Level_Page--;
    LE_Stack_LVLNames();
}


void LE_Stack_BGs(void)
{
    Clear_Tx_Stack(LE_LSet_Stack_Bg);

    Stack_TextureEx(LE_LSet_Stack_Bg, Backgrounds[Sel_bg], 960, BG_SELECT_Y - 165, 330, 330);
}


void B_LE_LSet_Return()
{
    if(Sel_bg > 0)
    {
        LE_LSet_Unstack();
        Lvl_Bg = BgNames[Sel_bg];
    }
    LE_LSet_Unstack();
    LE_Unload();
    GP_Load(EDIT);
}


void B_LE_LSet_LeftArrow()
{
    if(Sel_bg > 0)
        Sel_bg--;
    LE_Stack_BGs();
}


void B_LE_LSet_RightArrow()
{
    if(Sel_bg < Backgrounds.size())
        Sel_bg++;
    LE_Stack_BGs();
}


void B_LE_LSet_Continue()
{
    Lvl_Bg = BgNames[Sel_bg];
    int y;
    for(y = 0; y < 9999; y++)
    {
        std::stringstream potato;
        std::string potato2;
        potato << y;
        potato >> potato2;
        bool swag = false;

        for(int x = 0; x < Levels.size(); x++)
        {

            if(Levels.at(x).name == "New Level " + potato2)
            {
                swag = true;
                break;
            }
        }
        if(swag == false)
            break;
    }

    std::stringstream potato;
    std::string potato2;
    potato << y;
    potato >> potato2;


    Lvl_name = "New Level " + potato2;
    Level temp;
    temp.name = Lvl_name;
    Levels.push_back(temp);
    SaveLevels();

    LE_LSet_Unstack();
    LE_Unload();
    GP_Load(EDIT);
}

void B_LE_LS_Load()
{
    if(Sel_Level > -1)
    {
        Lvl_name = Levels.at((Level_Page * dis_LEVELS_MAX) + Sel_Level - 1).name;
        LE_LS_Unstack();
        LE_Unload();
        GP_Load(EDIT);
    }
}


void B_LE_LS_Delete()
{
    if(Sel_Level > -1)
    {
        std::string temp = "User/Level Editor/" + Levels.at((Level_Page * dis_LEVELS_MAX) + Sel_Level - 1).name;
        remove(temp.c_str());
        Levels.erase(Levels.begin() + (Level_Page * dis_LEVELS_MAX) + Sel_Level - 1);
        LE_Stack_LVLNames();
        SaveLevels();
        B_LE_SelLevel(-1);
    }
}


void LE_LSet_Unstack()
{
    Unstack_Stack(LE_LSet_Stack);
    Unstack_Stack(LE_LSet_Stack_Bg);
    Unstack_BStack(LE_LSet_BStack);
    Unstack_BStack(LE_LSet_BStack_Bg);
    Unstack_TStack(LE_LSet_TStack);
    RemoveTexture(partext, "LE: partext");
}


void SaveLevels()
{
    std::ofstream File("User/Level Editor/Levels.txt");
    if(File.is_open())
    {
        for(int x = 0; x < Levels.size(); x++)
        {
            File << Levels.at(x).name << std::endl;
        }
    }
    else
    {
        LogSDLError("LE:SaveFileNames(): Could not open /User/Level Editor/Levels.txt");
    }

    File.close();
}
