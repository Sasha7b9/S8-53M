project (S8-53M)

include_directories(../..
                    ../../Device/src
                    ../../Device/src/Hardware/FatFS
                    ../../Device/src/Hardware/HAL
                    ../../Device/src/Hardware/LwIP
                    ../../Device/src/Hardware/USBD
                    ../../Device/src/Hardware/USBH
                                 src
                    ../../common/_VS_GUI/CMSIS
                    ../../common/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../../common/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../../common/STM32Cube_F4/Middlewares/Third_Party/FatFs/src
                    ../../common/STM32Cube_F4/Middlewares/Third_Party/LwIP/src/include
                    ../../common/STM32Cube_F4/Middlewares/Third_Party/LwIP/system
                    ../../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../../ThirdParty/wxWidgets/include
                    ../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud
                    )
add_definitions(-DSTM32F437xx -DWXUSINGDLL -DGUI -DDEVICE)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4100)    # 'identifier' : unreferenced formal parameter
add_compile_options(/wd4127)    # conditional expression is constant
add_compile_options(/wd4211)    # nonstandard extension used : redefined extern to static
add_compile_options(/wd4255)    # 'function' : no function prototype given: converting '()' to '(void)'
add_compile_options(/wd4310)    # cast truncates constant value
add_compile_options(/wd4464)    # relative include path contains '..'
add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4623)    # 'derived class' : default constructor was implicitly defined as deleted because a base class default constructor is inaccessible or deleted
add_compile_options(/wd4625)    # 'derived class' : copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
add_compile_options(/wd4626)    # 'xxx': assignment operator was implicitly defined as deleted
add_compile_options(/wd4668)    # 'xxx' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
add_compile_options(/wd4710)    # 'xxx': function not inlined
add_compile_options(/wd4774)    # 'string' : format string expected in argument number is not a string literal
add_compile_options(/wd4820)    # 'xxx': '4' bytes padding added after data member 'yyy'
add_compile_options(/wd4996)    # 'xxx': This function or variable may be unsafe.
add_compile_options(/wd5026)    # 'type': move assignment operator was implicitly defined as deleted
add_compile_options(/wd5027)    # 'type': move assignment operator was implicitly defined as deleted
add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc.
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5219)
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies that compiler generated copy/move constructors and copy/move assignment operators are not trivial
add_compile_options(/wd26812)   # the enum type 'xxx' is unscoped. Prefer 'enum class' over 'enum'

