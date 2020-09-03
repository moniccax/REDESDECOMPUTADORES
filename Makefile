lsudp: socketclient.c socketserver.c
	gcc -o cliente socketclient.c -Wall -Wextra -pedantic
	gcc -o servidor socketserver.c -Wall -Wextra -pedantic
clean:
	rm socketclient *.o *~
	rm socketserver *.o *~
