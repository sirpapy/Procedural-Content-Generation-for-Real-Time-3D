OBJ = main.o
INC = -I "./"
Rasterrain : $(OBJ)
	g++ $(OBJ) -o main
	rm -f $(OBJ)
main.o:
	g++ -c main.cpp $(INC)
	
clean:
	rm -f $(OBJ) main
