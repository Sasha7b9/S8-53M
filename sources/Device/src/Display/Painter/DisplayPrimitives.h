#pragma once


struct ProgressBar
{
    int x;
    int y;
    int width;
    int height;
    float fullTime;
    float passedTime;

    void Draw();
};
