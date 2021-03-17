#pragma once 


// ���������� ���� ��� ��� ����������� ����� ������.
class FM
{
public:
    static void Init();
    
    static void Draw();
    
    static void PressLevelUp();
    
    static void PressLevelDown();
    
    static void RotateRegSet(int angle);
    
    static bool GetNameForNewFile(char name[255]);

    static void PressTab();

    static uint needRedrawFileManager;  // ���� 1, �� ����-�������� ��������� � ������ �����������
                                        // ���� 2, �� ������������ ������ ��������
                                        // ���� 3, �� ������������ ������ �����

private:
    
    static bool FileIsExist(const char name[255]);

    static void DrawLongString(int x, int y, pchar string, bool hightlight);

    static void DrawDirs(int x, int y);

    static void DrawFiles(int x, int y);

    static void DecCurrentDir();

    static void DecCurrentFile();

    static void DrawHat(int x, int y, char *string, int num1, int num2);

    static void DrawNameCurrentDir(int left, int top);

    static void IncCurrentDir();

    static void IncCurrentFile();
};
