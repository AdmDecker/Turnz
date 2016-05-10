#include <string>
#include <iostream>
#include <fstream>
#include "../Headers/Global.h"
#include "../Headers/States.h"
#include <vector>
#include <sstream>

#define EMPTY 0
#define BLOB 1
#define BLOCK 2
#define FINISH 3

const bool rotateblocks = false;

struct Blob
{
    bool isfalling = false;
    bool toDelete = false;
    int posx, posy, UID;
    int tx;
};



extern SDL_Window *win;
extern SDL_Renderer *ren;
extern SDL_Texture *wall;
extern int RES_WIDTH;
extern int RES_HEIGHT;

extern int setmode;

extern std::string Starttext;

std::vector<SDL_Texture *> Blocks;
std::vector<Blob> Blobs;
int par;
std::vector<std::string> HighScorers;
std::vector<int> HighScores;
int LevelDatatx[121];
int GPLevelDatatx[11][11];
int LevelData[121];
int GPLevelData[11][11];
SDL_Texture *tx_map[11][11];

SDL_Texture *LV_Tx[4];

extern std::string Lvl_Bg;
extern std::string Lvl_name;

bool LV_VIEW = false;



void GP_Unload(void);

void Update_brick();
void Update_txmap();
void Update_panel();
void Update_GPLevelData();

void GP_EditorLoad();
void GP_EditorUnload();

void GP_Sel_tx(int z);
void GP_Sel_slot(int z);
void GP_SelTool(int x);

void GP_ExitEditor();
void GP_SaveLevel();
void GP_TestLevel();
void InitLevel();

void GP_B_Settings();
void B_LV_view();
void Scanlevel();

TX_STACK *GP_TxStack;
TX_STACK *GP_TxtStack;

TX_STACK *GP_E_TxStack;
TX_STACK *GP_E_TxtStack;
TX_STACK *GP_E_TxtToolStack;

B_STACK *GP_panel_BStack;
B_STACK *GP_toolset_BStack;
B_STACK *GP_brick_BStack;
B_STACK *GP_E_BStack;

SDL_Texture *Lvl_Bg_tx;
SDL_Texture *GP_victorytext;
SDL_Texture *GP_gameovertext;

SDL_Texture *brick;
SDL_Texture *panel;
SDL_Texture *GP_button;


SDL_Texture *Buttons_txt[5];

SDL_Texture *tools[4];

int Block_Page = 0;
int Sel_txtool = -1;
int Sel_tool = -1;

extern int loadmode;


SDL_Texture *Lvl_Info[5];

int Gravity = 0;
double pos = 0;
double screenangle = 0;

SDL_Texture *GameTexture[3];

void GP_Load(int mode)
{
    setmode = mode;

    GP_TxtStack = new TX_STACK;
    Stack_TStack(GP_TxtStack);

    HighScores.clear();
    HighScorers.clear();

    std::ifstream File;
    std::string temp;
    {//Load Blocks
    File.open("resources/Graphic/Blocks/Blocks.txt");
    while(getline(File, temp))
    {
        SDL_Texture* block = LoadTexture("Blocks/" + temp, "GP: Block");
        if(block == NULL)
        {
            ErrorMessage("Fatal Error: Block texture missing. Please replace block textures.", LE_Load);
        }
        Blocks.push_back(block);
    }
    }
    File.close();

    {//Load Level From File
    if(!Load_Level())
    {
        if(mode == EDIT)
        {
            for(int x = 0; x < 121; x++)
            {
                LevelDatatx[x] = 0;
                LevelData[x] = 0;
            }
            for(int x = 0; x < 10; x++)
            {
                HighScores.push_back(999);
                HighScorers.push_back("NOBODY");
            }

            GP_SaveLevel();
        }
        else
        {
            LogSDLError("Could not open level.");
        }
    }
    }
    File.close();
    brick = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 990, 990);
    SDL_SetTextureBlendMode(brick, SDL_BLENDMODE_BLEND);


    Update_GPLevelData();
    Scanlevel();
    Update_txmap();
    Update_brick();

    GP_TxStack = new TX_STACK;
    Stack_Stack(GP_TxStack);

    Lvl_Bg_tx = LoadTexture("Backgrounds/" + Lvl_Bg, "Game: Level Background");

    Stack_Texture(GP_TxStack, Lvl_Bg_tx, 465, 45);
    Stack_Texture(GP_TxStack, brick, 465, 45);

    if(mode == EDIT)
    {
        GP_EditorLoad();
    }
    else if(mode == PLAY)
    {
        InitLevel();
    }

    Lvl_Info[0] = LoadText(Lvl_name, 26, "GP: Lvl Info");

    Stack_Text(GP_TxtStack, Lvl_Info[0], 235, 87);

    Exit_Sequence_Add(GP_Unload);
}


