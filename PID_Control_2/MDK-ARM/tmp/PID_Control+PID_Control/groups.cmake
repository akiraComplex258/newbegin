# groups.cmake

# group Application/MDK-ARM
add_library(Group_Application_MDK-ARM OBJECT
  "${SOLUTION_ROOT}/startup_stm32f407xx.s"
)
target_include_directories(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Application_MDK-ARM_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Application_MDK-ARM_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Application_MDK-ARM PUBLIC
  Group_Application_MDK-ARM_ABSTRACTIONS
)
set(COMPILE_DEFINITIONS
  STM32F407xx
  _RTE_
)
cbuild_set_defines(AS_ARM COMPILE_DEFINITIONS)
set_source_files_properties("${SOLUTION_ROOT}/startup_stm32f407xx.s" PROPERTIES
  COMPILE_FLAGS "${COMPILE_DEFINITIONS}"
)

# group Application/User/Core
add_library(Group_Application_User_Core OBJECT
  "${SOLUTION_ROOT}/../Core/Src/main.c"
  "${SOLUTION_ROOT}/../Core/Src/gpio.c"
  "${SOLUTION_ROOT}/../Core/Src/can.c"
  "${SOLUTION_ROOT}/../Core/Src/dma.c"
  "${SOLUTION_ROOT}/../Core/Src/tim.c"
  "${SOLUTION_ROOT}/../Core/Src/usart.c"
  "${SOLUTION_ROOT}/../Core/Src/stm32f4xx_it.c"
  "${SOLUTION_ROOT}/../Core/Src/stm32f4xx_hal_msp.c"
)
target_include_directories(Group_Application_User_Core PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Application_User_Core PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Application_User_Core_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Application_User_Core_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Application_User_Core PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Application_User_Core PUBLIC
  Group_Application_User_Core_ABSTRACTIONS
)
set_source_files_properties("${SOLUTION_ROOT}/../Core/Src/dma.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Core/Src/tim.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Core/Src/usart.c" PROPERTIES
  COMPILE_OPTIONS ""
)

# group Drivers/STM32F4xx_HAL_Driver
add_library(Group_Drivers_STM32F4xx_HAL_Driver OBJECT
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c"
)
target_include_directories(Group_Drivers_STM32F4xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Drivers_STM32F4xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Drivers_STM32F4xx_HAL_Driver_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Drivers_STM32F4xx_HAL_Driver_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Drivers_STM32F4xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Drivers_STM32F4xx_HAL_Driver PUBLIC
  Group_Drivers_STM32F4xx_HAL_Driver_ABSTRACTIONS
)
set_source_files_properties("${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c" PROPERTIES
  COMPILE_OPTIONS ""
)

# group Drivers/CMSIS
add_library(Group_Drivers_CMSIS OBJECT
  "${SOLUTION_ROOT}/../Core/Src/system_stm32f4xx.c"
)
target_include_directories(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Drivers_CMSIS_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Drivers_CMSIS_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Drivers_CMSIS PUBLIC
  Group_Drivers_CMSIS_ABSTRACTIONS
)

# group application
add_library(Group_application OBJECT
  "${SOLUTION_ROOT}/../application/CAN_receive.c"
  "${SOLUTION_ROOT}/../application/serialplot.c"
  "${SOLUTION_ROOT}/../application/uart_dma.c"
)
target_include_directories(Group_application PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_application PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_application_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_application_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_application PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_application PUBLIC
  Group_application_ABSTRACTIONS
)

# group bsp
add_library(Group_bsp OBJECT
  "${SOLUTION_ROOT}/../bsp/boards/bsp_can.c"
)
target_include_directories(Group_bsp PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_bsp PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_bsp_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_bsp_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_bsp PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_bsp PUBLIC
  Group_bsp_ABSTRACTIONS
)

# group pidcontrol
add_library(Group_pidcontrol OBJECT
  "${SOLUTION_ROOT}/../pidcontrol/pid_control.c"
)
target_include_directories(Group_pidcontrol PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_pidcontrol PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_pidcontrol_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_pidcontrol_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_pidcontrol PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_pidcontrol PUBLIC
  Group_pidcontrol_ABSTRACTIONS
)
