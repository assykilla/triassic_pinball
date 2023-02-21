//modified by Xander Reyes, //author: Gordon Griesel
//date: Spring 2022
#include <string>
using namespace std;
int alex_feature;
string waterfall[5];

void stringtext(string *text) {
    //string temp1[5] = {"Requirements", "Design", "Coding", "Testing", "Maintenance" }; 
    //string temp2[5] = {"Xander's", "Epic", "Minecraft", "Feature", "Mode"};
    //string waterfall[5];

    for (int i = 0; i < 5; i++) {
            waterfall[i] = text[i];
    }
}