void GP_Unload(void)
{
    for(int x = 0; x < Blocks.size(); x++)
        RemoveTexture(Blocks[x], "GP: Block");

    Blocks.clear();

    RemoveTexture(Lvl_Bg_tx, "Game: Level Background");
    RemoveTexture(GameTexture[0], "Game: Background");
    RemoveTexture(GP_gameovertext, "GP: Gameover text");

    RemoveTexture(Lvl_Info[0], "GP: Lvl Info");

    SDL_DestroyTexture(brick);

    Unstack_Stack(GP_TxtStack);

    Unstack_TStack(GP_TxtStack);





    Exit_Sequence_Remove(GP_Unload);
}

SDL_Texture *Editor_Back;
SDL_Texture *Hollow;

void GP_EditorLoad()
{
    {//Stacks Loading
    //Button Stacks
    GP_E_BStack = new B_STACK;
    GP_panel_BStack = new B_STACK;
    GP_brick_BStack = new B_STACK;
    GP_toolset_BStack = new B_STACK;


    Stack_BStack(GP_E_BStack);
    Stack_BStack(GP_panel_BStack);
    Stack_BStack(GP_brick_BStack);
    Stack_BStack(GP_toolset_BStack);

    //Texture Stacks
    GP_E_TxStack = new TX_STACK;
    GP_E_TxtStack = new TX_STACK;

    Stack_Stack(GP_E_TxStack);
    Stack_TStack(GP_E_TxtStack);


    //Text Stacks
    GP_E_TxtToolStack = new TX_STACK;

    Stack_TStack(GP_E_TxtToolStack);
    }

    Update_GPLevelData();
    Scanlevel();
    Update_txmap();
    Update_brick();




    Editor_Back = LoadTexture("Level-Editor.png", "Level Editor: Background");
    Hollow = LoadTexture("Hollow.png", "GP: Hollow");

    Stack_Texture(GP_E_TxStack, Editor_Back);

    panel = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 270, 630);
    SDL_SetTextureBlendMode(panel, SDL_BLENDMODE_BLEND);

    Update_panel();

    {//Create Button Matrix
        int z = 0;
        for(int x = 0; x < 11; x++)
        {
            for(int y = 0; y < 11; y++)
            {
                Stack_Button(GP_brick_BStack, x * 90 + 465, y * 90 + 45, 90, 90, GP_Sel_slot, z++);
            }
        }
    }

    {//Create Toolset panel
        tools[EMPTY] = LoadText("Space", 20, "GP tools text");
        tools[BLOB] = LoadText("Blob", 20, "GP tools text");
        tools[BLOCK] = LoadText("Block", 20, "GP tools text");
        tools[FINISH] = LoadText("Finish", 20, "GP tools text");

        for(int x = 0; x < 4; x++)
        {
            Stack_Button(GP_E_BStack, 1611, (80 * x) + 129, 194, 60, GP_SelTool, x);
            Stack_Text(GP_E_TxtStack, tools[x], 1708, (80 * x) + 161 );
        }
    }

    Stack_Texture(GP_E_TxStack, panel, 98, 347);
    Stack_Texture(GP_E_TxStack, Hollow, 98, 347);
    Stack_Texture(GP_E_TxStack, Hollow, 1669, 537);


    Buttons_txt[0] = LoadText("Exit", 20, "GP: Exit Button Text");
    Buttons_txt[1] = LoadText("View", 20, "GP: View Button Text");
    Buttons_txt[2] = LoadText("Save", 20, "GP: Save Button Text");
    Buttons_txt[3] = LoadText("Test", 20, "GP: Test Button Text");
    Buttons_txt[4] = LoadText("Settings", 20, "GP: Settings Button Text");

    Stack_Text(GP_E_TxtStack, Buttons_txt[0], 1729 + 90, 1014 + 29); //Exit Button
    Stack_Text(GP_E_TxtStack, Buttons_txt[1], 1512 + 90, 814 + 42); //View Button
    Stack_Text(GP_E_TxtStack, Buttons_txt[2], 1512 + 90, 919 + 42); //Save Button
    Stack_Text(GP_E_TxtStack, Buttons_txt[3], 1723 + 90, 814 + 42); //Test Button
    Stack_Text(GP_E_TxtStack, Buttons_txt[4], 1723 + 90, 919 + 42); //Settings Button

    Stack_Button(GP_E_BStack, 1512, 919, 180, 85, GP_SaveLevel); //Save Level
    Stack_Button(GP_E_BStack, 1723, 919, 180, 85, GP_B_Settings); //Settings

    Stack_Button(GP_E_BStack, 1723, 814, 180, 85, GP_TestLevel); //Test Level
    Stack_Button(GP_E_BStack, 1512, 814, 180, 85, B_LV_view); //Logical View
    Stack_Button(GP_E_BStack, 1729, 1014, 170, 58, GP_ExitEditor); //Exit Level


    {//LV stuff
        LV_Tx[0] = LoadTexture("LV_Empty.png", "LV_Empty");
        LV_Tx[1] = LoadTexture("LV_Blob.png", "LV_Blob");
        LV_Tx[2] = LoadTexture("LV_Block.png", "LV_Block");
        LV_Tx[3] = LoadTexture("LV_Finish.png", "LV_Finish");
    }
    Exit_Sequence_Add(GP_EditorUnload);
}


