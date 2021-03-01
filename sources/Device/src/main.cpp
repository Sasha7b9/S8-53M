#include "device.h"

int main(void)
{    
    Device::Init();
    
    int i = 0;

    while(1)
    {
        i++;
        Device::Update();
    }
}
