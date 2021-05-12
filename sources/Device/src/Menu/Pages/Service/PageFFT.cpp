// 2021/05/12 15:44:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Warnings.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(cMath_FFT_Enable, PageService::PageMath::PageFFT::self,
    "�����������", "Display",
    "�������� � ��������� ����������� �������",
    "Enables or disables the display of the spectrum",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.math.fft.enabled, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(cMath_FFT_Scale, PageService::PageMath::PageFFT::self,
    "�����", "Scale",
    "����� ������� ������ ������� - �������� ��� ���������������",
    "Sets the scale of the output spectrum - linear or logarithmic",
    "��������", "Log",
    "��������", "Linear",
    set.math.fft.scale, nullptr, nullptr, nullptr
)


DEF_CHOICE_3(cMath_FFT_Source, PageService::PageMath::PageFFT::self,
    "��������", "Source",
    "����� ��������� ��� ������� �������",
    "Selecting the source for the calculation of the spectrum",
    "����� 1", "Channel 1",
    "����� 2", "Channel 2",
    "����� 1 + 2", "Channel 1 + 2",
    set.math.fft.source, nullptr, nullptr, nullptr
)


DEF_CHOICE_4(cMath_FFT_Window, PageService::PageMath::PageFFT::self,
    "����", "Window",
    "����� ���� ��� ������� �������",
    "Sets the window to calculate the spectrum",
    "�����������", "Rectangle",
    "��������", "Hamming",
    "��������", "Blackman",
    "�����", "Hann",
    set.math.fft.window, nullptr, nullptr, nullptr
)


static bool IsActive_Math_FFT_Limit()
{
    return set.math.fft.scale.IsLog();
}


DEF_CHOICE_3(cMath_FFT_Limit, PageService::PageMath::PageFFT::self,
    "��������", "Range",
    "����� ����� ������ ������ ���������� �� ��������� �������",
    "Here you can set the limit of monitoring the power spectrum",
    "-40��", "-40dB",
    "-60��", "-60dB",
    "-80��", "-80dB",
    set.math.fft.max_DB, IsActive_Math_FFT_Limit, nullptr, nullptr
)


static bool IsActive_Math_FFT()
{
    return ModeDrawMath::IsDisabled();
}


static void OnPress_Math_FFT()
{
    if (!IsActive_Math_FFT())
    {
        Warnings::ShowWarningBad(Warning::ImpossibleEnableFFT);
    }
}


DEF_PAGE_6(pageFFT, PageService::PageMath::self, NamePage::MathFFT,
    "������", "SPECTRUM",
    "����������� ������� �������� �������",
    "Mapping the input signal spectrum",
    cMath_FFT_Enable,                                   // ������ - ���������� - ������ - �����������
    cMath_FFT_Scale,                                    // ������ - ���������� - ������ - �����
    cMath_FFT_Source,                                   // ������ - ���������� - ������ - ��������
    cMath_FFT_Window,                                   // ������ - ���������� - ������ - ����
    PageService::PageMath::PageFFT::PageCursors::self,  // ������ - ���������� - ������ - �������
    cMath_FFT_Limit,                                    // ������ - ���������� - ������ - ��������
    IsActive_Math_FFT, OnPress_Math_FFT, nullptr, nullptr
)

const Page *PageService::PageMath::PageFFT::self = &pageFFT;
