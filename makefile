BUILD_DIR = build

RELEASE_BIN = ${BUILD_DIR}/a.out

first: build

release: ${RELEASE_BIN}


a.out: src/main.cpp
	g++ src/main.cpp

clean:
	rm -rf *.o
