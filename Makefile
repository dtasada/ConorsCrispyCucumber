build:
	g++ ./src/*.cpp -o ./HageveldSmashBros -std=c++23 -lSDL2 -lSDL2_image -Wall

run: build
	./HageveldSmashBros

clean:
	rm HageveldSmashBros