file(GLOB LIBS_CMSIS        ../../common/_VS_GUI/CMSIS/*)
file(GLOB LIBS_FATFS        ../../common/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/diskio.*
                            ../../common/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/ff.*
                            ../../common/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/ff_gen_drv.*
                            ../../common/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/option/unicode.c
                            ../../common/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/option/syscall.c)
file(GLOB LIBS_LWIP         ../../common/_VS_GUI/LwIP/*)
file(GLOB LIBS_HAL          ../../common/_VS_GUI/HAL/*)
file(GLOB LIBS_USBH         ../../common/_VS_GUI/USBH/*)
file(GLOB DISPLAY           ../../Device/src/Display/Colors.*
                            ../../Device/src/Display/Colors_d.cpp
                            ../../Device/src/Display/Display.*
                            ../../Device/src/Display/DisplayPrimitives.*
                            ../../Device/src/Display/Grid.*
                            ../../Device/src/Display/Painter.*
                            ../../Device/src/Display/PainterText.cpp
                                ../../common/Display/Colors.*
                                ../../common/Display/Primitives.*
                                ../../common/Display/Text.*
                                         src/Display/Painter_win.cpp
                                         src/Display/Primitives_win.cpp)
file(GLOB DISPLAY_FONT          ../../common/Display/Font/*)
file(GLOB LAN               ../../Device/src/LAN/LAN.h
                                         src/LAN/LAN_win.cpp
                            ../../Device/src/LAN/SocketTCP.h
                            ../../Device/src/LAN/SocketTCP.cpp)
file(GLOB FDRIVE            ../../Device/src/FDrive/*)
file(GLOB FPGA              ../../Device/src/FPGA/FPGA.h
                            ../../Device/src/FPGA/MathFPGA.*
                            ../../Device/src/FPGA/Storage.*
                                         src/FPGA/FPGA_win.cpp)
file(GLOB HARDWARE          ../../Device/src/Hardware/EPROM.h
                                         src/Hardware/EPROM_win.cpp
                            ../../Device/src/Hardware/Hardware.*
                            ../../Device/src/Hardware/Sound.*
                            ../../Device/src/Hardware/Timer.*)
file(GLOB HARDWARE_HAL          ../../common/Hardware/HAL/HAL.h
                                ../../common/Hardware/HAL/HAL_PINS.h
                                ../../common/Hardware/HAL/cHAL_PINS.cpp
                                ../../common/Hardware/HAL/cHAL_ADC3.cpp
                                ../../common/Hardware/HAL/cHAL_CRC.cpp
                                ../../common/Hardware/HAL/cHAL_DAC.cpp
                                ../../common/Hardware/HAL/cHAL_OTP.cpp
                                ../../common/Hardware/HAL/cHAL_RTC.cpp
                                ../../common/Hardware/HAL/cHAL_SPI1.cpp
                                ../../common/Hardware/HAL/cHAL_SRAM.cpp
                                ../../common/Hardware/HAL/cHAL_TIM6.cpp
                                ../../common/Hardware/HAL/cHAL_TIM7.cpp
                                         src/Hardware/*.h
                                         src/Hardware/HAL/*.cpp)
file(GLOB MENU              ../../Device/src/Menu/*.h
                            ../../Device/src/Menu/*.cpp)
file(GLOB MENU_PAGES        ../../Device/src/Menu/Pages/*)
file(GLOB PANEL             ../../Device/src/Panel/* ../../common/Keyboard/*)
file(GLOB SETTINGS          ../../Device/src/Settings/*)
file(GLOB SRC                            src/main_win.cpp
                            ../../Device/src/defines.*
                            ../../Device/src/device.*
                            ../../Device/src/Globals.*
                            ../../Device/src/Tables.*
                                ../../common/Log.h
                                ../../common/cLog.cpp)
file(GLOB VCP               ../../Device/src/VCP/*.h
                            ../../Device/src/VCP/*.cpp)
file(GLOB UTILS             ../../Device/src/Utils/*.h
                            ../../Device/src/Utils/*.cpp
                                ../../common/Utils/*)
file(GLOB GUI                            src/GUI/*.h
                                         src/GUI/*.cpp
                                         src/GUI/*.c)
file(GLOB GUI_DIALOGS                    src/GUI/Dialogs/*)

set_source_files_properties(../../Device/src/Ethernet/Ethernet.cpp
                            ../../Device/src/Menu/Pages/PageHelp.cpp
                            ../../Device/src/Panel/PanelFunctions.cpp
                            PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(libs\\CMSIS    FILES ${LIBS_CMSIS})
source_group(libs\\FatFS    FILES ${LIBS_FATFS})
source_group(libs\\LwIP     FILES ${LIBS_LWIP})
source_group(libs\\HAL      FILES ${LIBS_HAL})
source_group(libs\\USBH     FILES ${LIBS_USBH})
source_group(Display        FILES ${DISPLAY})
source_group(Display\\Font  FILES ${DISPLAY_FONT})
source_group(LAN            FILES ${LAN})
source_group(FDrive         FILES ${FDRIVE})
source_group(FPGA           FILES ${FPGA})
source_group(GUI            FILES ${GUI})
source_group(GUI\\Dialogs   FILES ${GUI_DIALOGS})
source_group(Hardware       FILES ${HARDWARE})
source_group(Hardware\\HAL  FILES ${HARDWARE_HAL})
source_group(Menu           FILES ${MENU})
source_group(Menu\\Pages    FILES ${MENU_PAGES})
source_group(Panel          FILES ${PANEL})
source_group(Settings       FILES ${SETTINGS})
source_group(src            FILES ${SRC})
source_group(VCP            FILES ${VCP})
source_group(Utils          FILES ${UTILS})

add_executable(S8-53M ${LIBS_CMSIS} ${LIBS_FATFS} ${LIBS_LWIP} ${LIBS_HAL} ${LIBS_USBH} ${DISPLAY} ${DISPLAY_FONT} ${LAN} ${FDRIVE} ${FPGA} ${GUI} ${GUI_DIALOGS} ${HARDWARE}
                      ${HARDWARE_HAL} ${MENU} ${MENU_PAGES} ${PANEL} ${SETTINGS} ${SRC} ${VCP} ${UTILS})

target_link_libraries(S8-53M ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud)
target_link_libraries(S8-53M ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxmsw31ud_core)

target_link_libraries(S8-53M WinMM.lib)
target_link_libraries(S8-53M version.lib)
target_link_libraries(S8-53M Imm32.Lib)
target_link_libraries(S8-53M Setupapi.lib)
