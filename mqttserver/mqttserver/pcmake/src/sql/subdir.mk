################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sql/sqlWrapper.c \

OBJS += \
./src/sql/sqlWrapper.o \


C_DEPS += \
./src/sql/sqlWrapper.d \

# Each subdirectory must supply rules for building sources it contributes
src/sql/%.o: ../src/sql/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	$(GCC) $(CFLAGS) -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

