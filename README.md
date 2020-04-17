# C-ClientServer

Its nothing but just a very simple TCP server and client model in shape of a chatting application.
Both server and client can send and receive messages.

serve.c and client.c needs to be complied seperately.

### $ gcc server.c -o server

### $ gcc clietnt.c -o client

## Usage:

### $ ./server portnumber
Portnumber can be any non reserved port number (1024-49151)

### $ ./client address portnumber
Address can be hostname or IP in case of local use localhost or 127.0.0.1 and port should be same of the server.

To close the port and end the session use
### You: exit()
From the message window from any of the side.

#### Gracias.