void GP_EditorUnload()
{
    SDL_DestroyTexture(panel);
    RemoveTexture(Editor_Back, "Level Editor: Background");

    Unstack_BStack(GP_panel_BStack);
    Unstack_BStack(GP_brick_BStack);
    Unstack_BStack(GP_E_BStack);
    Unstack_BStack(GP_toolset_BStack);

    Unstack_Stack(GP_E_TxStack);

    Unstack_TStack(GP_E_TxtStack);
    Unstack_TStack(GP_E_TxtToolStack);

    RemoveTexture(tools[0], "GP tools text");
    RemoveTexture(tools[1], "GP tools text");
    RemoveTexture(tools[2], "GP tools text");
    RemoveTexture(tools[3], "GP tools text");

    RemoveTexture(LV_Tx[0], "LV_Empty");
    RemoveTexture(LV_Tx[1], "LV_Blob");
    RemoveTexture(LV_Tx[2], "LV_Block");
    RemoveTexture(LV_Tx[3], "LV_Finish");

    RemoveTexture(Buttons_txt[0], "GP: Exit Button Text");
    RemoveTexture(Buttons_txt[1], "GP: View Button Text");
    RemoveTexture(Buttons_txt[2], "GP: Save Button Text");
    RemoveTexture(Buttons_txt[3], "GP: Test Button Text");
    RemoveTexture(Buttons_txt[4], "GP: Settings Button Text");


    LV_VIEW = false;

    Exit_Sequence_Remove(GP_EditorUnload);
}


void Update_txmap()
{

    for(int x = 0; x < 11; x++)
    {
        for(int y = 0; y < 11; y++)
        {
            tx_map[x][y] = Blocks[GPLevelDatatx[x][y]];
        }
    }
}


