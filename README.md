# Introduction
This is a dicing game written in C socket and Linux commands.

# Run the game:
1. Download the client.c and server.c to local Linux machines.
2. "gcc server.c -o server" and "./server port1" -- substitute 'port1' with a real port
3. "gcc client.c -o client" and "./client ip port1" -- change 'ip' and 'port1' with the real ip and port of the server 
4. client hits the enter key first, then the server hit, and take turns afterward.
