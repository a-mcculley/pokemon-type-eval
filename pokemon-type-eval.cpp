//Note (2024): This code was written in around 2018-2019 when I was far less skilled. As a result this code is very sloppy; it solves the problem in a brute force manner, and exhibits poor style. There are almost certainly more efficient methods. 
//I would approach the problem differently if I were to do it again. This is not representative of my current skill level.
//This code is, at worst, O(n^6) where n is the number of types; however, pokemon types are effectively a constant 18. 18 is a small enough number
//That this is acceptably fast. Additionally, since this input is always constant, its realistically O(1) in practice (and if one or two more types were added, it would still be acceptably fast). Additionally, the speed is acceptable since this was only ever intended to run once.
//There are multiple directions this could be expanded in the future if I return to it; the offensive type combo with fewest resistances, the defensive combo with the fewest super effectives, a system that accounts for 4x super effectives/0.25 resistances/immunities, and most immunities total, are all angles off the top of my head.
//Additionally, this code doesn't handle ties in any way; the best defensive type combo (with 12 resistances + immunities) it outputs is normal/steel, but there are actually 3 that have 12: normal/steel, electric/steel, and ghost/steel. This code will end up just printing whichever it encounters first.

//The purpose of this code was to allow me to evaluate the effectiveness of various pokemon type combinations.
//In particular, the initial purpose was the combination of 4 types that would hit the most type combos super effectively.
//I then also added a mode for the combo of 2 types that would hit the most type combos super effectively (the optimal offensive typing)
//Finally, I decided to also add a mode that would output the type combo with the most defenses + immunities.

//Input file with the name "typeChart.txt" is of this exact format; line 1 is the number of types, line 2 is the type names in a space seperated format
//lines 3 through the end of file are the type matchups for a given type. Any specific line starts with the name of a [attacking] type, followed by a space,
//followed by its matchup with every other type. In particular, these matchups are shown in a sequence of chars, with . representing neutral, * representing super effective,
//- representing not very effective, and 0 representing immunity. These character strings have no spaces between characters, and the assumption is that the order is the same as the order
//the names were given on line 2; for example, if water is index 2 on line 2, then to see the effectiveness of electric on water, we would go to the line
//beginning with electric, then go to the character with index 2 among the effectiveness string (in this case we would see a star, as electric is super effective against water)

#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

struct pkmnType
{
    string name;
    int effectiveness = 0;
};//pkmnType

