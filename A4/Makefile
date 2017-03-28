DEPENDENCIES = hash.h ftree.h
PORT = 6969
FLAGS = -Wall -std=gnu99 -DPORT=$(PORT)

%.o: %.c ${DEPENDENCIES}
	gcc ${FLAGS} -c $<

all: rcopy_client rcopy_server

rcopy_client: rcopy_client.o ftree.o hash_functions.o
	gcc ${FLAGS} -o $@ $^

rcopy_server: rcopy_server.o ftree.o hash_functions.o
	gcc ${FLAGS} -o $@ $^

clean: 
	rm *.o rcopy_client rcopy_server
