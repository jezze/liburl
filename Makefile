BIN:=test
OBJ:=test.o liburl.o

all: $(BIN)

%.o: %.c
	@echo CC $@
	@$(CC) -c -o $@ -Wall -Werror -pedantic $(CFLAGS) $^

clean:
	@$(RM) $(BIN) $(OBJ)

$(BIN): $(OBJ)
	@echo LD $@
	@$(CC) -o $@ -Wall -Werror -pedantic $^ -lm