void Update_GPLevelData()
{
    int z = 0;
    for(int x = 0; x < 11; x++)
    {
        for(int y = 0; y < 11; y++)
        {
            GPLevelData[x][y] = LevelData[z];
            if(LevelData[z] != BLOB)
            {
                GPLevelDatatx[x][y] = LevelDatatx[z];
            }
            else
            {
                GPLevelDatatx[x][y] = EMPTY;
            }

            z++;
        }
    }
}


void Update_brick()
{
    SDL_Rect square;
    square.h = 90;
    square.w = 90;

    SDL_SetRenderTarget(ren, brick);

    //SDL_SetTextureBlendMode(Blocks[0], SDL_BLENDMODE_NONE);
    //SDL_RenderCopy(ren, Blocks[0], NULL, NULL);
    //SDL_SetTextureBlendMode(Blocks[0], SDL_BLENDMODE_BLEND);

    if(!LV_VIEW)
    {
        for(int x = 0; x < 11; x++)
        {
            for(int y = 0; y < 11; y++)
            {
                    square.x = x*90;
                    square.y = y*90;
                    if(rotateblocks == false)
                    {
                        SDL_SetTextureBlendMode(tx_map[x][y], SDL_BLENDMODE_NONE);
                        SDL_RenderCopy(ren, tx_map[x][y], NULL, &square);
                        SDL_SetTextureBlendMode(tx_map[x][y], SDL_BLENDMODE_BLEND);
                    }
                    else
                    {
                        SDL_SetTextureBlendMode(tx_map[x][y], SDL_BLENDMODE_BLEND);
                        SDL_RenderCopyEx(ren, tx_map[x][y], NULL, &square, -screenangle, NULL, SDL_FLIP_NONE);
                        SDL_SetTextureBlendMode(tx_map[x][y], SDL_BLENDMODE_NONE);
                    }

            }
        }


        for(int z = 0; z < Blobs.size(); z++)
        {
            int x = Blobs[z].posx;
            int y = Blobs[z].posy;
            if(Blobs[z].isfalling)
            {
                switch (Gravity)
                {
                    case 0:
                        square.x = x * 90;
                        square.y = y * 90 + pos;
                        break;
                    case 1:
                        square.x = x * 90 - pos;
                        square.y = y * 90;
                        break;
                    case 2:
                        square.x = x * 90;
                        square.y = y * 90 - pos;
                        break;
                    case 3:
                        square.x = x * 90 + pos;
                        square.y = y * 90;
                        break;
                }

            }
            else
            {
                square.x = x * 90;
                square.y = y * 90;
            }


            SDL_SetTextureBlendMode(Blocks[Blobs[z].tx], SDL_BLENDMODE_BLEND);
            SDL_RenderCopyEx(ren, Blocks[Blobs[z].tx], NULL, &square, -screenangle, NULL, SDL_FLIP_NONE);
        }
    }
    else
    {
        for(int x = 0; x < 11; x++)
        {
            for(int y = 0; y < 11; y++)
            {
                square.x = x*90;
                square.y = y*90;
                SDL_SetTextureBlendMode(LV_Tx[GPLevelData[x][y]], SDL_BLENDMODE_NONE);
                SDL_RenderCopy(ren, LV_Tx[GPLevelData[x][y]], NULL, &square);
                SDL_SetTextureBlendMode(LV_Tx[GPLevelData[x][y]], SDL_BLENDMODE_BLEND);
            }
        }
    }
    SDL_SetRenderTarget(ren, NULL);

}


void Update_panel()
{
    int x = 0;
    int y = 0;

    SDL_SetRenderTarget(ren, panel);

    SDL_SetTextureBlendMode(Blocks[0], SDL_BLENDMODE_NONE);
    SDL_RenderCopy(ren, Blocks[0], NULL, NULL);
    SDL_SetTextureBlendMode(Blocks[0], SDL_BLENDMODE_BLEND);

    SDL_Rect square;
    square.h = 90;
    square.w = 90;

    Clear_BStack(GP_panel_BStack);

    for(int z = Block_Page * 27; z < Blocks.size() && z < (Block_Page + 1) * 27; z++)
    {
        if(x < 3)
        {
            square.x = 90 * x++;
        }
        else
        {
            x = 0;
            square.x = 90 * x++;
            square.y = 90 * ++y;
        }
        Stack_Button(GP_panel_BStack, square.x + 98, square.y + 347, square.w, square.h, GP_Sel_tx, z);
        SDL_RenderCopy(ren, Blocks.at(z), NULL, &square);
    }
    SDL_SetRenderTarget(ren, NULL);
}


