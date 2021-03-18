#pragma once


#define SET_TBASE                           (set.time.tBase)

#define TSHIFT                              (set.time.tShiftRel)

#define TIME_DIV_XPOS                       (set.time.timeDivXPos)
#define TIME_DIV_XPOS_IS_SHIFT_IN_MEMORY    (TIME_DIV_XPOS == FunctionTime::ShiftInMemory)

#define SET_TPOS                            (set.time.tPos)
#define SET_TPOS_IS_LEFT                    (SET_TPOS == TPos::Left)
#define SET_TPOS_IS_CENTER                  (SET_TPOS == TPos::Center)
#define SET_TPOS_IS_RIGHT                   (SET_TPOS == TPos::Right)

#define SAMPLE_TYPE                         (set.time.sampleType)
#define SAMPLE_TYPE_IS_EQUAL                (SAMPLE_TYPE == SampleType::Equal)
#define SAMPLE_TYPE_IS_REAL                 (SAMPLE_TYPE == SampleType::Real)

#define PEAKDET                             (set.time.peakDet)
#define PEAKDET_IS_DISABLE                  (PEAKDET == PeackDetMode::Disable)
#define PEAKDET_IS_ENABLE                   (PEAKDET == PeackDetMode::Enable)

#define FPGA_IN_RANDOMIZE_MODE              (SET_TBASE < TBase::_100ns)

#define SET_SELFRECORDER                    (set.time.selfRecorder)


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


// Узнать привязку отсительно уровня синхронизации в точках.
int sTime_TPosInPoints(PeackDetMode::E peakDet, int numPoints, TPos::E tPos);

// Смещение по времени в точках экрана. Т.к. на канал у нас работают два АЦП поочерёдно, это значение отличается от засылаемого в аппаратную часть в два раза.
int sTime_TShiftInPoints(PeackDetMode::E peakDet);

// Минимальное смещение по времени, которое может быть записано в аппаратную часть.
int16 sTime_TShiftMin();

// Смещение по времени, соответствующее позиции TPos.
int16 sTime_TShiftZero();

// Если true, значит, работает рандомизатор.
bool sTime_RandomizeModeEnabled();
