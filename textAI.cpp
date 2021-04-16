#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

#define N 1024
#define N_WORD 32
#define TOTAL_WORDS 256
#define SHINGLE 3
#define TO_BE_REMOVED "чтд кг км м г с а к но и да не или либо же а что чтобы как так, и т.д., и пр., и др. " 
#define SEPARATORS "., !:-Ч;"
#define ENG_LETTERS "AaBCcEeHKkMmOoPpTXxYy"
#define RUS_LETTERS "ја¬—с≈еЌ кћмќо–р≈’х”у"
#define RUS_UPP_TO_LOW_CASE 32

bool isSeparator(char c);
bool isEngLetter(char c);
int strLen(char str[]);
void strCat(char str1[], char str2[]);
void strCpy(char str1[], char str2[]);
void strCpyStrToChar(char str1[], string str2);
int strCmp(char str1[], char str2[]);
void emptyTextArray(char textSplitted[][N_WORD], int numberOfRaws);
void splitText(char text[], char textSplitted[][N_WORD], char wordsToBeRemoved[][N_WORD]);
void createWordsToBeRemoved(char toBeRemoved[], char wordsToBeRemoved[][N_WORD]);
void replaseEngLetters(char textAr[]);
void replaceUpperCase(char textAr[]);
bool isWordForTransfer(char word[]);
bool isNumber(char word[]);
bool isToBeRemoved(char word[], char wordsToBeRemoved[][N_WORD]);
int calcTextUnits(char textSplitted[][N_WORD]);
double calcPlagiatRate(char textSplitted[][N_WORD], char fragmentSplitted[][N_WORD]);

int main()
{
    SetConsoleCP(1251); 
    SetConsoleOutputCP(1251); 
    string text = "aЅ и пр. 2 cCB√ ƒ≈ »  Ћћ Ќќ либо", fragment = "¬√ ƒ≈ »  Ћћ";

    char textAr[N], fragmentAr[N], toBeRemoved[] = TO_BE_REMOVED;
    char textSplitted[TOTAL_WORDS][N_WORD], fragmentSplitted[TOTAL_WORDS][N_WORD], wordsToBeRemoved[TOTAL_WORDS][N_WORD];
    int textUnits = 0, fragmentUnits = 0, toRemoveUnits = 0;

    strCpyStrToChar(textAr, text);
    strCpyStrToChar(fragmentAr, fragment);

    replaseEngLetters(textAr);
    replaceUpperCase(textAr);
    replaseEngLetters(fragmentAr);
    replaceUpperCase(fragmentAr);
    
    emptyTextArray(textSplitted, TOTAL_WORDS);
    emptyTextArray(fragmentSplitted, TOTAL_WORDS);
    emptyTextArray(wordsToBeRemoved, TOTAL_WORDS);

    createWordsToBeRemoved(toBeRemoved, wordsToBeRemoved);
    splitText(textAr, textSplitted, wordsToBeRemoved);
    splitText(fragmentAr, fragmentSplitted, wordsToBeRemoved);
    
  

    cout << "Text: " << textUnits << "; Fragment: " << fragmentUnits << endl;

    calcPlagiatRate(textSplitted, fragmentSplitted);

    
    for (int i = 0; i < 10; i++)
    {
        cout << i + 1 << ") \"" << textSplitted[i] << "\"" << endl;
    }

    

    return 0;
}

bool isSeparator(char c)
{
    char separator[] = SEPARATORS;

    for (int i = 0; separator[i] != '\0'; i++)
        if (c == separator[i])
            return true;
    return false;
}

bool isEngLetter(char c)
{
    return ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z');
}

int strLen(char str[])
{
    int len = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        len++;
    }

    return len;
}

void strCat(char str1[], char str2[])
{
    int len1 = strLen(str1);
    int i;

    for (i = 0; str2[i] != '\0'; i++)
    {
        str1[len1 + i] = str2[i];
    }

    str1[len1 + i] = '\0';
}

void strCpy(char str1[], char str2[])
{
    int i;

    for (i = 0; str2[i] != '\0'; i++)
    {
        str1[i] = str2[i];
    }

    str1[i] = '\0';
}

void strCpyStrToChar(char str1[], string str2)
{
    int i;

    for (i = 0; str2[i] != '\0'; i++)
    {
        str1[i] = str2[i];
    }

    str1[i] = '\0';
}

int strCmp(char str1[], char str2[])
{
    int i = 0;

    for (i = 0; ; i++)
    {
        if (str1[i] > str2[i])
            return 1;

        if (str1[i] < str2[i])
            return -1;

        if (str1[i] == '\0' and str2[i] == '\0')
            return 0;
    }
}

