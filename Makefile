all: compile link

compile:
	g++ -I"C:\Cpplib\SFML-2.5.1\include" -c main.cpp

link:
	g++ main.o -o main -L"C:\Cpplib\SFML-2.5.1\lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
