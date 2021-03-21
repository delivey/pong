#include "bounce.h"

void bounce(float &ballY, int screenY, int ballRadius, float &ballYSpeed)
{
    if (ballY > screenY || ballY < ballRadius)
    {
        ballYSpeed *= -1;
    }
}