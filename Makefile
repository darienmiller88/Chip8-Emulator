all: compile link

compile:
	g++ -c *.cpp -I./include  

link:
	g++ -o main *.o -L./lib -lsfml-graphics -lsfml-window -lsfml-system