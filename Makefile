CC = clang

BIN = interpreter
FILE = interpreter.c 
#PROGRAM = interpreter.o
TEST_FILE = trial.bf

all: main test

main: 
	${CC} -g  -o ${BIN} ${FILE} 

test:
	./${BIN} ${TEST_FILE}

clean: 
	rm ${BIN} ${PROGRAM}

