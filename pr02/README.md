## Ian Malerich - CS352 - Project 02

This project contains to applications which need
to be built, the client and the server. Each has its
own makefile residing in their respective directories.
There is also a makefile residing in the root directory,
this references those two makefiles to build both the client 
and the server. Binaries are still generated in the subdirectories
but the root makefile will make a symbolic link to a 'client.bin' and
'server.bin' application at the root level of this project for 
convenience.

This application provides a default ip address when you jus thit enter,
as discussed below however though this worked fine on my remote machine
when logged into pyrite I found I had to be explicit about the ip address
I was connecting to to get it to work, particularly when running 
the client and server in different ssh sessions.

## client/main.c

Source code for the application. Requests a server from the user,
you can just press enter to skip this step if you are running the
server and client on the same machine. Once a connection is
established, the client loops reading messages from the standard input
and sending them to the server along wiht the digital signature.
The servers response is then written to the standard output.

When prompted for a message you can type 'exit' to terminate the client
program and close the connection with the server.

## server/main.c

Source code for the server. Pretty straightforward, the main
method sets up the connection and initates an infinite loop
which sits waiting for clients. When a client arrives, a new
thread or process (specified at compile time, see "Extra Credit") 
is created to handle that request.

## shared/util.c

Some shared code between the two. Just some string parsing.
For each line of data sente by the client, it needs to send
both the message and the signature, these are separated
by a null terminating character. This file provides
a utility for reading up to and including the null terminator.
The utility also checks for the EOF signal which the server
uses to learn that a client has disconnected. The thread/process
handling that particular client can then terminate.

## Extra Credit

I have completed the extra credit by using pthreads on the server
rather than forking. However I do support both and you can switch
between processes vs threading at compile time. The makefile
located at ./server/makefile includes the line:
	"THREADS = -D '__USE_THREADING'
This defines an environment variable which tells my program
to use threading. If you comment/delete this line, the program
will instead use fork() to create processes when compiled.
