//George Torres

void georgeFeature()
{
    ggprint8b(&r[6], 20, 0x0034a4eb, "George's Feature");

    if (XK_leftarrow) {
        ball.wid -= .005;
    }
    if (XK_righarrow) {
        ball.wid += .005;
    }
    if (XK_uparrow) {
        ball.hgt += .005;
    }
    if (XK_uparrow) {
        ball.hgt -= .005;
    }
}