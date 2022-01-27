################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/log/elog.c \
../src/log/elog_buf.c \
../src/log/elog_async.c \
../src/log/elog_port.c \
../src/log/elog_file.c \
../src/log/elog_file_port.c \
../src/log/elog_utils.c

OBJS += \
./src/log/elog.o \
./src/log/elog_buf.o \
./src/log/elog_async.o \
./src/log/elog_file.o \
./src/log/elog_file_port.o \
./src/log/elog_port.o \
./src/log/elog_utils.o


C_DEPS += \
./src/log/elog.d \
./src/log/elog_buf.d \
./src/log/elog_async.d \
./src/log/elog_file.d \
./src/log/elog_file_port.d \
./src/log/elog_port.d \
./src/log/elog_utils.d


# Each subdirectory must supply rules for building sources it contributes
src/log/%.o: ../src/log/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	$(GCC) $(CFLAGS) -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

