#include "Measures.h"
#include "GlobalFunctions.h"
#include "Math.h"
#include "Menu/Pages/PageMeasures.h"
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "Display/Colors.h"
#include "Display/Painter.h"
#include "Display/Grid.h"
#include "Hardware/Sound.h"
#include <cstdio>


struct StructMeasure
{
    const char *name;
    const char UGO;
};

static const StructMeasure measures[Measure::Count] =
{
    {"",            '\x00'},
    {"U ����",      '\x20'},
    {"U ���",       '\x25'},
    {"U ���",       '\x2a'},
    {"U ���� ���",  '\x40'},
    {"U ��� ���",   '\x45'},
    {"U ����",      '\x4a'},
    {"U ��",        '\x60'},
    {"U ���",       '\x65'},
    {"������+",     '\x6a'},
    {"������-",     '\x80'},
    {"������",      '\x85'},
    {"�������",     '\x8a'},
    {"�� ������",   '\xa0'},
    {"�� �����",    '\xa5'},
    {"����+",       '\xaa'},
    {"����-",       '\xc0'},
    {"������+",     '\xc5'},
    {"������-",     '\xca'},
    {"��������\xa7",'\xe0'},
    {"��������\xa6",'\xe5'},
    {"����\xa7",    '\xe0'},
    {"����\xa6",    '\xe5'}
};

static int8 posActive = 0;                  // ������� ��������� ��������� (�� ������� ������)
static int8 posOnPageChoice = 0;            // ������� ������� �� �������� ������ ���������

bool Measure::IsActive(int row, int col)
{
    if(posActive >= Measure::NumCols() * Measure::NumRows())
    {
        posActive = 0;
    }
    return (row * Measure::NumCols() + col) == posActive;
}

void Measure::GetActive(int *row, int *col)
{
    *row = posActive / Measure::NumCols();
    *col = posActive - (*row) * Measure::NumCols();
}

void Measure::SetActive(int row, int col)
{
    posActive = static_cast<int8>(row * Measure::NumCols() + col);
}

char Measure::GetChar(Measure::E measure)
{
    return measures[measure].UGO;
}

int Measure::GetDY()
{
    if(MEAS_SOURCE_IS_A_B)
    {
        return 30;
    }
    return 21;
}

int Measure::GetDX()
{
    return GRID_WIDTH / 5; 
}

const char* Measure::Name(int row, int col)
{
    int numMeasure = row * Measure::NumCols() + col;
    return measures[MEASURE(numMeasure)].name;
}

Measure::E Measure::Type(int row, int col)
{
    int numMeasure = row * Measure::NumCols() + col;
    return MEASURE(numMeasure);
}

int Measure::GetTopTable()
{
    if(MEAS_NUM_IS_6_2 || MEAS_NUM_IS_6_1)
    {
        return GRID_BOTTOM - Measure::GetDY() * 6;
    }
    return GRID_BOTTOM - Measure::NumRows() * Measure::GetDY();
}

int Measure::NumCols()
{
    static const int cols[] = {1, 2, 5, 5, 5, 1, 2};
    return cols[MEAS_NUM];
}

int Measure::NumRows()
{
    static const int rows[] = {1, 1, 1, 2, 3, 6, 6};
    return rows[MEAS_NUM];
}

int Measure::GetDeltaGridLeft()
{
    if(SHOW_MEASURES && MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if(MEAS_NUM_IS_6_1)
        {
            return Measure::GetDX();
        }
        else if(MEAS_NUM_IS_6_2)
        {
            return Measure::GetDX() * 2;
        }
    }
    return 0;
}

int Measure::GetDeltaGridBottom()
{
    if(SHOW_MEASURES && MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if(MEAS_NUM_IS_1_5)
        {
            return Measure::GetDY();
        }
        else if(MEAS_NUM_IS_2_5)
        {
            return Measure::GetDY() * 2;
        }
        else if(MEAS_NUM_IS_3_5)
        {
            return Measure::GetDY() * 3;
        }
    }
    return 0;
}

