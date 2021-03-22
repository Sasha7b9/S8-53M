#include "defines.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Display/Font/Font_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Math_.h"
#include "FDrive/FDrive.h"
#include "Menu/FileManager.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <cstring>


using namespace Primitives;


#define RECS_ON_PAGE 23
#define WIDTH_COL 135


static char currentDir[255] = "\\";
static int numFirstDir = 0;         // Номер первого выведенного каталога в левой панели. Всего может быть выведено RECS_ON_PAGE каталогов
static int numCurDir = 0;           // Номер подсвеченного каталога
static int numFirstFile = 0;        // Номер первого выведенного файла в правой панели. Всего может быть выведено RECS_ON_PAGE файлов.
static int numCurFile = 0;          // Номер подсвеченного файла
static int numDirs = 0;
static int numFiles = 0;

static bool cursorInDirs = false;    // Если true, то ручка УСТАНОВКА перемещает по каталогам


uint FM::needRedrawFileManager = 0;

void FM::Init()
{
    std::strcpy(currentDir, "\\");
    numFirstDir = 0;
    numFirstFile = 0;
    numCurDir = 0;
    numCurFile = 0;
}


void FM::DrawLongString(int x, int y, pchar string, bool hightlight)
{
    int length = Font::GetLengthText(string);

    Color color = Color::FILL;
    if (hightlight)
    {
        Region(WIDTH_COL + 9, 8).Fill(x - 1, y, color);

        color = Color::BACK;
    }

    if (length <= WIDTH_COL)
    {
        Text(string).Draw(x, y, color);
    }
    else
    {
        Text(string).DrawWithLimitation(x, y, color, x, y, WIDTH_COL, 10);
        Text("...").Draw(x + WIDTH_COL + 3, y);
    }
}


void FM::DrawHat(int x, int y, char *string, int num1, int num2)
{
    Region(WIDTH_COL + 9, RECS_ON_PAGE * 9 + 11).Fill(x - 1, y, Color::BACK);
    Text(String(string, num1, num2)).Draw(x + 60, y, Color::FILL);
    HLine().Draw(y + 10, x + 2, x + 140);
}


void FM::DrawDirs(int x, int y)
{
    FDrive::GetNumDirsAndFiles(currentDir, &numDirs, &numFiles);
    DrawHat(x, y, "Каталог : %d/%d", numCurDir + ((numDirs == 0) ? 0 : 1), numDirs);
    StructForReadDir sfrd;
    y += 12;

    String nameDir = FDrive::GetNameDir(currentDir, numFirstDir, &sfrd);

    if (nameDir.Size() != 0)
    {
        DrawLongString(x, y, nameDir.c_str(), cursorInDirs && (numFirstDir == numCurDir));
        int drawingDirs = 0;
        while (drawingDirs < (RECS_ON_PAGE - 1) && FDrive::GetNextNameDir(nameDir.c_str(), &sfrd))
        {
            drawingDirs++;
            DrawLongString(x, y + drawingDirs * 9, nameDir.c_str(), cursorInDirs && ( numFirstDir + drawingDirs == numCurDir));
        }
    }
}


void FM::DrawFiles(int x, int y)
{
    DrawHat(x, y, "Файл : %d/%d", numCurFile + ((numFiles == 0) ? 0 : 1), numFiles);
    char nameFile[255];
    StructForReadDir sfrd;
    y += 12;
    if (FDrive::GetNameFile(currentDir, numFirstFile, nameFile, &sfrd))
    {
        DrawLongString(x, y, nameFile, !cursorInDirs && (numFirstFile == numCurFile));
        int drawingFiles = 0;
        while (drawingFiles < (RECS_ON_PAGE - 1) && FDrive::GetNextNameFile(nameFile, &sfrd))
        {
            drawingFiles++;
            DrawLongString(x, y + drawingFiles * 9, nameFile, !cursorInDirs && (numFirstFile + drawingFiles == numCurFile));
        }
    }
}


bool FM::FileIsExist(const char name[255])
{
    char nameFile[255];
    FDrive::GetNumDirsAndFiles(currentDir, &numDirs, &numFiles);
    StructForReadDir sfrd;
    if(FDrive::GetNameFile(currentDir, 0, nameFile, &sfrd))
    {
        while(FDrive::GetNextNameFile(nameFile, &sfrd))
        {
            if(std::strcmp(name + 2, nameFile) == 0)
            {
                return true;
            }
        }
    }
    return false;
}


void FM::DrawNameCurrentDir(int left, int top)
{
    Color::FILL.SetAsCurrent();
    int length = Font::GetLengthText(currentDir);
    if (length < 277)
    {
        Text(currentDir).Draw(left + 1, top + 1);
    }
    else
    {
        char *pointer = currentDir + 2;
        while (length > 277)
        {
            while (*pointer != '\\' && pointer < currentDir + 255)
            {
                pointer++;
            }
            if (pointer >= currentDir + 255)
            {
                return;
            }
            length = Font::GetLengthText(++pointer);
        }
        Text(pointer).Draw(left + 1, top + 1);
    }
}


void FM::Draw()
{
    if (needRedrawFileManager == 0)
    {
        return;
    }

    int left = 1;
    int top = 1;
    int width = 297;
    int left2col = width / 2;

    if (needRedrawFileManager == 1)
    {
        Display::BeginFrame(Color::BACK);
        Menu::Draw();
        Primitives::Rectangle(width, 239).Draw(1, 0, Color::FILL);
        FDrive::GetNumDirsAndFiles(currentDir, &numDirs, &numFiles);
        DrawNameCurrentDir(left + 1, top + 2);
        HLine().Draw(left2col, top + 16, 239, Color::FILL);
        HLine().Draw(top + 15, 0, width);
    }

    if (needRedrawFileManager != 3)
    {
        DrawDirs(left + 3, top + 18);
    }

    if (needRedrawFileManager != 2)
    {
        DrawFiles(left2col + 3, top + 18);
    }

    Display::EndFrame();

    needRedrawFileManager = 0;
}


