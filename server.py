import socket

# Listen for incoming connections on port 52727
# Not going to worry about multithreading for now

# Some useful variables
serv_port = 52727
serv_addr = '' # Listen on all interfaces

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = (serv_addr, serv_port) # Tuple of server address and port
print('starting up on %s port %s' % server_address)
sock.bind(server_address)

# Listen for incoming connections
sock.listen(1)

while True:
    # Wait for a connection
    print('waiting for a connection')
    connection, client_address = sock.accept()
    try:
        print('connection from', client_address)
        # Get user input, send to client, and receive response
        while True:
            data = input("Enter message to send: ")
            connection.sendall(data.encode())
            data = connection.recv(1024)
            print('Received "%s"' % data.decode())
    finally:
        # Clean up the connection
        connection.close()