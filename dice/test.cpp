#include <iostream>
#include <string>
using namespace std;

int roll(string st)
{
    string data = "";                                                      
    int num = 0, diceType = 0, mod = 1, sum = 0, pos = 0;
    bool hasMod = false;
    while (pos < st.size())                                                 
    {
        if (st[pos] == 'd')                                                 
        {                                                                   
            num = stoi(data);
            data = "";
        }
        else if (st[pos] == '+' || st[pos] == '-')                          
        {                                                                   
            hasMod = true;                                                  
            diceType = stoi(data);
            data = "";
            if (st[pos] == '-') mod = -1;                                   
        }
        else if (st[pos] == ' ')                                            
        {
            if (hasMod) mod *= stoi(data);
            else
            {
                mod = 0;
                diceType = stoi(data);
            }
            for (int i = 0; i < num; i++) sum += rand() % diceType + 1;     
            sum += mod;
            hasMod = false;                                                 
            mod = 1;
            data = "";
            num = 0;
            diceType = 0;
        }
        else data += st[pos];
        pos++;
    }
    return sum;
}

void main()
{
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < 100; i++) cout << roll("2d5+10 1d10 ") << endl;
    system("pause");
}