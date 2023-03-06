// Issa Samara


#ifndef _ISAMARA_H_
#define _ISAMARA_H_

class prompt {
    public:
        float w, h;
        float pos[2];
        unsigned char color[3];
        void set_color ( unsigned char col[3]);
        prompt();
        prompt(float, float, float, float);
};


extern void render_menu(prompt,Rect,int,int,int);
extern void render_title(Rect t,int,int);
extern unsigned int select_option(int,int);

#endif
