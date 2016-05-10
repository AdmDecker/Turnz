#include "../Headers/Global.h"
#include "../Headers/States.h"
#include <vector>
#include <fstream>

#define USER 0
#define GAME 1

extern const int dis_LEVELS_MAX = 7;
std::vector<Level> UserLevels;
std::vector<Level> GameLevels;
SDL_Texture *Names[dis_LEVELS_MAX];

int LM_Level_Page = 0;
int LM_type = GAME;
int LM_Sel_Level = -1;
extern int par;

void DisplayLevels(void);
void B_LM_RightArrow(void);
void B_LM_LeftArrow(void);
void LM_SelLevel(int);
void B_LM_Play();
void B_LM_Exit();
void LM_Unload();
void B_LM_Switch();

TX_STACK *LM_LabelsStack;
TX_STACK *LM_MainStack;
TX_STACK *LM_TStack;

B_STACK *LM_MainBStack;
B_STACK *LM_LevelBStack;

SDL_Texture *LM_textures[10];

extern std::string Lvl_name;
extern int setmode;
extern int loadmode;
void B_LM_HighScore();

void LM_Load()
{
    { //Initialize Stacks
        LM_LabelsStack = new TX_STACK;
        Stack_TStack(LM_LabelsStack);

        LM_TStack = new TX_STACK;
        Stack_TStack(LM_TStack);

        LM_MainStack = new TX_STACK;
        Stack_Stack(LM_MainStack);

        LM_MainBStack = new B_STACK;
        Stack_BStack(LM_MainBStack);

        LM_LevelBStack = new B_STACK;
        Stack_BStack(LM_LevelBStack);
    }


    { //Load Textures
        LM_textures[0] = LoadTexture("Main-Levels.png", "LM Background");
        LM_textures[6] = LoadTexture("User-Levels.png", "LM Background");
        LM_textures[1] = LoadText("PLAY", 26, "LM Text");
        LM_textures[2] = LoadText("SCORES", 26, "LM Text");
        LM_textures[3] = LoadText("LEVELS", 26, "LM Text");
        LM_textures[4] = LoadText("BACK", 26, "LM Text");
        LM_textures[5] = LoadTexture("Highlight.png", "LM: Highlight");
    }


    { //Stack Textures
        Stack_Texture(LM_MainStack, LM_textures[0]); //Background
        Stack_Text(LM_TStack, LM_textures[2], 446, 1005);
        Stack_Text(LM_TStack, LM_textures[1], 966, 1005);
        Stack_Text(LM_TStack, LM_textures[3], 1476, 1005);
        Stack_Text(LM_TStack, LM_textures[4], 125, 57);
    }


    { //Stack Buttons
        Stack_Button(LM_MainBStack, 125, 515, 115, 140, B_LM_LeftArrow);
        Stack_Button(LM_MainBStack, 1680, 510, 115, 145, B_LM_RightArrow);

        Stack_Button(LM_MainBStack, 836, 951, 250, 115, B_LM_Play);
        Stack_Button(LM_MainBStack, 0, 0, 250, 115, B_LM_Exit);
        Stack_Button(LM_MainBStack, 332, 951, 301, 111, B_LM_HighScore);
        Stack_Button(LM_MainBStack, 1285, 951, 301, 111, B_LM_Switch);
    }


    { //Load Levels
        std::ifstream File("User/Level Editor/Levels.txt");
        std::string temp;
        while(getline(File, temp))
        {
            if(temp != "")
            {
                Level templevel;
                templevel.name = temp;
                UserLevels.push_back(templevel);
            }
        }

        File.close();

        File.open("resources/Levels/Levels.txt");

        while(getline(File, temp))
        {
            if(temp != "")
            {
                Level templevel;
                templevel.name = temp;
                GameLevels.push_back(templevel);
            }
        }
        File.close();

        for(int x = 0; x < GameLevels.size(); x++)
        {
            GameLevels[x].pos = x;
        }

        for(int x = 0; x < UserLevels.size(); x++)
        {
            UserLevels[x].pos = x;
        }
    }


    DisplayLevels();
}


void B_LM_Exit()
{
    LM_Unload();
    MM_Load();
}


void LM_Unload()
{
    { //Unload Textures
        RemoveTexture(LM_textures[0], "LM Background");
        RemoveTexture(LM_textures[1], "LM Text");
        RemoveTexture(LM_textures[2], "LM Text");
        RemoveTexture(LM_textures[3], "LM Text");
        RemoveTexture(LM_textures[4], "LM Text");
        RemoveTexture(LM_textures[5], "LM: Highlight");
        RemoveTexture(LM_textures[6], "LM Background");

        for(int x = 0; x < dis_LEVELS_MAX && Names[x] != NULL; x++)
        {
            RemoveTexture(Names[x], "LM: Level Labels");
            Names[x] = NULL;
        }
    }

    { //Unstack Stacks
        Unstack_TStack(LM_LabelsStack);
        Unstack_Stack(LM_MainStack);
        Unstack_BStack(LM_MainBStack);
        Unstack_BStack(LM_LevelBStack);
        Unstack_TStack(LM_TStack);
    }


    UserLevels.clear();
    GameLevels.clear();
    LM_Sel_Level = -1;
    LM_type = GAME;

}


