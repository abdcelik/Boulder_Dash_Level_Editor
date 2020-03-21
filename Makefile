CC := gcc

CFLAGS := -lSDL2 -lSDL2_image

HDRS := ./Sources/LE_basis.h

SRCS := ./Sources/main.c ./Sources/LE_basis.c

OBJS := $(SRCS:.c=.o)

EXEC := LevelEditor

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

