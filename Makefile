build:
	mkdir -p ./target
	g++ ./src/*.cpp -o ./target/HageveldSmashBros -std=c++20 -lSDL2 -lSDL2_image -lSDL2_ttf -Wall -O3

run: build
	./target/HageveldSmashBros

clean:
	rm -r ./target
