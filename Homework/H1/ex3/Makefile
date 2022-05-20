CC      = g++

CFLAGS  = -std=c++11 -Wconversion -Wall -Werror -Wextra -pedantic

LDLAGS  = 

TARGET  = ex3

SRCDIR  = ./src

SRCS    = $(wildcard $(SRCDIR)/*.cpp)

OBJDIR  = ./obj

OBJS    = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))
 
INCDIR  = -I./inc
 
LIBDIR  = 
 
LIBS    = -lgmpxx -lgmp

$(TARGET): $(OBJS) $(LIBS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	-mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCDIR) -o $@ -c $<

all: clean $(OBJS) $(TARGET)

clean:
	-rm -f $(OBJS) $(TARGET) *.d