OBJ = main.o
INC = -I "./"
Rasterrain : $(OBJ)
	g++ $(OBJ) -o lray
	rm -f $(OBJ)
main.o:
	g++ -c main.cpp $(INC) -std=c++11 -ggdb
	
clean:
	rm -f $(OBJ) lray
