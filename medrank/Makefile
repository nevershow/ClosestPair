CXX = g++ -std=c++11 -O3
SRCS = src/block_file.cpp src/B_Node.cpp src/B_Tree.cpp src/PreProcess.cpp src/medrankAlgorithm.cpp src/main.cpp
OBJS = $(SRCS:.cpp=.o)

medrank: $(OBJS)
	$(CXX) -o medrank $(OBJS)

main: src/main.cpp
medrankAlgorithm: src/medrankAlgorithm.h
PreProcess: src/PreProcess.h
B_Tree: src/B_Tree.h
B_Node: src/B_Node.h
block_file: src/block_file.h
clean:
	rm -f src/*.o
	rm -f *.bin
run:
	rm -f *.bin
	./medrank -n 60000 -d 784 -qn 100 -ds ./data/Mnist.ds -qs ./data/Mnist.q
