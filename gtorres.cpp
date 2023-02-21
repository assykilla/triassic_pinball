//George Torres

void mirrorMode()
{

    if (positionx >= 0) {
        float positionx -= positionx - positionx;
    }
    else {
        float positionx += positionx + positionx;
    }
}