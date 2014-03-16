CC = g++ 
CFLAGS = -Wall -W
TARGETS = traceroute
 
all: $(TARGETS)

traceroute: traceroute.o icmp.o echo_request.o sockwrap.o icmp.o icmp_receive.o timeval_op.o

clean:
	rm -f *.o	

distclean: clean
	rm -f $(TARGETS)
