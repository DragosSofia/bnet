build: bnet.c
	gcc bnet.c -o bnet -g -Wall

run: bnet
	./bnet

clean:
	rm bnet