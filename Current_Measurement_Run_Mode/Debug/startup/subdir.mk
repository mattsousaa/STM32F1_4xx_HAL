################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f446xx.s 

OBJS += \
./startup/startup_stm32f446xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -I"C:/Users/mateu/Documents/Curso STM32/MCU2/Current_Measurement_Run_Mode/StdPeriph_Driver/inc" -I"C:/Users/mateu/Documents/Curso STM32/MCU2/Current_Measurement_Run_Mode/inc" -I"C:/Users/mateu/Documents/Curso STM32/MCU2/Current_Measurement_Run_Mode/CMSIS/device" -I"C:/Users/mateu/Documents/Curso STM32/MCU2/Current_Measurement_Run_Mode/CMSIS/core" -x assembler-with-cpp --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

