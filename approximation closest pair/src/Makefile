CXX = g++ -O3 -lm
SRCS = *.cpp
OBJS = $(SRCS:.o)

cp: $(OBJS)
	$(CXX) $(SRCS) -o cp $(OBJS)

clean:
	rm -f *.o
	rm -f cp

run:
	./cp -n 60000 -d 784 -f Mnist.ds