void Measure::RotateRegSet(int angle)
{
    static const int8 step = 3;
    static int8 currentAngle = 0;
    currentAngle += (int8)angle;
    if (currentAngle < step && currentAngle > -step)
    {
        return;
    }
    if (PageMeasures::choiceMeasuresIsActive)
    {
        posOnPageChoice += (int8)Math_Sign(currentAngle);
        Sound::RegulatorSwitchRotate();
        if (posOnPageChoice < 0)
        {
            posOnPageChoice = Measure::Count - 1;
        }
        else if (posOnPageChoice == Measure::Count)
        {
            posOnPageChoice = 0;
        }
        MEASURE(posActive) = (Measure::E)posOnPageChoice;
        Painter::ResetFlash();
    }
    else
    {
        int row = 0;
        int col = 0;
        Measure::GetActive(&row, &col);
        col += Math_Sign(currentAngle);
        if (col < 0)
        {
            col = Measure::NumCols() - 1;
            row--;
            if (row < 0)
            {
                row = Measure::NumRows() - 1;
            }
        }
        else if (col == Measure::NumCols())
        {
            col = 0;
            row++;
            if (row >= Measure::NumRows())
            {
                row = 0;
            }
        }
        Measure::SetActive(row, col);
        Sound::RegulatorSwitchRotate();
    }
    currentAngle = 0;
}

void Measure::ShorPressOnSmallButtonSettings()
{
    PageMeasures::choiceMeasuresIsActive = !PageMeasures::choiceMeasuresIsActive;
    if(PageMeasures::choiceMeasuresIsActive)
    {
        posOnPageChoice = static_cast<int8>(MEASURE(posActive));
    }
}

void Measure::ShortPressOnSmallButonMarker()
{
    if(MEASURE_IS_MARKED(posActive))
    {
        MEAS_MARKED = Measure::None;
    }
    else
    {
        MEAS_MARKED = MEASURE(posActive);
    }
}

void Measure::DrawPageChoice()
{
    if(!PageMeasures::choiceMeasuresIsActive)
    {
        return;
    }

    bool num61or62 = MEAS_NUM_IS_6_1 || MEAS_NUM_IS_6_2;

    int x = num61or62 ? (Grid::Right() - 3 * GRID_WIDTH / 5) : Grid::Left();
    int y = GRID_TOP;
    int dX = GRID_WIDTH / 5;
    int dY = 22;
    int maxRow = num61or62 ? 8 : 5;
    int maxCol = num61or62 ? 3 : 5;
    Measure::E meas = Measure::None;
    Painter::SetFont(TypeFont::_UGO);
    for(int row = 0; row < maxRow; row++)
    {
        for(int col = 0; col < maxCol; col++)
        {
            if(meas >= Measure::Count)
            {
                break;
            }
            int x0 = x + col * dX;
            int y0 = y + row * dY;
            bool active = meas == posOnPageChoice;
            Painter::DrawRectangleC(x0, y0, dX, dY, COLOR_FILL);
            Painter::FillRegionC(x0 + 1, y0 + 1, dX - 2, dY - 2, active ? Color::FLASH_10 : COLOR_BACK);
            Painter::SetColor(active ? Color::FLASH_01 : COLOR_FILL);
            Painter::Draw10SymbolsInRect(x0 + 2, y0 + 1, Measure::GetChar(meas));
            if(meas < Measure::Count)
            {
                Painter::SetFont(TypeFont::_5);
                Painter::DrawTextRelativelyRightC(x0 + dX, y0 + 12, measures[meas].name, active ? Color::FLASH_01 : COLOR_FILL);
                Painter::SetFont(TypeFont::_UGO);
            }
            ++meas;
        }
    }
    Painter::SetFont(TypeFont::_8);
}


Measure::E& operator++(Measure::E &measure)
{
    measure = (Measure::E)((int)measure + 1);
    return measure;
}
