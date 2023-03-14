//modified by Xander Reyes, //author: Gordon Griesel
//date: Spring 2022
#include <string>
using namespace std;
int alex_feature;
string waterfall[5];
float velocity[2] = {0};
void stringtext(string *text) {
    for (int i = 0; i < 5; i++) {
            waterfall[i] = text[i];
    }
}

