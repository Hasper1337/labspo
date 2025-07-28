NAME = lab3
SRC = $(NAME).c
EXEC = $(NAME).exe

CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -mwindows -finput-charset=UTF-8 -fexec-charset=CP1251 -DUNICODE -D_UNICODE

build_and_run: $(EXEC)
	wine $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(SRC) -o $(EXEC) $(CFLAGS)
	
clean:
	rm -f $(EXEC)
