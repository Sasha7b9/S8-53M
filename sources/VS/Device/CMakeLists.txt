project (S8-53M)

include_directories(../..
                    ../../Device/src
                    ../../Device/src/Hardware/FatFS
                    ../../Device/src/Hardware/USBH
                    ../
                    ../../common/STM32Cube_F4/Middlewares/Third_Party/FatFs/src
                    ../../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    )
add_definitions(-DSTM32F437xx -DWXUSINGDLL -DGUI -DDEVICE)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

#add_compile_options(/wd4201)    # nonstandard extension used: nameless struct/union
#add_compile_options(/wd4464)    # relative include path contains '..'
#add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
#add_compile_options(/wd4571)    # Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
#add_compile_options(/wd4623)    # 'derived class' : default constructor was implicitly defined as deleted because a base class default constructor is inaccessible or deleted
#add_compile_options(/wd4625)    # 'xxx': copy constructor was implicitly defined as deleted 
#add_compile_options(/wd4626)    # 'xxx': assignment operator was implicitly defined as deleted
#add_compile_options(/wd4668)    # 'xxx' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#add_compile_options(/wd4710)    # 'xxx': function not inlined
#add_compile_options(/wd4774)    # 'string' : format string expected in argument number is not a string literal
add_compile_options(/wd4820)    # 'xxx': '4' bytes padding added after data member 'yyy'
#add_compile_options(/wd4996)    # 'xxx': This function or variable may be unsafe.
#add_compile_options(/wd5026)    # 'xxx': move constructor was implicitly defined as deleted
#add_compile_options(/wd5027)    # 'type': move assignment operator was implicitly defined as deleted
#add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc.
#add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#add_compile_options(/wd5219)
#add_compile_options(/wd26812)   # Prefer 'enum class' over 'enum' (Enum.3)

file(GLOB FDRIVE                            ../../Device/src/FDrive/*)

source_group(FDrive                              FILES ${FDRIVE})

add_executable(S8-53M ${FDRIVE})

target_link_libraries(S8-53M ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud)
target_link_libraries(S8-53M ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxmsw31ud_core)

target_link_libraries(S8-53M WinMM.lib)
target_link_libraries(S8-53M version.lib)
target_link_libraries(S8-53M Imm32.Lib)
target_link_libraries(S8-53M Setupapi.lib)
