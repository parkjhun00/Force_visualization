import socket
import matplotlib.pyplot as plt

# Set up the server
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("localhost", 12345))  # Must match the C++ program
server_socket.listen(1)

fz_values = []

while True:
    # Wait for a connection
    client_socket, address = server_socket.accept()

    # Read data from the client
    data = client_socket.recv(1024)
    while data:
        fz_values.append(float(data))
        data = client_socket.recv(1024)

    client_socket.close()

    # Plot the data
    plt.plot(fz_values)
    plt.xlabel('Count')
    plt.ylabel('Fz')
    plt.show()
