build:
	gcc process_generator.c -o process_generator.out
	gcc clk.c -o clk.out
	gcc scheduler.c -o scheduler.out -lm
	gcc process.c -o process.out
	gcc test_generator.c -o test_generator.out
	gcc memoryProcess.c -o memoryProcess.out

clean:
	rm -f *.out scheduler.log scheduler.perf memory.log

all: clean build

run:
	./process_generator.out
