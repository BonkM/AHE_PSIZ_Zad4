
.PHONY: all
all: dist/ber dist/gen test
	@ :

dist/ber: ber.cpp
	mkdir -p dist
	g++ --std=c++17 -o dist/ber.exe ber.cpp
	mv dist/ber.exe dist/ber

dist/gen: gen.cpp
	mkdir -p dist
	g++ --std=c++17 -o dist/gen.exe gen.cpp
	mv dist/gen.exe dist/gen

testdata/test-0-a: dist/gen
	mkdir -p testdata
	./dist/gen 'U' 4 > $@

testdata/test-0-b: dist/gen
	mkdir -p testdata
	./dist/gen 'U' 5 > $@

testdata/test-1-a: dist/gen
	mkdir -p testdata
	./dist/gen 'U' 100 > $@

testdata/test-1-b: dist/gen
	mkdir -p testdata
	./dist/gen 'U' 100 > $@

testdata/test-2-a: dist/gen
	mkdir -p testdata
	./dist/gen 'U' 100 > $@

testdata/test-2-b: dist/gen
	mkdir -p testdata
	./dist/gen 'U' 95 'V' 5 > $@

testdata/test-3-a: dist/gen
	mkdir -p testdata
	./dist/gen 'U' 400000000 > $@

testdata/test-3-b: dist/gen
	mkdir -p testdata
	./dist/gen 'P' 400000000 > $@

.PHONY: test
test: test-0 test-1 test-2 test-3
	@ :

.PHONY: test-0
test-0: dist/ber testdata/test-0-a testdata/test-0-b
	@ echo test-0
	./dist/ber testdata/test-0-a testdata/test-0-b | grep -q -F 'Different bits: 8'
	@ echo OK

.PHONY: test-1
test-1: dist/ber testdata/test-1-a testdata/test-1-b
	@ echo test-1
	./dist/ber testdata/test-1-a testdata/test-1-b | grep -q -F 'Different bits: 0'
	@ echo OK

.PHONY: test-2
test-2: dist/ber testdata/test-2-a testdata/test-2-b
	@ echo test-2
	./dist/ber testdata/test-2-a testdata/test-2-b | grep -q -F 'Different bits: 10'
	@ echo OK

.PHONY: test-3
test-3: dist/ber testdata/test-3-a testdata/test-3-b
	@ echo test-3
	./dist/ber testdata/test-3-a testdata/test-3-b | grep -q -F 'Different bits: 800000000'
	@ echo OK

.PHONY: clean
clean:
	rm -rf dist testdata
