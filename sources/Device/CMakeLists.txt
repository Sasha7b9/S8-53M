project (Device)

include_directories(src
                    src/Hardware/FatFS
                    src/Hardware/HAL
                    src/Hardware/LwIP
                    src/Hardware/USBD
                    src/Hardware/USBH
                    ../common/vs_keil/ARMCLANG
                    ../common/vs_keil/CMSIS
                    ../common/vs_keil/FatFS
                    ../common/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../common/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/STM32Cube_F4/Drivers/CMSIS/Include
                    ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../common/STM32Cube_F4/Middlewares/Third_Party/FatFS/src
                    ../common/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/drivers
                    ../common/STM32Cube_F4/Middlewares/Third_Party/LwIP/src/include
                    ../common/STM32Cube_F4/Middlewares/Third_Party/LwIP/system
                    ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
                    )

add_definitions(-DSTM32F429xx -DUSE_HAL_DRIVER)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4211)    # nonstandard extension used : redefined extern to static
add_compile_options(/wd4310)    # cast truncates constant value
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
file(GLOB VS_KEIL_CMSIS    ../common/vs_keil/CMSIS/core_cm3.h)
file(GLOB VS_KEIL_FATFS    ../common/vs_keil/FatFS/*)

file(GLOB LIBS_FATFS     ../common/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/ff_gen_drv.*
                         ../common/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/ff.*
                         ../common/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/diskio.*
                         ../common/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/option/ccsbcs.*
                         ../common/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/option/syscall.*)

file(GLOB LIBS_CMSIS     ../common/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include/*
                         ../common/STM32Cube_F4/Drivers/CMSIS/Include/*)

file(GLOB LIBS_HAL       ../common/STM32Cube_F4/Drivers/STM32F2xx_HAL_Driver/Inc/*.h
                         ../common/STM32Cube_F4/Drivers/STM32F2xx_HAL_Driver/Src/*.c)

file(GLOB LIBS_USBD      ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_ctlreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_ioreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/usbd_cdc.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.*)

file(GLOB LIBS_USBH      ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_core.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ctlreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ioreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_pipes.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_bot.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_scsi.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.*)

file(GLOB HARDWARE       src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB HARDWARE_CMSIS src/Hardware/CMSIS/*)
file(GLOB HARDWARE_FATFS src/Hardware/FatFS/*)
file(GLOB HARDWARE_HAL   src/Hardware/HAL/*)
file(GLOB HARDWARE_LWIP  src/Hardware/LwIP/*)
file(GLOB HARDWARE_USBD  src/Hardware/USBD/*)
file(GLOB HARDWARE_USBH  src/Hardware/USBH/*)
file(GLOB SRC            src/*.h src/*.cpp)
file(GLOB DISPLAY        src/Display/*.h src/Display/*.cpp)
file(GLOB DISPLAY_FONT   src/Display/font/*)
file(GLOB ETHERNET       src/Ethernet/*)
file(GLOB FDRIVE         src/FDrive/*)
file(GLOB FPGA           src/FPGA/*)
file(GLOB MENU           src/Menu/*.h src/Menu/*.cpp)
file(GLOB MENU_PAGES     src/Menu/Pages/*)
file(GLOB PANEL          src/Panel/*)
file(GLOB SETTINGS       src/Settings/*)
file(GLOB UTILS          src/Utils/*)
file(GLOB VCP            src/VCP/*.h src/VCP/*.cpp)
file(GLOB VCP_SCPI       src/VCP/SCPI/*)

set_source_files_properties(src/Panel/PanelFunctions.cpp PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(src/Menu/Pages/PageHelp.cpp PROPERTIES HEADER_FILE_ONLY TRUE)
#set_source_files_properties(src/VCP/SCPI/trigSCPI.cpp PROPERTIES HEADER_FILE_ONLY TRUE)
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
source_group(Hardware\\FatFS         FILES ${HARDWARE_FATFS})
source_group(Hardware\\HAL           FILES ${HARDWARE_HAL})
source_group(Hardware\\LwIP          FILES ${HARDWARE_LWIP})
source_group(Hardware\\USBD          FILES ${HARDWARE_USBD})
source_group(Hardware\\USBH          FILES ${HARDWARE_USBH})
source_group(src                     FILES ${SRC})
source_group(Display                 FILES ${DISPLAY})
source_group(Display\\font           FILES ${DISPLAY_FONT})
source_group(Ethernet                FILES ${ETHERNET})
source_group(FDrive                  FILES ${FDRIVE})
source_group(FPGA                    FILES ${FPGA})
source_group(Menu                    FILES ${MENU})
source_group(Menu\\Pages             FILES ${MENU_PAGES})
source_group(Panel                   FILES ${PANEL})
source_group(Settings                FILES ${SETTINGS})
source_group(Utils                   FILES ${UTILS})
source_group(VCP                     FILES ${VCP})
source_group(VCP\\SCPI               FILES ${VCP_SCPI})

add_library(Device ${VS_KEIL_CMSIS} ${VS_KEIL_ARMCLANG} ${VS_KEIL_FATFS} ${LIBS_CMSIS} ${LIBS_FATFS} ${LIBS_HAL} ${LIBS_LWIP} ${LIBS_USBH} ${HARDWARE}
                   ${HARDWARE_CMSIS} ${HARDWARE_FATFS} ${HARDWARE_HAL} ${HARDWARE_LWIP} ${HARDWARE_USBD} ${HARDWARE_USBH} ${SRC} ${DISPLAY} 
                   ${DISPLAY_FONT} ${ETHERNET} ${FDRIVE} ${FPGA} ${MENU} ${MENU_PAGES} ${PANEL} ${SETTINGS} ${UTILS} ${VCP} ${VCP_SCPI})
