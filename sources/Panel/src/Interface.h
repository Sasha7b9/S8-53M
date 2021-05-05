#pragma once


class Interface
{
public:

    static void Update();

private:

    static void ProcessReceivedData(uint8 *data, uint size);

    static void ProcessByte(uint8 byte);
};