void DisplayLevels(void)
{
    Clear_Tx_Stack(LM_LabelsStack);

    Clear_BStack(LM_LevelBStack);

    for(int x = 0; x < dis_LEVELS_MAX && Names[x] != NULL; x++)
    {
        RemoveTexture(Names[x], "LM: Level Labels");
        Names[x] = NULL;
    }


    if(LM_type == USER) //Create Level Name Textures
    {
        for(int x = 0; x < dis_LEVELS_MAX && x + (dis_LEVELS_MAX * LM_Level_Page) < UserLevels.size(); x++)
        {
            Names[x] = LoadText(UserLevels.at(x + LM_Level_Page * dis_LEVELS_MAX).name, 26, "LM: Level Labels");
        }

        //Stack Highlighter Buttons
        for(int x = LM_Level_Page * dis_LEVELS_MAX; x < UserLevels.size() && x < dis_LEVELS_MAX + (LM_Level_Page * dis_LEVELS_MAX); x++)
        {
            Stack_Button(LM_LevelBStack, 360, ((x - (LM_Level_Page * dis_LEVELS_MAX)) + 1) * 100 + 120, 900, 100, LM_SelLevel, x);
        }
    }

    if(LM_type == GAME) //Create Level Name Textures
    {
        for(int x = 0; x < dis_LEVELS_MAX && x + (dis_LEVELS_MAX * LM_Level_Page) < GameLevels.size(); x++)
        {
            Names[x] = LoadText(GameLevels.at(x + LM_Level_Page * dis_LEVELS_MAX).name, 26, "LM: Level Labels");
        }

        for(int x = LM_Level_Page * dis_LEVELS_MAX; x < GameLevels.size() && x < dis_LEVELS_MAX + (LM_Level_Page * dis_LEVELS_MAX); x++)
        {
            Stack_Button(LM_LevelBStack, 360, ((x - (LM_Level_Page * dis_LEVELS_MAX)) + 1) * 100 + 120, 900, 100, LM_SelLevel, (x - (LM_Level_Page * dis_LEVELS_MAX)));
        }
    }

    int z = 0;
    //Stack Textures
    for(int x = LM_Level_Page * dis_LEVELS_MAX; x < dis_LEVELS_MAX + (LM_Level_Page * dis_LEVELS_MAX) && Names[x] != NULL; x++)
    {
        Stack_Text(LM_LabelsStack, Names[z++], 960,((x - (LM_Level_Page * dis_LEVELS_MAX)) + 1) * 100 + 120, true, false);
    }

    return;
}


void B_LM_RightArrow(void)
{
    if(LM_type == GAME)
    {
        if(LM_Level_Page < GameLevels.size() / dis_LEVELS_MAX)
            LM_Level_Page++;
    }

    if(LM_type == USER)
    {
        if(LM_Level_Page < UserLevels.size() / dis_LEVELS_MAX)
            LM_Level_Page++;
    }

    DisplayLevels();
    LM_SelLevel(-1);
}


void B_LM_LeftArrow(void)
{
    if(--LM_Level_Page < 0)
        LM_Level_Page = 0;

    DisplayLevels();
    LM_SelLevel(-1);
}


void LM_SelLevel(int x)
{
    if(LM_Sel_Level >= 0)
        Unstack_Texture(LM_MainStack, LM_textures[5]);
    if(x >= 0)
    {
        LM_Sel_Level = x + 1;
        Stack_Texture(LM_MainStack, LM_textures[5], 310, (x+1) * 100 + 125);
    }
}


void B_LM_Play()
{
    if(LM_Sel_Level > -1)
    {
        par = 0;
        if(LM_type == USER)
        {
            Lvl_name = UserLevels.at((LM_Level_Page * dis_LEVELS_MAX) + LM_Sel_Level - 1).name;
            loadmode = EDIT;
        }
        else if(LM_type == GAME)
        {
            Lvl_name = GameLevels.at((LM_Level_Page * dis_LEVELS_MAX) + LM_Sel_Level - 1).name;
            loadmode = PLAY;
        }
        LM_Unload();
        GP_Load(PLAY);
    }
}


void B_LM_HighScore()
{
    if(LM_Sel_Level > -1)
    {
        if(LM_type == USER)
        {
            Lvl_name = UserLevels.at((LM_Level_Page * dis_LEVELS_MAX) + LM_Sel_Level - 1).name;
            loadmode = EDIT;
        }
        else if(LM_type == GAME)
        {
            Lvl_name = GameLevels.at((LM_Level_Page * dis_LEVELS_MAX) + LM_Sel_Level - 1).name;
            loadmode = PLAY;
        }

        LM_Unload();

        Display_HighScores(LM_Load);
    }
}


void B_LM_Switch()
{
    if(LM_type == USER)
    {
        Unstack_Texture(LM_MainStack, LM_textures[6]);
        Stack_Texture(LM_MainStack, LM_textures[0]);
    }
    else
    {
        Unstack_Texture(LM_MainStack, LM_textures[0]);
        Stack_Texture(LM_MainStack, LM_textures[6]);
    }
    LM_type = !LM_type;
    LM_Sel_Level = -1;

    DisplayLevels();
}
