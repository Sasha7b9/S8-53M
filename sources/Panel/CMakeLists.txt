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


file(GLOB HARDWARE       src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB HARDWARE_HAL   src/Hardware/HAL/*)

source_group(Hardware          FILES ${HARDWARE})
source_group(Hardware\\HAL     FILES ${HARDWARE_HAL})

add_library(Panel ${HARDWARE} ${HARDWARE_HAL})
