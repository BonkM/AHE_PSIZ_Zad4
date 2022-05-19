
.PHONY: all
all: dist/ber

dist/ber: ber.cpp
	mkdir -p dist
	g++ --std=c++17 -o dist/ber.exe ber.cpp
	mv dist/ber.exe dist/ber

.PHONY: clean
clean:
	rm -rf dist
