CROSS_COMPILE_ROOT	= $(ARM_NONE_EABI_ROOT)
ROOTDIR 			= .
BUILD_DIR			= $(ROOTDIR)/build
OBJECT_DIR			= $(BUILD_DIR)/obj
TARGET				= main
OPT					= -O0
DEBUG				= 1

CROSS_PREFIX		= arm-none-eabi-
CC					= $(CROSS_PREFIX)gcc		# 编译器，.c 转 .o 
CXX					= $(CROSS_PREFIX)g++
AS					= $(CROSS_PREFIX)gcc -x assembler-with-cpp	# 汇编器，.s 转 .o 
# AS					= $(CROSS_PREFIX)as			# 汇编器，.s 转 .o 
AR					= $(CROSS_PREFIX)ar			# 归档器，生成静态库（.a）
LD					= $(CROSS_PREFIX)ld			# 链接器，链接目标文件（.o）、静态库（.a）或者共享库（.so）生成可执行文件（.elf）或共享库（.so）
OBJDUMP				= $(CROSS_PREFIX)objdump	# 用于显示目标文件各个部分/节的内容，包括机器指令、符号表、头部信息等
CP					= $(CROSS_PREFIX)objcopy	# 用于复制或者转换目标文件，在复制/转换过程中，可以有选择的复制或者修改，然后保存到另一个文件中
STRIP				= $(CROSS_PREFIX)strip		# 用于去除目标文件的符号信息和调试信息，从而减小目标文件大小
SZ					= $(CROSS_PREFIX)size		# 用于显示目标文件各个节（Sections）的大小，常用于分析可执行文件、目标文件或库文件的大小，以便于了解程序内存占用

HEX					= $(CP) -O ihex
BIN					= $(CP) -O binary -S

CPU					= -mthumb -mcpu=cortex-m3
FLOAT-ABI 			= 
MCU					= $(CPU) $(FLOAT-ABI)

# C_DEFS
C_DEFS				= \
-DSTM32F10X_MD \
-DUSE_STDPERIPH_DRIVER
ASM_DEFS			= 

# C_SOURCES
# C_SOURCES			= $(wildcard User/*.c)
# C_SOURCES			= $(wildcard STM32F10x_LIB/periph/src/*.c)
# $(foreach item,${C_SOURCES}, $(info $(item) \))
ASM_SOURCES			= $(ROOTDIR)/lib/start/startup_stm32f10x_md.s
C_SOURCES			= \
main.c \
lib/start/stm32f10x_it.c \
lib/start/system_stm32f10x.c \
lib/spl/src/misc.c \
lib/spl/src/stm32f10x_adc.c \
lib/spl/src/stm32f10x_bkp.c \
lib/spl/src/stm32f10x_can.c \
lib/spl/src/stm32f10x_cec.c \
lib/spl/src/stm32f10x_crc.c \
lib/spl/src/stm32f10x_dac.c \
lib/spl/src/stm32f10x_dbgmcu.c \
lib/spl/src/stm32f10x_dma.c \
lib/spl/src/stm32f10x_exti.c \
lib/spl/src/stm32f10x_flash.c \
lib/spl/src/stm32f10x_fsmc.c \
lib/spl/src/stm32f10x_gpio.c \
lib/spl/src/stm32f10x_i2c.c \
lib/spl/src/stm32f10x_iwdg.c \
lib/spl/src/stm32f10x_pwr.c \
lib/spl/src/stm32f10x_rcc.c \
lib/spl/src/stm32f10x_rtc.c \
lib/spl/src/stm32f10x_sdio.c \
lib/spl/src/stm32f10x_spi.c \
lib/spl/src/stm32f10x_tim.c \
lib/spl/src/stm32f10x_usart.c \
lib/spl/src/stm32f10x_wwdg.c \
src/delay.c \
src/buzzer.c \

# C_INCLUDES 
C_INCLUDES 			= \
-Ilib/CoreSupport \
-Ilib/spl/inc \
-Ilib/start \
-Iinc \

ASM_INCLUDES		=

# CFLAGS
CFLAGS				= $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS				+= -MMD -MP -MF"$(@:%.o=%.d)"	# 产生头文件依赖，.d 保存在与 .o相同的目录下 
ASFLAGS				= $(MCU) $(ASM_DEFS) $(ASM_INCLUDES) 
ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2	# -g 生成调试信息，	-gdwarf-2 指定调试信息的格式
endif

# LD FLAGS
LDSCRIPT			= $(ROOTDIR)/lib/start/stm32_flash.ld
LIBDIR				= \
-L$(CROSS_COMPILE_ROOT)/arm-none-eabi/lib \
-L$(CROSS_COMPILE_ROOT)/lib/gcc/arm-none-eabi/14.2.0
LIBS				= -lc -lm -lgcc
LDFLAGS				= -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# OBJECTS
OBJECTS 			= $(addprefix $(OBJECT_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
OBJECTS 			+= $(addprefix $(OBJECT_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(OBJECT_DIR)/%.o: %.c Makefile | $(OBJECT_DIR) 
	@$(CC) -c $< $(CFLAGS) -o $@

$(OBJECT_DIR)/%.o: %.s Makefile | $(OBJECT_DIR) 
	@$(CC) -x assembler-with-cpp -c $< $(CFLAGS) -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@$(CC) $^ $(LDFLAGS) -o $@
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	@$(HEX) $^ $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@$(BIN) $^ $@

$(OBJECT_DIR):
	@mkdir -p $@

.PHONY:clean
clean:
	@rm -rf $(BUILD_DIR)/*

-include $(wildcard $(OBJECT_DIR)/*.d)
