# 定义变量
CC = gcc
CFLAGS = -I1-include
LDFLAGS = -lwiringPi
OBJDIR = 3-obj
SRCDIR = 2-src
TARGET = exec

SRCS = main.c $(SRCDIR)/ST7796U.c

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 生成目标文件
$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)  # 确保目标目录存在
	$(CC) $(CFLAGS) -c -o $@ $<

# 清理生成的文件
clean:
	rm -f $(TARGET) $(OBJDIR)/*.o

# 伪目标
.PHONY: all clean
