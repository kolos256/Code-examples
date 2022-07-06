#path to arm-none-eabi-gcc
COMPILERPATH := D:/Various/notes/PROJECTS/PROJ-PYCAMERA/tools/gcc-arm-none-eabi-10.3-2021.10-win32/gcc-arm-none-eabi-10.3-2021.10/bin/
CC  := $(COMPILERPATH)arm-none-eabi-gcc
AS  := $(COMPILERPATH)arm-none-eabi-gcc
CPP := $(COMPILERPATH)arm-none-eabi-g++
CP 	:= $(COMPILERPATH)arm-none-eabi-objcopy
SZ 	:= $(COMPILERPATH)arm-none-eabi-size
DP 	:= $(COMPILERPATH)arm-none-eabi-objdump

CCFLAGS  := -Wfloat-equal -Wall -Wextra -march=armv6-m -mthumb -mcpu=cortex-m0 -mfloat-abi=soft -fmessage-length=0 -ffunction-sections
CPPFLAGS  := -Wfloat-equal -Wall -Wextra -march=armv6-m -mthumb -mcpu=cortex-m0 -mfloat-abi=soft -fmessage-length=0 -ffunction-sections
LDFLAGS  := --specs=nosys.specs --specs=nano.specs -Wl,-Map=output.map -Wl,--gc-sections -lc -lm -lnosys -T STM32F030F4Px_FLASH.ld

BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps

TARGET	 := Stepper_TEST1
INCLUDE  := -Iinc/ \
            -IDrivers/STM32F0xx_HAL_Driver/Inc/ \
			-IDrivers/STM32F0xx_HAL_Driver/Inc/Legacy \
			-IDrivers/CMSIS/Device/ST/STM32F0xx/Include \
			-IDrivers/CMSIS/Include

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
getfiles=$(filter-out $(call rwildcard,$1,$2), $(call rwildcard,$1,$3))

CSRC      := $(call getfiles,src/,"",*.c) \
			 $(call getfiles,Drivers/STM32F0xx_HAL_Driver/Src/,**template**,*.c) \
			 Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/system_stm32f0xx.c

CPPSRC	  :=

ASMSRC    := Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f030x6.s \

DEFINES	 := __weak=__attribute__((weak)) \
			__packed=__attribute__((__packed__)) \
			USE_HAL_DRIVER \
			STM32F030x6

DEPS 	:= $(CSRC:%.c=$(OBJ_DIR)/%.d)

OBJECTS := $(CSRC:%.c=$(OBJ_DIR)/%.o)
OBJECTS += $(ASMSRC:%.s=$(OBJ_DIR)/%.o)
OBJECTS += $(CPPSRC:%.cpp=$(OBJ_DIR)/%.o)

CCFLAGS += $(addsuffix ', $(addprefix '-D, $(DEFINES)))
CPPFLAGS += $(addsuffix ', $(addprefix '-D, $(DEFINES)))

all: build $(APP_DIR)/$(TARGET).elf $(APP_DIR)/$(TARGET).hex $(APP_DIR)/$(TARGET).dp size

TST1:
	@echo $(ASMSRC)
	@echo $(CSRC)
	@echo $(OBJECTS)

TST2: 
	@echo $(call getfiles,src/,*system* *timebase* ,*.c)

TST3: $(APP_DIR)/$(TARGET).elf
	$(DP) -h $(APP_DIR)/$(TARGET).elf

TST4: $(APP_DIR)/$(TARGET).elf
	$(DP) -t $(APP_DIR)/$(TARGET).elf

.PHONY: TST3 TST4

-include $(DEPS)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) $(INCLUDE) -MMD -MP -o $@ -c $<

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCLUDE) -MMD -MP -o $@ -c $<

$(OBJ_DIR)/%.o: %.s
	@mkdir -p $(@D)
	$(AS) $(CCFLAGS) $(INCLUDE) -o $@ -c $<

ifneq ($(CPPSRC),)

$(APP_DIR)/$(TARGET).elf: $(OBJECTS)
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGET).elf $(OBJECTS)

else

$(APP_DIR)/$(TARGET).elf: $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGET).elf $(OBJECTS)

endif


$(APP_DIR)/$(TARGET).hex: $(APP_DIR)/$(TARGET).elf
	@$(CP) -Oihex $(APP_DIR)/$(TARGET).elf $(APP_DIR)/$(TARGET).hex

$(APP_DIR)/$(TARGET).dp: $(APP_DIR)/$(TARGET).elf
	$(DP) -D $(APP_DIR)/$(TARGET).elf > $(APP_DIR)/$(TARGET).dp

.PHONY: all build clean debug release size TST1

size: $(APP_DIR)/$(TARGET).elf
	@echo "----------------------------------------------------------------------"
	@$(SZ) $(APP_DIR)/$(TARGET).elf

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CCFLAGS += -DDEBUG -g -Og -gdwarf-2
debug: all

release: CCFLAGS += -Og
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
