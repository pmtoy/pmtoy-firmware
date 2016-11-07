PROJECT(pmtoy C)
CMAKE_MINIMUM_REQUIRED(VERSION 3.3)
SET(STM32CubeF0_FW ${CMAKE_SOURCE_DIR}/../STM32Cube_FW_F0_V1.6.0)
SET(STLINK_BIN ${CMAKE_SOURCE_DIR}/../stlink/st-flash)

IF(NOT EXISTS "${STM32CubeF0_FW}")
	MESSAGE( FATAL_ERROR "Not found STM32CubeF0 FW 1.6.0, please download from st.com." )
ENDIF()

IF(NOT EXISTS "${STLINK_BIN}")
	MESSAGE( WARNING "Not found st-flash, disable flash command." )
ENDIF()

option(BUILD_USB_VCP "Build USB VCP feature" ON)

INCLUDE_DIRECTORIES(
    ${STM32CubeF0_FW}/Drivers/CMSIS/Include/
    ${STM32CubeF0_FW}/Drivers/CMSIS/Device/ST/STM32F0xx/Include/
    ${STM32CubeF0_FW}/Drivers/STM32F0xx_HAL_Driver/Inc/

    ${STM32CubeF0_FW}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/
    ${STM32CubeF0_FW}/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/

    Inc
    Lib
)
SET(HAL_LIB_SRC ${STM32CubeF0_FW}/Drivers/STM32F0xx_HAL_Driver/Src/)
SET(USB_LIB_SRC ${STM32CubeF0_FW}/Middlewares/ST/STM32_USB_Device_Library/)

SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_BUILD_TYPE DEBUG)
SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_C_COMPILER "arm-none-eabi-gcc")
SET(CMAKE_LINKER "arm-none-eabi-ld")
SET(CMAKE_OBJCOPY "arm-none-eabi-objcopy")
SET(CMAKE_SIZE "arm-none-eabi-size")


SET(LINK_SCRIPT ${CMAKE_SOURCE_DIR}/Boot/STM32F070F6_FLASH.ld)
SET(FAMILY "STM32F070x6")

SET(STARTUP Boot/startup_stm32f070x6.s Boot/system_stm32f0xx.c)
SET(STARTUP ${STARTUP}
    ${HAL_LIB_SRC}/stm32f0xx_hal.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_cortex.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_rcc.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_rcc_ex.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_gpio.c

    ${HAL_LIB_SRC}/stm32f0xx_hal_dma.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_pwr.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_pcd.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_pcd_ex.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_i2c.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_spi.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_spi_ex.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_tim.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_tim_ex.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_uart.c
    ${HAL_LIB_SRC}/stm32f0xx_hal_uart_ex.c
)

IF(BUILD_USB_VCP)
SET(STARTUP ${STARTUP}
    ${USB_LIB_SRC}/Class/CDC/Src/usbd_cdc.c
    ${USB_LIB_SRC}/Core/Src/usbd_core.c
    ${USB_LIB_SRC}/Core/Src/usbd_ctlreq.c
    ${USB_LIB_SRC}/Core/Src/usbd_ioreq.c
)
ENDIF(BUILD_USB_VCP)

SET_PROPERTY(SOURCE Boot/startup_stm32f070x6.s PROPERTY LANGUAGE C)
SET(SRC main.c stm32f0xx_it.c stm32f0xx_hal_msp.c)


IF(BUILD_USB_VCP)
SET(USB_SRC usbd_cdc_interface.c usbd_conf.c usbd_desc.c)
ENDIF(BUILD_USB_VCP)

SET(CMAKE_C_FLAGS_DEBUG "-O0 -g -ggdb")
SET(CMAKE_VERBOSE_MAKEFILE ON)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES pmtoy.map)

SET(MCU_FLAGS "-mlittle-endian -mcpu=cortex-m0 -march=armv6-m -mthumb -mfloat-abi=soft ")
SET(CMAKE_C_FLAGS "${MCU_FLAGS} -Wall --std=gnu99 -Wl,--gc-sections -ffreestanding -fno-builtin  -ffunction-sections -fdata-sections")
ADD_DEFINITIONS(-D${FAMILY} -DUSE_HAL_DRIVER)

IF(BUILD_USB_VCP)
    ADD_DEFINITIONS(-DUSE_USB_VCP)
ENDIF(BUILD_USB_VCP)

SET(CMAKE_EXE_LINKER_FLAGS "--specs=nano.specs -lc -lnosys -static -Wl,-Map=pmtoy.map -nostartfiles -Wl,--gc-section -T${LINK_SCRIPT}")
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

CMAKE_POLICY(SET CMP0015 OLD)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

ADD_EXECUTABLE(${PROJECT_NAME}.elf ${STARTUP} ${SRC} ${USB_SRC})

ADD_CUSTOM_COMMAND(DEPENDS ${PROJECT_NAME}.elf OUTPUT ${PROJECT_NAME}.hex COMMAND ${CMAKE_OBJCOPY} -Oihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex)
ADD_CUSTOM_COMMAND(DEPENDS ${PROJECT_NAME}.elf OUTPUT ${PROJECT_NAME}.bin COMMAND ${CMAKE_OBJCOPY} -Obinary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin)
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf POST_BUILD COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf)

ADD_CUSTOM_TARGET(generate_bin ALL DEPENDS ${PROJECT_NAME}.bin)
ADD_CUSTOM_TARGET(generate_hex ALL DEPENDS ${PROJECT_NAME}.hex)

ADD_CUSTOM_TARGET(erase ${STLINK_BIN} erase)
ADD_CUSTOM_TARGET(flash ${STLINK_BIN} write ${PROJECT_NAME}.bin 0x8000000 depends ${PROJECT_NAME}.bin)