void FM::PressTab()
{
    needRedrawFileManager = 1;

    if (cursorInDirs)
    {
        if (numFiles != 0)
        {
            cursorInDirs = false;
        }
    }
    else
    {
        if (numDirs != 0)
        {
            cursorInDirs = true;
        }
    }
}


void FM::PressLevelDown()
{
    needRedrawFileManager = 1;

    if (!cursorInDirs)
    {
        return;
    }

    StructForReadDir sfrd;

    String nameDir = FDrive::GetNameDir(currentDir, numCurDir, &sfrd);

    if (nameDir.Size() != 0)
    {
        if (std::strlen(currentDir) + nameDir.Size() < 250)
        {
            FDrive::CloseCurrentDir(&sfrd);
            std::strcat(currentDir, "\\");
            std::strcat(currentDir, nameDir.c_str());
            numFirstDir = 0;
            numFirstFile = 0;
            numCurDir = 0;
            numCurFile = 0;
        }

    }
    FDrive::CloseCurrentDir(&sfrd);
}


void FM::PressLevelUp()
{
    needRedrawFileManager = 1;
    if (std::strlen(currentDir) == 1)
    {
        return;
    }
    char *pointer = currentDir + std::strlen(currentDir);
    while (*pointer != '\\')
    {
        pointer--;
    }
    *pointer = '\0';
    numFirstDir = 0;
    numFirstFile = 0;
    numCurDir = 0;
    numCurFile = 0;
    cursorInDirs = true;
}


void FM::IncCurrentDir()
{
    if (numDirs > 1)
    {
        numCurDir++;
        if (numCurDir > numDirs - 1)
        {
            numCurDir = 0;
            numFirstDir = 0;
        }
        if (numCurDir - numFirstDir > RECS_ON_PAGE - 1)
        {
            numFirstDir++;
        }
    }
}


void FM::DecCurrentDir()
{
    if (numDirs > 1)
    {
        numCurDir--;
        if (numCurDir < 0)
        {
            numCurDir = numDirs - 1;
            LIMITATION(numFirstDir, numDirs - RECS_ON_PAGE, 0, numCurDir);
        }
        if (numCurDir < numFirstDir)
        {
            numFirstDir = numCurDir;
        }
    }
}


void FM::IncCurrentFile()
{
    if (numFiles > 1)
    {
        numCurFile++;
        if (numCurFile > numFiles - 1)
        {
            numCurFile = 0;
            numFirstFile = 0;
        }
        if (numCurFile - numFirstFile > RECS_ON_PAGE - 1)
        {
            numFirstFile++;
        }
    }
}


void FM::DecCurrentFile()
{
    if (numFiles > 1)
    {
        numCurFile--;
        if (numCurFile < 0)
        {
            numCurFile = numFiles - 1;
            LIMITATION(numFirstFile, numFiles - RECS_ON_PAGE, 0, numCurFile);
        }
        if (numCurFile < numFirstFile)
        {
            numFirstFile = numCurFile;
        }
    }
}


void FM::RotateRegSet(int angle)
{
    Sound::RegulatorSwitchRotate();
    if (cursorInDirs)
    {
        angle > 0 ? DecCurrentDir() : IncCurrentDir();
        needRedrawFileManager = 2;
    }
    else
    {
        angle > 0 ? DecCurrentFile() : IncCurrentFile();
        needRedrawFileManager = 3;
    }
}


bool FM::GetNameForNewFile(char name[255])
{
    char buffer[20];
    int number = 1;

LabelNextNumber:

    std::strcpy(name, currentDir);
    std::strcat(name, "\\");

    int size = (int)(std::strlen(FILE_NAME));
    if (size == 0)
    {
        return false;
    }

    if (FileNamingMode::IsHand())
    {
        LIMITATION(size, size, 1, 95);
        std::strcat(name, FILE_NAME);
        std::strcat(name, ".");
        std::strcat(name, ModeSaveSignal::IsBMP() ? "bmp" : "txt");
        return true;
    }
    else
    {
        PackedTime time = HAL_RTC::GetPackedTime();
                           //  1          2           3         4           5             6
        uint values[] = {0, time.year, time.month, time.day, time.hours, time.minutes, time.seconds};

        char *ch = FILE_NAME_MASK;
        char *wr = name;

        while (*wr != '\0')
        {
            wr++;
        }

        while (*ch)
        {
            if (*ch >= 0x30)        // Если текстовый символ
            {
                *wr = *ch;          // то записываем его в имя файла
                wr++;
            }
            else
            {
                if (*ch == 0x07)    // Если здесь надо записать порядковый номер
                {
                    std::strcpy(wr, GF::Int2String(number, false, *(ch + 1)).c_str());
                    wr += std::strlen(buffer);
                    ch++;
                }
                else
                {
                    if (*ch >= 0x01 && *ch <= 0x06)
                    {
                        std::strcpy(wr, GF::Int2String((int)values[*ch], false, 2).c_str());
                        wr += std::strlen(buffer);
                    }
                }
            }
            ch++;
        }

        *wr = '.';
        *(wr + 1) = '\0';

        std::strcat(name, ModeSaveSignal::IsBMP() ? "bmp" : "txt");

        if(FileIsExist(name))
        {
            number++;
            goto LabelNextNumber; //-V2505
        }

        return true;
    }
}
