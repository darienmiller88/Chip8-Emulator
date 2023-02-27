all: compile link

compile:
	g++ -c *.cpp -IC:\SFML-2.5.1\include  

link:
	g++ -o main *.o -LC:\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system