void emptyTextArray(char textSplitted[][N_WORD], int numberOfRaws)
{
    for (int i = 0; i < numberOfRaws; i++)
        textSplitted[i][0] = '\0';
}

void splitText(char text[], char textSplitted[][N_WORD], char wordsToBeRemoved[][N_WORD])
{
    char word[N] = "";
    int i = 0, iw = 0, iSp = 0;

    for (i = 0; text[i] != '\0'; i++) {
        if (!isSeparator(text[i])) {
            word[iw] = text[i];
            iw++;
            if (isSeparator(text[i + 1]) or text[i + 1] == '\0') {
                word[iw] = '\0';
                iw = 0;
                if (word[0] != '\0' and !isNumber(word) and !isToBeRemoved(word, wordsToBeRemoved)) {
                    strCpy(textSplitted[iSp], word);
                    iSp++;
                }
                word[0] = '\0'; //?
            }
        }
    }
}

void createWordsToBeRemoved(char toBeRemoved[], char wordsToBeRemoved[][N_WORD])
{
    char word[N_WORD] = "";
    int i = 0, iw = 0, iSp = 0;

    for (i = 0; toBeRemoved[i] != '\0'; i++) {
        if (!isSeparator(toBeRemoved[i])) {
            word[iw] = toBeRemoved[i];
            iw++;
            if (isSeparator(toBeRemoved[i + 1]) or toBeRemoved[i + 1] == '\0') {
                word[iw] = '\0';
                iw = 0;
                strCpy(wordsToBeRemoved[iSp], word);
                iSp++;
            }
        }
    }
}

void replaseEngLetters(char textAr[])
{
    char engLetters[] = ENG_LETTERS;
    char rusLetters[] = RUS_LETTERS;
    int i, j;

    for (i = 0; textAr[i] != '\0'; i++)
    {
        if (isEngLetter(textAr[i]))
        {
            for (j = 0; engLetters[j] != '\0'; j++)
            {
                if (textAr[i] == engLetters[j])
                {
                    textAr[i] = rusLetters[j];
                    break;
                }
            }
        }
    }
}

void replaceUpperCase(char textAr[])
{
    for (int i = 0; textAr[i] != '\0'; i++)
    {
        if ('ј' <= textAr[i] and textAr[i] <= 'я')
        {
            textAr[i] = textAr[i] + RUS_UPP_TO_LOW_CASE;
        }
    }
}

bool isWordForTransfer(char word[])
{
    return word[0] != '\0' and !isNumber(word);
}

bool isNumber(char word[])
{
    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] <= '0' or word[i] >= '9')
            return false;
    }

    return true;
}

bool isToBeRemoved(char word[], char wordsToBeRemoved[][N_WORD])
{
    int numberOfWordsToRemove = calcTextUnits(wordsToBeRemoved);

    for (int i = 0; i < numberOfWordsToRemove; i++)
        if (strCmp(word, wordsToBeRemoved[i]) == 0)
            return true;
    
    return false;
}

int calcTextUnits(char textSplitted[][N_WORD])
{
    int textUnits = 0;
    for (textUnits = 0; textSplitted[textUnits][0] != '\0'; textUnits++) {}

    return textUnits;
}

double calcPlagiatRate(char textSplitted[][N_WORD], char fragmentSplitted[][N_WORD])
{
    int shinglesTotal = 0, shinglesEquial = 0;
    double plagiat;
    bool isEquial;
    int iFr, iText, i;
    int textUnits = calcTextUnits(textSplitted);
    int fragmentUnits = calcTextUnits(fragmentSplitted);

    for (iFr = 0; iFr <= fragmentUnits - SHINGLE; iFr++)
    {
        for (iText = 0; iText <= textUnits - SHINGLE; iText++)
        {
            isEquial = true;

            for (i = 0; i < SHINGLE; i++)
            {
                if (strCmp(fragmentSplitted[iFr + i], textSplitted[iText + i]) != 0)
                {
                    isEquial = false;
                    break;
                }
            }

            if (isEquial)
            {
                shinglesEquial++;
                break;
            }
        }

        shinglesTotal++;
    }

    plagiat = (double)shinglesEquial / (double)shinglesTotal;

    cout << "Total: " << shinglesTotal << endl;
    cout << "Eq: " << shinglesEquial << endl;
    cout << "Pl: " << plagiat << endl;

    return plagiat;
}