void GP_Sel_tx(int z)
{
    if(Sel_txtool != -1)
    {
        Unstack_Texture(GP_E_TxStack, Blocks.at(Sel_txtool));
    }
    Sel_txtool = z;
    Stack_Texture(GP_E_TxStack, Blocks.at(Sel_txtool), 1669, 537);
}


void GP_Sel_slot(int z)
{
    if(Sel_txtool != -1 && Sel_tool != -1)
    {
        LevelDatatx[z] = Sel_txtool;
        LevelData[z] = Sel_tool;
    }

    Update_GPLevelData();

    Update_txmap();
    Scanlevel();
    Update_brick();
}


void GP_SaveLevel()
{
    std::ofstream File;

    std::string Filepath;

    if(loadmode == EDIT)
    {
        Filepath = "User/Level Editor/" + Lvl_name;
    }
    else
    {
        Filepath = ("resources/levels/" + Lvl_name);
    }

    File.open(Filepath.c_str());


    for(int x = 0; x < 121; x++)
    {
        File << LevelDatatx[x];
    }

    for(int x = 0; x < 121; x++)
    {
        File << LevelData[x];
    }

    File << Lvl_Bg << '~';

    for(int x = 0; x < 10; x++)
    {
        File << HighScorers[x] << '~';
    }

    for(int x = 0; x < 10; x++)
    {
        File << HighScores[x] << '~';
    }

    std::cout << par << std::endl;
    File << par << '~';

    File.close();
}


void GP_TestLevel()
{
    GP_SaveLevel();
    GP_EditorUnload();
    InitLevel();
}

void GP_SelTool(int x)
{
    if(Sel_tool != -1)
    {
        Unstack_Texture(GP_E_TxtToolStack, tools[Sel_tool]);
    }
    Sel_tool = x;
    Stack_Text(GP_E_TxtToolStack, tools[Sel_tool], 1715, 657);
}


void B_LV_view()
{
    LV_VIEW = !LV_VIEW;
    Update_brick();
}


void GP_ExitEditor()
{
    GP_EditorUnload();
    GP_Unload();
    setmode = -1;
    LE_Load();
}

void GP_B_Settings()
{
    setmode = EDIT;
    GP_EditorUnload();
    GP_Unload();
    LE_Load();
}

//Game Logic
//
//
//

void LeftArrow();
void RightArrow();
void CheckBlobs();
void loopingcode();
void Winner();




int blobsfalling = 0;
bool RotatingLeft = false;
bool RotatingRight = false;

TX_STACK *GP_LevelStack;

TX_STACK *GP_LevelTxtStack;

B_STACK *GP_LevelBStack;

const double dropratei = 6;
double droprate = dropratei;
const double acceleration = 1.5;
const double turnrate = 90. / 60.;
void Exit_Level();
void GameOver();
void Add_HighScore();
void B_Exit_Level();
void Update_ScoreText();

SDL_Texture *GameMessages[7];
SDL_Texture *Game_Foreground[3];

int Score;

void Input_Check(std::string);
void Lvl_Restart();

