test: Makefile src/test.cpp ../Copter.h ../Copter.cpp
	mingw32-g++ -std=c++03 -c ../Copter.cpp -o obj/Copter.o
	ar cr lib/Copter.lib obj/Copter.o
	ranlib lib/Copter.lib
	mingw32-g++ -std=c++03 -Wall src/test.cpp lib/Copter.lib -o bin/test.exe
	bin/test.exe

clean:
	del /q/f obj\*.o
	del /q/f lib\*.lib