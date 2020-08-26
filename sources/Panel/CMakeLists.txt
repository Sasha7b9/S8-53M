project (Panel)

include_directories(..
                    src
                    src/Hardware/HAL
                    ../common/vs_keil/ARMCLANG
                    ../common/vs_keil/CMSIS
                    ../common/vs_keil/FatFS
                    ../common/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../common/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/STM32Cube_F4/Drivers/CMSIS/Include
                    )
                    
add_definitions(-DSTM32F429xx -DPANEL)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")


add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4820)    # 'bytes' bytes padding added after construct 'member_name'
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified

file(GLOB VS_KEIL_ARMCLANG ../common/vs_keil/ARMCLANG/*)
file(GLOB VS_KEIL_CMSIS    ../common/vs_keil/CMSIS/core_cm4.h)

file(GLOB LIBS_CMSIS     ../common/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F2xx/Include/*
                         ../common/STM32Cube_F4/Drivers/CMSIS/Include/*)

file(GLOB LIBS_HAL       ../common/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.h
                         ../common/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.c)

file(GLOB HARDWARE       src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB HARDWARE_CMSIS src/Hardware/CMSIS/*)
file(GLOB HARDWARE_HAL   src/Hardware/HAL/*)
file(GLOB HARDWARE_STM32 src/Hardware/stm32/*)
file(GLOB SRC            src/*.h src/*.cpp)
file(GLOB COMMON         ../common/Command.h ../common/Decoder_p.h ../common/Decoder_p.cpp ../common/Decoder_d.h)
file(GLOB DISPLAY        src/Display/*.h src/Display/*.cpp)

set_source_files_properties(${LIBS_HAL} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(libs\\vs_keil\\ARMCLANG FILES ${VS_KEIL_ARMCLANG})
source_group(libs\\vs_keil\\CMSIS    FILES ${VS_KEIL_CMSIS})
source_group(libs\\CMSIS             FILES ${LIBS_CMSIS})
source_group(libs\\HAL               FILES ${LIBS_HAL})
source_group(Hardware                FILES ${HARDWARE})
source_group(Hardware\\CMSIS         FILES ${HARDWARE_CMSIS})
source_group(Hardware\\HAL           FILES ${HARDWARE_HAL})
source_group(Hardware\\stm32         FILES ${HARDWARE_STM32})
source_group(src                     FILES ${SRC})
source_group(common                  FILES ${COMMON})
source_group(Display                 FILES ${DISPLAY})

add_library(Panel ${VS_KEIL_ARMCLANG} ${VS_KEIL_CMSIS} ${LIBS_CMSIS} ${LIBS_HAL} ${HARDWARE} ${HARDWARE_CMSIS} ${HARDWARE_HAL} ${HARDWARE_STM32}
                  ${SRC} ${COMMON} ${DISPLAY})