void InitLevel()
{
    Score = 0;
    Update_GPLevelData();

    GP_LevelStack = new TX_STACK;
    Stack_Stack(GP_LevelStack);

    GP_LevelTxtStack = new TX_STACK;
    Stack_TStack(GP_LevelTxtStack);

    GP_LevelBStack = new B_STACK;
    Stack_BStack(GP_LevelBStack);

    GameMessages[0] = LoadText("Level Complete", 20, "GP: Winner text");
    GameMessages[1] = LoadText("GAME OVER: MOVE LIMIT REACHED", 36, "GP: Gameover text");
    GameMessages[2] = LoadText("Error: No Blobs", 30, "GP: No Blobs Text");
    GameMessages[3] = LoadText("EXIT", 26, "GP: Exit level button");
    GameMessages[4] = LoadText("RESTART", 26, "GP: Restart level button");


    Stack_Text(GP_LevelTxtStack, GameMessages[4], 1780, 889);
    Stack_Text(GP_LevelTxtStack, GameMessages[3], 1780, 1030);

    Scanlevel();
    if(Blobs.size() < 1)
    {
        Stack_Text(GP_LevelTxtStack, GameMessages[2], 960, 540);
    }
    else
    {
        CheckBlobs();

        if(blobsfalling == 0)
        {
            Stack_Key(SDLK_LEFT, LeftArrow);
            Stack_Key(SDLK_RIGHT, RightArrow);
        }

        Stack_Function(loopingcode);
    }

    Game_Foreground[0] = LoadTexture("in-game-1.png", "Game: Foreground");
    Game_Foreground[1] = LoadTexture("in-game-2.png", "Game: Foreground");
    Game_Foreground[2] = LoadTexture("in-game-3.png", "Game: Foreground");


    Stack_Texture(GP_LevelStack, Game_Foreground[0], 0, 0);
    Stack_Texture(GP_LevelStack, Game_Foreground[1], 466, 0);
    Stack_Texture(GP_LevelStack, Game_Foreground[1], 466, 1033);
    Stack_Texture(GP_LevelStack, Game_Foreground[2], 1455, 0);

    std::stringstream swag;
    std::string swaggot;
    swag << par;
    swag >> swaggot;


    Lvl_Info[2] = LoadText("Move Limit: " + swaggot, 18, "GP: Lvl Info");
    Stack_Text(GP_LevelTxtStack, Lvl_Info[2], 85, 174, false, false);



    Update_ScoreText();

    Score = 0;

    Stack_Button(GP_LevelBStack, 1645, 835, 275, 117, Lvl_Restart);
    Stack_Button(GP_LevelBStack, 1645, 963, 275, 117, B_Exit_Level);
}


void Exit_Level()
{
    Unstack_Stack(GP_LevelStack);
    Unstack_BStack(GP_LevelBStack);

    Unstack_TStack(GP_LevelTxtStack);

    Unstack_Function(loopingcode);

    RemoveTexture(Game_Foreground[0], "Game: Foreground");
    RemoveTexture(Game_Foreground[1], "Game: Foreground");
    RemoveTexture(Game_Foreground[2], "Game: Foreground");

    RemoveTexture(Lvl_Info[1], "GP: Lvl Info");
    RemoveTexture(Lvl_Info[2], "GP: Lvl Info");

    RemoveTexture(GameMessages[0], "GP: Winner text");
    RemoveTexture(GameMessages[1],"GP: Gameover text");
    RemoveTexture(GameMessages[2],"GP: No Blobs Text");
    RemoveTexture(GameMessages[3], "GP: Exit level button");
    RemoveTexture(GameMessages[4], "GP: Restart level button");

    Change_Angle(GP_TxStack, brick, 0);

    Blobs.clear();

    Gravity = 0;
    pos = 0;
    screenangle = 0;
    droprate = dropratei;
}


void B_Exit_Level()
{
    Exit_Level();
    GP_Unload();
    if(setmode == PLAY)
    {
        LM_Load();
    }
    else
    {
        GP_Load(EDIT);
    }
}


void Scanlevel()
{
    Blobs.clear();
    for(int x = 0; x < 11; x++)
    {
        for(int y = 0; y < 11; y++)
        {
            if(GPLevelData[x][y] == BLOB)
            {
                Blob temp;
                temp.UID = x;
                temp.posy = y;
                temp.posx = x;
                temp.tx = LevelDatatx[(x * 11) + y];
                Blobs.push_back(temp);
            }
        }
    }

}



