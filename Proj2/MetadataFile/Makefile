caldata: caldata.o writedata
	gcc caldata.o -o caldata -g
writedata: writedata.o
	gcc writedata.o -o writedata -g
caldata.o: caldata.c
	gcc -c caldata.c -g
writedata.o: writedata.c
	gcc -c writedata.c -g
clean:
	rm -f *.o caldata writedata
