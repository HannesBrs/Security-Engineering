$(CC) = gcc

#compile object files to executable named inode
inode: main.o
	$(CC) main.o -o inode

#compile main.c into main.o object files
main.o: main.c
	$(CC) -c main.c

#removes all object files and the executable
clean:
	rm *.o inode
