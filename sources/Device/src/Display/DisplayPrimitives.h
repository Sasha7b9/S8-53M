#pragma once


struct ProgressBar
{
    int x;
    int y;
    int width;
    int height;
    float fullTime;
    float passedTime;
};


void ProgressBar_Draw(const ProgressBar *bar);
