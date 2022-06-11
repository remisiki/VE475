CC      = gcc

CFLAGS  = -std=c11 -Wconversion -Wall -Werror -Wextra -pedantic

LDLAGS  = 

TARGET  = ex4

SRCDIR  = ./src

SRCS    = $(wildcard $(SRCDIR)/*.c)

OBJDIR  = ./obj

OBJS    = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))
 
INCDIR  = -I./inc
 
LIBDIR  = 
 
LIBS    = 

$(TARGET): $(OBJS) $(LIBS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	-mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCDIR) -o $@ -c $<

all: clean $(OBJS) $(TARGET)

clean:
	-rm -f $(OBJS) $(TARGET) *.d