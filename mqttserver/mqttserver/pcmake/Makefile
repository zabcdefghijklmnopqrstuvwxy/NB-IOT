################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/log/subdir.mk
-include src/server/subdir.mk
-include src/sql/subdir.mk
-include src/common/subdir.mk
-include subdir.mk
-include objects.mk
-include comm.mk


ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: mqttserver

# Tool invocations
mqttserver: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	@echo 'Invoking: $(LDFLAGS)'
	$(GCC) $(LDFLAGS) -o "mqttserver" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) mqttserver
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets

