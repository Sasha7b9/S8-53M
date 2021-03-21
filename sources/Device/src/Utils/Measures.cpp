#include "defines.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Math_.h"
#include "Display/Grid.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/Measures.h"
#include "Utils/ProcessingSignal.h"


using namespace Primitives;


struct StructMeasure
{
    pchar name;
    const char UGO;
};

static const StructMeasure measures[Measure::Count] =
{
    {"",            '\x00'},
    {"U макс",      '\x20'},
    {"U мин",       '\x25'},
    {"U пик",       '\x2a'},
    {"U макс уст",  '\x40'},
    {"U мин уст",   '\x45'},
    {"U ампл",      '\x4a'},
    {"U ср",        '\x60'},
    {"U скз",       '\x65'},
    {"Выброс+",     '\x6a'},
    {"Выброс-",     '\x80'},
    {"Период",      '\x85'},
    {"Частота",     '\x8a'},
    {"Вр нараст",   '\xa0'},
    {"Вр спада",    '\xa5'},
    {"Длит+",       '\xaa'},
    {"Длит-",       '\xc0'},
    {"Скважн+",     '\xc5'},
    {"Скважн-",     '\xca'},
    {"Задержка\xa7",'\xe0'},
    {"Задержка\xa6",'\xe5'},
    {"Фаза\xa7",    '\xe0'},
    {"Фаза\xa6",    '\xe5'}
};

static int8 posActive = 0;                  // Позиция активного измерения (на котором курсор)
static int8 posOnPageChoice = 0;            // Позиция курсора на странице выбора измерения
int Measure::top_measures = Grid::Bottom();


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
    return Grid::WIDTH / 5; 
}

String Measure::Name(int row, int col)
{
    int numMeasure = row * Measure::NumCols() + col;
    return String(measures[MEASURE(numMeasure)].name);
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
        return Grid::Bottom() - Measure::GetDY() * 6;
    }
    return Grid::Bottom() - Measure::NumRows() * Measure::GetDY();
}

int Measure::NumCols()
{
    static const int cols[] = {1, 2, 5, 5, 5, 1, 2};
    return cols[MeasuresNumber::Get()];
}

int Measure::NumRows()
{
    static const int rows[] = {1, 1, 1, 2, 3, 6, 6};
    return rows[MeasuresNumber::Get()];
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
        posOnPageChoice += (int8)Math::Sign(currentAngle);
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
        Color::ResetFlash();
    }
    else
    {
        int row = 0;
        int col = 0;
        Measure::GetActive(&row, &col);
        col += Math::Sign(currentAngle);
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

    int x = num61or62 ? (Grid::Right() - 3 * Grid::WIDTH / 5) : Grid::Left();
    int y = Grid::TOP;
    int dX = Grid::WIDTH / 5;
    int dY = 22;
    int maxRow = num61or62 ? 8 : 5;
    int maxCol = num61or62 ? 3 : 5;
    Measure::E meas = Measure::None;
    Font::Set(TypeFont::UGO);
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
            Rectangle(dX, dY).Draw(x0, y0, Color::FILL);
            Region(dX - 2, dY - 2).Fill(x0 + 1, y0 + 1, active ? Color::FLASH_10 : Color::BACK);
            (active ? Color::FLASH_01 : Color::FILL).SetAsCurrent();
            Char(Measure::GetChar(meas)).Draw10SymbolsInRect(x0 + 2, y0 + 1);
            Font::Set(TypeFont::S5);
            Text(measures[meas].name).DrawRelativelyRight(x0 + dX, y0 + 12, active ? Color::FLASH_01 : Color::FILL);
            Font::Set(TypeFont::UGO);
            ++meas;
        }
    }
    Font::Set(TypeFont::S8);
}


Measure::E& operator++(Measure::E &measure)
{
    measure = (Measure::E)((int)measure + 1);
    return measure;
}


void Measure::DrawAll()
{
    if (!SHOW_MEASURES)
    {
        top_measures = Grid::Bottom();
        return;
    }

    Processing::CalculateMeasures();

    if (MEAS_FIELD_IS_HAND)
    {
        int x0 = MEAS_POS_CUR_T0 - SHIFT_IN_MEMORY + Grid::Left();
        int y0 = MEAS_POS_CUR_U0 + Grid::TOP;
        int x1 = MEAS_POS_CUR_T1 - SHIFT_IN_MEMORY + Grid::Left();
        int y1 = MEAS_POS_CUR_U1 + Grid::TOP;
        GF::SortInt(&x0, &x1);
        GF::SortInt(&y0, &y1);
        Primitives::Rectangle(x1 - x0, y1 - y0).Draw(x0, y0, Color::FILL);
    }

    int x0 = Grid::Left() - Measure::GetDeltaGridLeft();
    int dX = Measure::GetDX();
    int dY = Measure::GetDY();
    int y0 = Measure::GetTopTable();

    int numRows = Measure::NumRows();
    int numCols = Measure::NumCols();

    for (int str = 0; str < numRows; str++)
    {
        for (int elem = 0; elem < numCols; elem++)
        {
            int x = x0 + dX * elem;
            int y = y0 + str * dY;
            bool active = Measure::IsActive(str, elem) && Menu::GetNameOpenedPage() == NamePage::SB_MeasTuneMeas;
            Measure::E meas = Measure::Type(str, elem);

            if (meas != Measure::None)
            {
                Region(dX, dY).Fill(x, y, Color::BACK);
                Primitives::Rectangle(dX, dY).Draw(x, y, Color::FILL);
                top_measures = Math::MinFrom2(top_measures, y);
            }

            if (active)
            {
                Region(dX - 4, dY - 4).Fill(x + 2, y + 2, Color::FILL);
            }

            if (meas != Measure::None)
            {
                Measure::Name(str, elem).Draw(x + 4, y + 2, active ? Color::BACK : Color::FILL);
                if (meas == MEAS_MARKED)
                {
                    Region(dX - 2, 9).Fill(x + 1, y + 1, active ? Color::BACK : Color::FILL);
                    Measure::Name(str, elem).Draw(x + 4, y + 2, active ? Color::FILL : Color::BACK);
                }
                if (MEAS_SOURCE_IS_A)
                {
                    Processing::GetStringMeasure(meas, Channel::A).Draw(x + 2, y + 11, ChA.GetColor());
                }
                else if (MEAS_SOURCE_IS_B)
                {
                    Processing::GetStringMeasure(meas, Channel::B).Draw(x + 2, y + 11, ChA.GetColor());
                }
                else
                {
                    Processing::GetStringMeasure(meas, Channel::A).Draw(x + 2, y + 11, ChA.GetColor());
                    Processing::GetStringMeasure(meas, Channel::B).Draw(x + 2, y + 20, ChB.GetColor());
                }
            }
        }
    }

    if (Menu::GetNameOpenedPage() == NamePage::SB_MeasTuneMeas)
    {
        Measure::DrawPageChoice();
    }
}
