#include "../Headers/Global.h"
#include <iostream>

SDL_Texture *TextInputTx[7];

TX_STACK *TITXStack;
TX_STACK *TITXTStack;
B_STACK *TIBStack;
std::string Returntext;
std::string Starttext;
void B_Backspace();

bool TI_FLAG = false;

void(*TIfunct)(std::string);


void B_SuperMassiveButton();
void Unload_TextInput();
void B_TIExit();

void TextInput(void(*funct)(std::string))
{
    {// Initialize Stacks
        TIBStack = new B_STACK;
        Stack_BStack(TIBStack);

        TITXStack = new TX_STACK;
        Stack_Stack(TITXStack);

        TITXTStack = new TX_STACK;
        Stack_TStack(TITXTStack);
    }

    Returntext = Starttext;

    TIfunct = funct;

    SDL_StartTextInput();

    TI_FLAG = true;

    TextInputTx[0] = LoadTexture("TextInput.png", "TextInput: Background");
    TextInputTx[1] = LoadText(Starttext, 26, "Text Input: Text");

    Stack_Texture(TITXStack, TextInputTx[0]);

    Stack_Button(TIBStack, 0, 0, 1920, 1080, B_SuperMassiveButton);
    Stack_Button(TIBStack, 825, 580, 281, 109, B_TIExit);

    Stack_Key(SDLK_BACKSPACE, B_Backspace);

    TI_Update_Text();

    Exit_Sequence_Add(Unload_TextInput);
    return;
}


void Unload_TextInput()
{
    RemoveTexture(TextInputTx[0], "TextInput: Background");
    RemoveTexture(TextInputTx[1], "Text Input: Text");

    Unstack_Stack(TITXStack);
    Unstack_TStack(TITXTStack);
    Unstack_BStack(TIBStack);

    Unstack_Key(SDLK_BACKSPACE);

    SDL_StopTextInput();

    TI_FLAG = false;
    Exit_Sequence_Remove(Unload_TextInput);
}


void B_TIExit()
{
    Unload_TextInput();
    TIfunct(Returntext);
}


void B_SuperMassiveButton()
{
    std::cout << "Super Massive Button PRESSED!\n";
}


void B_Backspace()
{
    if(Returntext.length() > 0)
    {
        Returntext.pop_back();
        TI_Update_Text();
    }
    return;
}


void TI_Update_Text()
{
    Unstack_Texture(TITXStack, TextInputTx[1]);
    RemoveTexture(TextInputTx[1], "TextInput: Text");
    TextInputTx[1] = LoadText(Returntext, 26, "TextInput: Text");
    Stack_Text(TITXTStack, TextInputTx[1], 960, 411);
}
