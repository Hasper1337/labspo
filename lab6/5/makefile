EXEC_SEQ = matrix_seq
SRC_SEQ = $(EXEC_SEQ).c
EXEC_PAR_PT = matrix_pth
SRC_PAR_PT = $(EXEC_PAR_PT).c
CFLAGS_PT = -pthread -lm

all: $(EXEC_SEQ) $(EXEC_PAR_PT)
	@echo "Последовательно"
	@time -p ./$(EXEC_SEQ)
	@echo
	@echo "С использованием pthreads"
	@time -p ./$(EXEC_PAR_PT)

$(EXEC_SEQ): $(SRC_SEQ)
	gcc $< -o $@ -lm

$(EXEC_PAR_PT): $(SRC_PAR_PT)
	gcc $< -o $@ $(CFLAGS_PT)

clean:
	rm -f $(EXEC_SEQ)
	rm -f $(EXEC_PAR_PT)
