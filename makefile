compile:
	g++ VigenereDecrypt.cpp -c
	g++ driver.cpp VigenereDecrypt.o -o decrypt.exe

run:
	./decrypt.exe

clean:
	rm *.exe
