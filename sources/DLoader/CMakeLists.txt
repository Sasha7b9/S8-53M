project (DLoader)

include_directories(src
                    src/Hardware/FatFS
                    src/Hardware/HAL
                    src/Hardware/USBH
                    ../common/vs_keil/ARMCLANG
                    ../common/vs_keil/CMSIS
                    ../common/vs_keil/FatFS
                    ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc
                    ../common/STM32Cube_F2/Drivers/CMSIS/Device/ST/STM32F2xx/Include
                    ../common/STM32Cube_F2/Drivers/CMSIS/Include
                    ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../common/STM32Cube_F2/Middlewares/Third_Party/FatFS/src
                    ../common/STM32Cube_F2/Middlewares/Third_Party/FatFS/src/drivers
                    ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
                    )

add_definitions(-DSTM32F207xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4201)
add_compile_options(/wd4514)
add_compile_options(/wd4623)
add_compile_options(/wd4626)
add_compile_options(/wd4774)
add_compile_options(/wd4820)
add_compile_options(/wd5027)
add_compile_options(/wd5039)
add_compile_options(/wd5045)
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies

file(GLOB VS_KEIL_ARMCLANG ../common/vs_keil/ARMCLANG/*)
file(GLOB VS_KEIL_CMSIS    ../common/vs_keil/CMSIS/*)
file(GLOB VS_KEIL_FATFS    ../common/vs_keil/FatFS/*)

file(GLOB LIBS_FATFS     ../common/STM32Cube_F2/Middlewares/Third_Party/FatFS/src/ff_gen_drv.*
                         ../common/STM32Cube_F2/Middlewares/Third_Party/FatFS/src/ff.*
                         ../common/STM32Cube_F2/Middlewares/Third_Party/FatFS/src/diskio.*
                         ../common/STM32Cube_F2/Middlewares/Third_Party/FatFS/src/option/ccsbcs.*
                         ../common/STM32Cube_F2/Middlewares/Third_Party/FatFS/src/option/syscall.*)

file(GLOB LIBS_CMSIS     ../common/STM32Cube_F2/Drivers/CMSIS/Device/ST/STM32F2xx/Include/*
                         ../common/STM32Cube_F2/Drivers/CMSIS/Include/*)

file(GLOB LIBS_HAL       ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_hal.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_hal_cortex.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_hal_dma.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_hal_flash.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_hal_flash_ex.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_hal_gpio.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_hal_hcd.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_hal_rcc.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_hal_spi.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_hal_sram.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_hal_tim.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_hal_tim_ex.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_ll_fsmc.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Inc/stm32f2xx_ll_usb.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_cortex.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dma.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_flash.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_flash_ex.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_gpio.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_hcd.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_rcc.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_spi.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_sram.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_tim.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_tim_ex.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_ll_fsmc.*
                         ../common/STM32Cube_F2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_ll_usb.*)

file(GLOB LIBS_USBH      ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_core.*
                         ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ctlreq.*
                         ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ioreq.*
                         ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_pipes.*
                         ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.*
                         ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.*
                         ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.*
                         ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.*
                         ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc.*
                         ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_bot.*
                         ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_scsi.*
                         ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.*
                         ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.*
                         ../common/STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.*)
                         
file(GLOB HARDWARE       src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB HARDWARE_CMSIS src/Hardware/CMSIS/*)
file(GLOB HARDWARE_FATFS src/Hardware/FatFS/*)
file(GLOB HARDWARE_HAL   src/Hardware/HAL/*)
file(GLOB HARDWARE_USBH  src/Hardware/USBH/*)
file(GLOB SRC            src/*.h src/*.cpp)
file(GLOB DISPLAY        src/Display/*.h src/Display/*.cpp)
file(GLOB DISPLAY_FONT   src/Display/font/*)
file(GLOB FDRIVE         src/FDrive/*)
file(GLOB PANEL          src/Panel/*)
file(GLOB SETTINGS       src/Settings/*)
file(GLOB UTILS          src/Utils/*)

set_source_files_properties(${LIBS_FATFS} ${LIBS_HAL} ${LIBS_USBH} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(libs\\vs_keil\\ARMCLANG FILES ${VS_KEIL_ARMCLANG})
source_group(libs\\vs_keil\\CMSIS    FILES ${VS_KEIL_CMSIS})
source_group(libs\\vs_keil\\FatFS    FILES ${VS_KEIL_FATFS})
source_group(libs\\CMSIS             FILES ${LIBS_CMSIS})
source_group(libs\\FatFS             FILES ${LIBS_FATFS})
source_group(libs\\HAL               FILES ${LIBS_HAL})
source_group(libs\\USBH              FILES ${LIBS_USBH})
source_group(Hardware                FILES ${HARDWARE})
source_group(Hardware\\CMSIS         FILES ${HARDWARE_CMSIS})
source_group(Hardware\\HAL           FILES ${HARDWARE_HAL})
source_group(Hardware\\USBH          FILES ${HARDWARE_USBH})
source_group(src                     FILES ${SRC})
source_group(Display                 FILES ${DISPLAY})
source_group(Display\\font           FILES ${DISPLAY_FONT})
source_group(FDrive                  FILES ${FDRIVE})
source_group(Panel                   FILES ${PANEL})
source_group(Settings                FILES ${SETTINGS})
source_group(Utils                   FILES ${UTILS})

                   
add_library(DLoader ${VS_KEIL_CMSIS} ${VS_KEIL_ARMCLANG} ${VS_KEIL_FATFS} ${LIBS_CMSIS}
                    ${LIBS_FATFS} ${LIBS_HAL} ${LIBS_USBH} ${HARDWARE} ${HARDWARE_CMSIS} ${HARDWARE_HAL} ${HARDWARE_USBH}
                    ${SRC} ${DISPLAY} ${DISPLAY_FONT} ${FDRIVE} ${PANEL} ${SETTINGS} ${UTILS})
                    