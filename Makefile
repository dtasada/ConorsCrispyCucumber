build:
	g++ ./src/*.cpp -o asdf -lSDL2 -lSDL2_image -Wall

run: build
	./asdf

clean:
	rm asdf