int main()
{
    ifstream inputFile("typeChart.txt");
    
    if(!inputFile.is_open())
    {
        cout << "File not opened";
        return 1;
    }//if

    unordered_map<string, vector<pkmnType>> attackers;
    int numTypes;
    inputFile >> numTypes;

    vector<pkmnType> defaultList;
    defaultList.reserve(numTypes);

    for(int i = 0; i < numTypes; i++)
    {
        string newWord;
        inputFile >> newWord;
        defaultList.push_back({newWord, 0});    
    }//for i

    for(int i = 0; i < numTypes; i++)
    {
        attackers[defaultList[i].name] = defaultList;
    }//for i

    for(int i = 0; i < numTypes; i++)
    {
        string curType;
        inputFile >> curType;
        vector<pkmnType>& curTypeList = attackers[curType];

        for(int j = 0; j < numTypes; j++)
        {
            char currEffective;
            inputFile >> currEffective;
            
            if(currEffective == '*')
            {
                curTypeList[j].effectiveness = 1;
            }//if
            else if (currEffective == '-')
            {
                curTypeList[j].effectiveness = -1;
            }//else if
            else if (currEffective == '0')
            {
                curTypeList[j].effectiveness = -4;
            }//else if
        }//for j
    }//for i

    cout << "Input 'a' to find the 4 type combination that hits the most type combos super effectively, 'b' for the 2 type combination that does the same, and 'c' for the 2 type combination with the highest number of resistances + immunities.\n";
    char mode;
    cin >> mode;

    if(mode == 'a')
    {
        vector<string> moveset(4);
        
        for(int i = 0; i < 4; i++)
        {
            moveset[i] = defaultList[i].name;
        }//for i

        vector<string> bestMoveset;
        int bestSuperEffective = -1;

        for(int a = 0; a < numTypes - 3; a++)
        {
            for(int b = a + 1; b < numTypes - 2; b++)
            {
                for(int c = b + 1; c < numTypes; c++)
                {
                    for(int d = c + 1; d < numTypes; d++)
                    {
                        moveset[0] = defaultList[a].name;
                        moveset[1] = defaultList[b].name;
                        moveset[2] = defaultList[c].name;
                        moveset[3] = defaultList[d].name;
                        vector<pkmnType>& typeComboA = attackers[moveset[0]];
                        vector<pkmnType>& typeComboB = attackers[moveset[1]];
                        vector<pkmnType>& typeComboC = attackers[moveset[2]];
                        vector<pkmnType>& typeComboD = attackers[moveset[3]];
                        int numSuperEffective = 0;
                        
                        for(int i = 0; i < numTypes; i++)
                        {
                            for(int j = i; j < numTypes; j++)
                            {
                                if(j == i)
                                {
                                    if(typeComboA[i].effectiveness > 0 || typeComboB[i].effectiveness > 0 || typeComboC[i].effectiveness > 0 || typeComboD[i].effectiveness > 0)
                                    {
                                        numSuperEffective++;

                                    }//if
                                }//if
                                else
                                {
                                    if(typeComboA[i].effectiveness + typeComboA[j].effectiveness > 0 || typeComboB[i].effectiveness + typeComboB[j].effectiveness > 0 || typeComboC[i].effectiveness + typeComboC[j].effectiveness > 0 || typeComboD[i].effectiveness + typeComboD[j].effectiveness > 0)
                                    {
                                        numSuperEffective++;

                                    }//if
                                }//else
                            }//for j
                        }//for i

                        if(numSuperEffective > bestSuperEffective)
                        {
                            bestSuperEffective = numSuperEffective;
                            bestMoveset = moveset;
                        }//if
                    }//for d
                }//for c
            }//for b
        }//for a

        cout << bestSuperEffective << "\n";
        for(int i = 0; i < 4; i++)
        {
            cout << bestMoveset[i] << " ";
        }//for i
    }//if
    else if(mode == 'b')
    {
        vector<string> typeSetOffense(2);
        vector<string> bestTypeSetOffense;
        int bestSuperEffective = -1;

        for(int a = 0; a < numTypes - 1; a++)
        {
            for(int b = a + 1; b < numTypes; b++)
            {
                typeSetOffense[0] = defaultList[a].name;
                typeSetOffense[1] = defaultList[b].name;
                vector<pkmnType>& typeComboA = attackers[typeSetOffense[0]];
                vector<pkmnType>& typeComboB = attackers[typeSetOffense[1]];
                int numSuperEffective = 0;
                
                for(int i = 0; i < numTypes; i++)
                {
                    for(int j = i; j < numTypes; j++)
                    {
                        if(j == i)
                        {
                            if(typeComboA[i].effectiveness > 0 || typeComboB[i].effectiveness > 0)
                            {
                                numSuperEffective++;

                            }//if
                        }//if
                        else
                        {
                            if(typeComboA[i].effectiveness + typeComboA[j].effectiveness > 0 || typeComboB[i].effectiveness + typeComboB[j].effectiveness > 0)
                            {
                                numSuperEffective++;

                            }//if
                        }//else
                    }//for j
                }//for i

                if(numSuperEffective > bestSuperEffective)
                {
                    bestSuperEffective = numSuperEffective;
                    bestTypeSetOffense = typeSetOffense;
                }//if
            }//for b
        }//for a

        cout << bestSuperEffective << "\n";
        for(int i = 0; i < 2; i++)
        {
            cout << bestTypeSetOffense[i] << " ";
        }//for i
    }//else if
    else if(mode == 'c')
    {
        vector<string> typeSetDefense(2);
        vector<string> bestTypeSetDefense;
        int bestResistEffective = -1;

        for(int a = 0; a < numTypes; a++)
        {
            for(int b = a; b < numTypes; b++)
            {
                typeSetDefense[0] = defaultList[a].name;
                typeSetDefense[1] = defaultList[b].name;
                int numResistEffective = 0;

                for(int i = 0; i < numTypes; i++)
                {
                    vector<pkmnType>& typeCombo = attackers[defaultList[i].name];
                    
                    if(a == b)
                    {
                        if(typeCombo[a].effectiveness < 0)
                        {
                            numResistEffective++;

                        }//if
                    }//if
                    else
                    {
                        if(typeCombo[a].effectiveness + typeCombo[b].effectiveness < 0)
                        {
                            numResistEffective++;

                        }//if
                    }//else
                }//for i

                if(numResistEffective > bestResistEffective)
                {
                    bestResistEffective = numResistEffective;
                    bestTypeSetDefense = typeSetDefense;
                }//if
            }//for b
        }//for a
        
        cout << bestResistEffective << "\n";
        for(int i = 0; i < 2; i++)
        {
            cout << bestTypeSetDefense[i] << " ";
        }//for i
    }//else if

    return 0;
}//main