void loopingcode()
{
    if(blobsfalling > 0)
    {
        droprate += acceleration;
        pos += droprate;
        if( pos >= 90)
        {
            for(int x = 0; x < Blobs.size(); x++)
            {
                if(Blobs[x].isfalling)
                {
                    switch (Gravity)
                    {
                        case 0:
                            //GPLevelDatatx[Blobs[x].posx][Blobs[x].posy] = 0; //Block it's currently in
                            //GPLevelDatatx[Blobs[x].posx][Blobs[x].posy + 1] = Blobs[x].tx; //Block it's falling into
                            GPLevelData[Blobs[x].posx][Blobs[x].posy + 1] = BLOB;
                            break;
                        case 1:
                            //GPLevelDatatx[Blobs[x].posx][Blobs[x].posy] = 0;
                            //GPLevelDatatx[Blobs[x].posx - 1][Blobs[x].posy] = Blobs[x].tx;
                            GPLevelData[Blobs[x].posx - 1][Blobs[x].posy] = BLOB;
                            break;
                        case 2:
                            //GPLevelDatatx[Blobs[x].posx][Blobs[x].posy] = 0;
                            //GPLevelDatatx[Blobs[x].posx][Blobs[x].posy - 1] = Blobs[x].tx;
                            GPLevelData[Blobs[x].posx][Blobs[x].posy - 1] = BLOB;
                            break;
                        case 3:
                            //GPLevelDatatx[Blobs[x].posx][Blobs[x].posy] = 0;
                            //GPLevelDatatx[Blobs[x].posx + 1][Blobs[x].posy] = Blobs[x].tx;
                            GPLevelData[Blobs[x].posx + 1][Blobs[x].posy] = BLOB;
                            break;
                    }

                    GPLevelData[Blobs[x].posx][Blobs[x].posy] = EMPTY;

                    switch (Gravity)
                    {
                        case 0:
                            Blobs.at(x).posy++;
                            break;
                        case 1:
                            Blobs.at(x).posx--;
                            break;
                        case 2:
                            Blobs.at(x).posy--;
                            break;
                        case 3:
                            Blobs.at(x).posx++;
                            break;
                    }
                }

            }

            for(int x = 0; x < Blobs.size(); x++)
            {
                if(Blobs[x].toDelete == true)
                {
                    GPLevelData[Blobs[x].posx][Blobs[x].posy] = FINISH;
                    //GPLevelDatatx[Blobs[x].posx][Blobs[x].posy] =
                    Blobs.erase(Blobs.begin() + x);
                }
            }



            pos = 0;
            Update_txmap();
            CheckBlobs();

            if(blobsfalling == 0)
            {
                Stack_Key(SDLK_LEFT, LeftArrow);
                Stack_Key(SDLK_RIGHT, RightArrow);
                droprate = dropratei;
                if(par < Score && Blobs.size() != 0)
                {
                    GameOver();
                }
            }
        }
    }


    if(RotatingLeft || RotatingRight)
    {
        if(RotatingLeft)
            screenangle -= turnrate;

        if(RotatingRight)
            screenangle += turnrate;


    if((int)(screenangle) % 90 == 0 || screenangle == 0)
    {
        RotatingLeft = false;
        RotatingRight = false;
        CheckBlobs();
        if(blobsfalling == 0)
        {
            Stack_Key(SDLK_LEFT, LeftArrow);
            Stack_Key(SDLK_RIGHT, RightArrow);
        }

    }

    Change_Angle(GP_TxStack, brick, screenangle);
    }

    Update_brick();
}


void LeftArrow()
{

    if(++Gravity > 3)
    {
        Gravity = 0;
    }

    RotatingLeft = true;

    Score++;


    Unstack_Key(SDLK_LEFT);
    Unstack_Key(SDLK_RIGHT);
    Update_ScoreText();
}


