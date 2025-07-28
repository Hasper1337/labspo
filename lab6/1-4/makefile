EXEC_SEQ = posled
SRC_SEQ = $(EXEC_SEQ).c
EXEC_PAR_PT = pth
SRC_PAR_PT = $(EXEC_PAR_PT).c
EXEC_PAR_OMP = openmp
SRC_PAR_OMP = $(EXEC_PAR_OMP).c
CFLAGS_OMP = -fopenmp -lm
CFLAGS_PT = -pthread -lm

all: $(EXEC_SEQ) $(EXEC_PAR_PT) $(EXEC_PAR_OMP)
	@echo "Последовательно"
	@time -p ./$(EXEC_SEQ)
	@echo "С использованием pthreads"
	@time -p ./$(EXEC_PAR_PT)
	@echo "С использованием OpenMP"
	@time -p ./$(EXEC_PAR_OMP)

$(EXEC_SEQ): $(SRC_SEQ)
	gcc $< -o $@ -lm

$(EXEC_PAR_PT): $(SRC_PAR_PT)
	gcc $< -o $@ $(CFLAGS_PT)

$(EXEC_PAR_OMP): $(SRC_PAR_OMP)
	gcc $< -o $@ $(CFLAGS_OMP)

clean:
	rm -f $(EXEC_SEQ)
	rm -f $(EXEC_PAR_PT)
	rm -f $(EXEC_PAR_OMP)
