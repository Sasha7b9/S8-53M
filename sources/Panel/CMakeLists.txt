project (Panel)

include_directories(src
                    src/Hardware/HAL
                    ../common/vs_keil/ARMCLANG
                    ../common/vs_keil/CMSIS
                    ../common/STM32Cube_L0/Drivers/STM32L0xx_HAL_Driver/Inc
                    ../common/STM32Cube_L0/Drivers/CMSIS/Device/ST/STM32L0xx/Include
                    ../common/STM32Cube_L0/Drivers/CMSIS/Include
                    )

add_definitions(-DSTM32L063xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4623)    # 'derived class' : default constructor was implicitly defined as deleted because a base class default constructor is inaccessible or deleted
add_compile_options(/wd4626)    # 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
add_compile_options(/wd4774)    # 'function' : format string expected in argument 3 is not a string literal
add_compile_options(/wd4820)    # 'bytes' bytes padding added after construct 'member_name'
add_compile_options(/wd5027)    # 'derived class' : move assignment operator was implicitly defined as deleted
add_compile_options(/wd5039)    # 'function': pointer or reference to potentially throwing function passed to 'extern "C"' function under -EHc. Undefined behavior may occur if this function throws an exception
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5219)    # implicit conversion from 'type1' to 'type2', possible loss of data
add_compile_options(/wd5220)

file(GLOB VS_KEIL_ARMCLANG ../common/vs_keil/ARMCLANG/*)
file(GLOB VS_KEIL_CMSIS    ../common/vs_keil/CMSIS/core_cm0plus.h)

file(GLOB LIBS_CMSIS     ../common/STM32Cube_L0/Drivers/CMSIS/Device/ST/STM32L0xx/Include/*
                         ../common/STM32Cube_L0/Drivers/CMSIS/Include/*)

file(GLOB LIBS_HAL       ../common/STM32Cube_L0/Drivers/STM32L0xx_HAL_Driver/Inc/*.h
                         ../common/STM32Cube_L0/Drivers/STM32L0xx_HAL_Driver/Src/*.c)

file(GLOB SRC            src/*.h src/*.c src/*.cpp)
file(GLOB HARDWARE_CMSIS src/Hardware/CMSIS/*)
file(GLOB HARDWARE_HAL   src/Hardware/HAL/*)

set_source_files_properties(${LIBS_HAL} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(libs\\vs_keil\\ARMCLANG FILES ${VS_KEIL_ARMCLANG})
source_group(libs\\vs_keil\\CMSIS    FILES ${VS_KEIL_CMSIS})
source_group(libs\\CMSIS             FILES ${LIBS_CMSIS})
source_group(libs\\HAL               FILES ${LIBS_HAL})
source_group(src                     FILES ${SRC})
source_group(Hardware\\CMSIS         FILES ${HARDWARE_CMSIS})
source_group(Hardware\\HAL           FILES ${HARDWARE_HAL})

add_library(Panel ${VS_KEIL_ARMCLANG} ${VS_KEIL_CMSIS} ${LIBS_CMSIS} ${LIBS_HAL} ${SRC} ${HARDWARE_CMSIS} ${HARDWARE_HAL})
