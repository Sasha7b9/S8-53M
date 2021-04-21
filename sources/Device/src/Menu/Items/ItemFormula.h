// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Описывает элемент меню для установки коэффициентов и знаков математической формулы
#define FIELD_SIGN_MEMBER_1_ADD 0
#define FIELD_SIGN_MEMBER_1_MUL 1
#define FIELD_SIGN_MEMBER_2_ADD 2
#define FIELD_SIGN_MEMBER_2_MUL 3
#define POS_SIGN_MEMBER_1       0
#define POS_KOEFF_MEMBER_1      1
#define POS_SIGN_MEMBER_2       2
#define POS_KOEFF_MEMBER_2      3


struct DataFormula
{
    int8 *function;      // Адрес ячейки, где хранится Function, из которой берётся знак операции
    int8 *koeff1add;     // Адрес коэффициента при первом члене для сложения
    int8 *koeff2add;     // Адрес коэффициента при втором члене для сложения
    int8 *koeff1mul;     // Адрес коэффициента при первом члене для умножения
    int8 *koeff2mul;     // Адрес коэффициента при втором члене для умножения
    int8 *curDigit;      // Текущий разряд : 0 - знак первого члена, 1 - коэффициент первого члена,
                            //  2 - знак второго члена, 3 - коэффициент второго члена
    pFuncVV funcOfChanged;  // Эта функция вызывается после изменения состояния элемента управления.
};


class Formula : public Item
{
public:
    Formula(const DataItem *const data) : Item(data) {};
    const DataFormula *OwnData() const {
        return reinterpret_cast<const DataFormula *>(data->ad);
    }
    virtual void Draw(int x, int y, bool opened = false) const;
private:
    void DrawClosed(int x, int y) const;
    void DrawLowPart(int x, int y) const;
    void WriteText(int x, int y, bool) const;
};
