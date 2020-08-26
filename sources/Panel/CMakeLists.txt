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


file(GLOB VS_KEIL_CMSIS    ../common/vs_keil/CMSIS/core_cm4.h)

file(GLOB LIBS_CMSIS     ../common/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F2xx/Include/*
                         ../common/STM32Cube_F4/Drivers/CMSIS/Include/*)

file(GLOB LIBS_HAL       ../common/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.h
                         ../common/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.c)

file(GLOB HARDWARE       src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB HARDWARE_HAL   src/Hardware/HAL/*)

set_source_files_properties(${LIBS_HAL} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(libs\\vs_keil\\CMSIS  FILES ${VS_KEIL_CMSIS})
source_group(libs\\CMSIS           FILES ${LIBS_CMSIS})
source_group(libs\\HAL             FILES ${LIBS_HAL})
source_group(Hardware              FILES ${HARDWARE})
source_group(Hardware\\HAL         FILES ${HARDWARE_HAL})

add_library(Panel ${VS_KEIL_CMSIS} ${LIBS_CMSIS} ${LIBS_HAL} ${HARDWARE} ${HARDWARE_HAL})
