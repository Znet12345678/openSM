CC = gcc
AR = ar
LDFLAGS =  -L${PWD} -L${PWD}/.. -lsm -Wl,-rpath=${PWD}
CFLAGS = -Werror -I../include -O2 -g -fPIC -std=c99
PREFIX=/usr/local
tests:
	@echo "LDCC opensm-libio-test-txt"
	@${CC} -o opensm-libio-test-txt  opensm-libio-test-txt.c ${CFLAGS} ${LDFLAGS}
	@echo "TEST opensm-libio-test-txt"
	@./opensm-libio-test-txt
	@echo "LDCC opensm-libio-test-file"
	@${CC} -o opensm-libio-test-file opensm-libio-test-file.c ${CFLAGS} ${LDFLAGS}
	@echo "TEST opensm-lbio-test-file"
	@./opensm-libio-test-file
	@echo "LDCC opensm-libuser-test"
	@${CC} -o opensm-libuser-test opensm-libuser-test.c ${CFLAGS} ${LDFLAGS}
	@echo "TEST opensm-libuser-test"
	@./opensm-libuser-test
