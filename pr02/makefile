all:
	# build the client and create a link as client.bin
	make -C ./client/
	rm -rf client.bin
	ln -s ./client/client ./client.bin
	# build the server and create a link as server.bin
	make -C ./server/
	rm -rf server.bin
	ln -s ./server/server ./server.bin

clean:
	rm -rf client.bin
	rm -rf server.bin
	make clean -C ./client/
	make clean -C ./server/
