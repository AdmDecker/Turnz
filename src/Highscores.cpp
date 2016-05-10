#include "../Headers/Global.h"
#include <sstream>

extern std::vector<std::string> HighScorers;
extern std::vector<int> HighScores;

TX_STACK *HS_Stack;
TX_STACK *HS_TStack;
B_STACK *HS_BStack;

SDL_Texture *HS_Tx[10];
void(*bailfunction)(void);
void Exit_HS();

std::vector<SDL_Texture*> HS_Scorers;
std::vector<SDL_Texture*> HS_Scores;

extern int setmode;

void B_Dummy();


void Display_HighScores(void(*funct)(void))
{
    Load_Level();


    {// Initialize Stacks
        HS_BStack = new B_STACK;
        Stack_BStack(HS_BStack);

        HS_TStack = new TX_STACK;
        Stack_TStack(HS_TStack);

        HS_Stack = new TX_STACK;
        Stack_Stack(HS_Stack);
    }


    HS_Tx[0] = LoadTexture("HighScores.png", "TextInput: Background");
    HS_Tx[1] = LoadText("EXIT", 26, "HS: Exit");

    Stack_Texture(HS_Stack, HS_Tx[0]);
    Stack_Text(HS_TStack, HS_Tx[1], 975, 977);

    int y = 0;
    int z = 0;

    for(int x = 0; x < HighScorers.size(); x++)
    {
        std::stringstream swag;
        std::string swag2;
        swag << x + 1;
        swag >> swag2;
        HS_Scorers.push_back(LoadText(swag2 + ". " + HighScorers[x], 26, "HS: Names"));
        Stack_Text(HS_TStack, HS_Scorers[x], 800 * z + 230, (y++ + 1) * 100 + 100, false, false);
        if(y == 5)
        {
            y = 0;
            z = 1;
        }

    }

    y = 0;
    z = 0;
    for(int x = 0; x < HighScores.size(); x++)
    {
        std::stringstream swag;
        swag << HighScores.at(x);
        std::string swag2;
        swag >> swag2;
        HS_Scores.push_back(LoadText(swag2, 26, "HS: Score"));
        Stack_Text(HS_TStack, HS_Scores[x], z * 400 + 800, (y++ + 1) * 100 + 100, false, false);
        if(y == 5)
        {
            y = 0;
            z = 2;
        }

    }

    bailfunction = funct;

    Stack_Button(HS_BStack, 837, 926, 269, 108, Exit_HS);
}


void Exit_HS()
{
    Unstack_BStack(HS_BStack);
    Unstack_Stack(HS_Stack);
    Unstack_TStack(HS_TStack);

    RemoveTexture(HS_Tx[0], "TextInput: Background");
    RemoveTexture(HS_Tx[1], "HS: Exit");

    for(int x = 0; x < HS_Scores.size(); x++)
    {
        RemoveTexture(HS_Scores[x], "HS: Score");
    }

    HS_Scorers.clear();
    HS_Scores.clear();

    bailfunction();
}


void B_Dummy()
{
    return;
}
