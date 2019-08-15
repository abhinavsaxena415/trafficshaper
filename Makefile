trafficshaper: trafficshaper.o queueFIFO.o requestQueue.o tokenQueue.o server.o	
	gcc -o trafficshaper -g trafficshaper.o queueFIFO.o requestQueue.o tokenQueue.o server.o -lpthread

trafficshaper.o: trafficshaper.c header.h queueFIFO.h
	gcc -g -c -Wall trafficshaper.c

queueFIFO.o: queueFIFO.c queueFIFO.h header.h
	gcc -g -c -Wall queueFIFO.c

requestQueue.o: requestQueue.c header.h queueFIFO.h
	gcc -g -c -Wall requestQueue.c

tokenQueue.o: tokenQueue.c header.h queueFIFO.h
	gcc -g -c -Wall tokenQueue.c

server.o: server.c header.h queueFIFO.h
	gcc -g -c -Wall server.c

clean:
	@rm -f $(PROGRAMS) *.o *.gch core trafficshaper