void RightArrow()
{

    if(--Gravity < 0)
    {
        Gravity = 3;
    }

    RotatingRight = true;

    Score++;


    Unstack_Key(SDLK_LEFT);
    Unstack_Key(SDLK_RIGHT);
    Update_ScoreText();
}


void CheckBlobs()
{
    int check;

    if(Blobs.size() == 0)
    {
        Winner();
        return;
    }

    blobsfalling = 0;

    for(int x = 0; x < Blobs.size(); x++)
    {
        bool redflag = false;


        switch (Gravity)
        {
            case 0:
                if(Blobs[x].posy + 1 >= 11)
                {
                    Blobs[x].isfalling = false;
                    redflag = true;
                    break;
                }

                check = GPLevelData[Blobs[x].posx][Blobs[x].posy + 1];
                break;

            case 1:
                if(Blobs[x].posx - 1 < 0)
                {
                    Blobs[x].isfalling = false;
                    redflag = true;
                    break;
                }

                check = GPLevelData[Blobs[x].posx - 1][Blobs[x].posy];
                break;

            case 2:
                if(Blobs[x].posy - 1 < 0)
                {
                    Blobs[x].isfalling = false;
                    redflag = true;
                    break;
                }

                check = GPLevelData[Blobs[x].posx][Blobs[x].posy - 1];
                break;

            case 3:
                if(Blobs[x].posx + 1 >= 11)
                {
                    Blobs[x].isfalling = false;
                    redflag = true;
                    break;
                }

                check = GPLevelData[Blobs[x].posx + 1][Blobs[x].posy];
                break;
        }

        if(redflag == false)
        {
            switch(check)
            {
                case EMPTY:
                    Blobs[x].isfalling = true;
                    blobsfalling++;
                    break;
                case BLOB:
                    Blobs[x].isfalling = false;
                    break;
                case BLOCK:
                    Blobs[x].isfalling = false;
                    break;
                case FINISH:
                    Blobs[x].isfalling = true;
                    Blobs[x].toDelete = true;
                    blobsfalling++;
                    break;
            }
        }
    }
}


void GameOver()
{
    Stack_Text(GP_LevelTxtStack, GameMessages[1], 960, 540, true, true);
    Unstack_Function(loopingcode);
    Unstack_Key(SDLK_LEFT);
    Unstack_Key(SDLK_RIGHT);
}


void Winner()
{
    Starttext = "WINNER";
    Unstack_Function(loopingcode);
    Add_HighScore();
    Unstack_Key(SDLK_LEFT);
    Unstack_Key(SDLK_RIGHT);
    Exit_Level();
    GP_Unload();
}

int index;
void Swaglet();

void Add_HighScore()
{
    index = 11;
    bool flag = false;
    for(int x = 9; x >= 0; x--)
    {
        if(HighScores[x] > Score)
        {
            index = x;
        }
    }

    if(index < 10)
    {
        TextInput(Input_Check);
    }
    else
    {
        if(setmode == EDIT)
            Display_HighScores(Swaglet);
        else
            Display_HighScores(LM_Load);
    }

    return;
}


void Input_Check(std::string Scorer)
{
    HighScorers.insert(HighScorers.begin() + index, Scorer);
    HighScorers.pop_back();
    HighScores.insert(HighScores.begin() + index, Score);
    HighScores.pop_back();

    GP_SaveLevel();


    if(setmode == EDIT)
        Display_HighScores(Swaglet);
    else
        Display_HighScores(LM_Load);
    return;
}


void Update_ScoreText()
{
    if(Lvl_Info != NULL)
    {
        Unstack_Texture(GP_LevelTxtStack, Lvl_Info[1]);
    }

    std::stringstream swag;
    std::string swaggot;
    swag << Score;
    swag >> swaggot;

    Lvl_Info[1] = LoadText("Score: " + swaggot, 18, "Lvl_Info");

    Stack_Text(GP_LevelTxtStack, Lvl_Info[1], 85, 124, false, false);
}


void Lvl_Restart()
{
    Score = 0;
    Exit_Level();
    InitLevel();
}


void Swaglet()
{
    GP_Load(EDIT);
}

