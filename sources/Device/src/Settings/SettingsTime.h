#pragma once


#define TIME_DIV_XPOS                       (set.time.timeDivXPos)
#define TIME_DIV_XPOS_IS_SHIFT_IN_MEMORY    (TIME_DIV_XPOS == FunctionTime::ShiftInMemory)

#define SET_TPOS                            (set.time.tPos)
#define SET_TPOS_IS_LEFT                    (SET_TPOS == TPos::Left)
#define SET_TPOS_IS_CENTER                  (SET_TPOS == TPos::Center)
#define SET_TPOS_IS_RIGHT                   (SET_TPOS == TPos::Right)


// Настройки оси X.
struct SettingsTime
{ //-V802
    TBase::E            tBase;          // Масштаб по времени.
    int16               tShiftRel;      // Смещение по времени.
    FunctionTime::E     timeDivXPos;
    TPos::E             tPos;           // Привязка синхронизации к памяти.
    SampleType::E       sampleType;     // Тип выборки для режима рандомизатора.
    PeackDetMode::E     peakDet;        // Режим работы пикового детектора
    bool                selfRecorder;   // Включен ли режим самописца.
    ENUM_POINTS_FPGA::E oldNumPoints;   // \brief Когда переключаемся в режим пикового детектора, устанавливаем
                                        // количество точек в 1024, а сюда  записываем то, что было, чтобы потом
                                        // восстановить.
};
