#pragma once


// Настройки оси X.
struct SettingsTime
{ //-V802
    TBase::E            tBase;          // Масштаб по времени.
    int16               tShiftRel;      // Смещение по времени.
    FunctionTime::E     function_time;
    TPos::E             tPos;           // Привязка синхронизации к памяти.
    SampleType::E       sampleType;     // Тип выборки для режима рандомизатора.
    PeackDetMode::E     peakDet;        // Режим работы пикового детектора
    bool                selfRecorder;   // Включен ли режим самописца.
    ENUM_POINTS_FPGA::E oldNumPoints;   // \brief Когда переключаемся в режим пикового детектора, устанавливаем
                                        // количество точек в 1024, а сюда  записываем то, что было, чтобы потом
                                        // восстановить.
};
