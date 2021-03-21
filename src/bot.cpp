#include "bot.h"

void move_bot(float &ballY, float &botY, int screenY, int playerYLength)
{
    if (ballY > 0 && ballY < screenY - playerYLength)
        botY = ballY;
}