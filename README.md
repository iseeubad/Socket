Sure! Below is a comprehensive and well-structured README file for your project.

---

# Advanced C Language Mini-Project: Sockets and Data Structures

## Description

This project involves creating two applications using advanced C programming techniques: a client application and a server application. The applications communicate through sockets, allowing for user authentication and data management. The project is divided into two parts: the client-side and the server-side.

## Table of Contents

1. [General Information](#general-information)
2. [Technologies](#technologies)
3. [Setup](#setup)
4. [Usage](#usage)
5. [Features](#features)
6. [Contributing](#contributing)
7. [Contact](#contact)

## General Information

### Client-Side

The client-side application performs the following functions:
1. Connects to the server socket.
2. Prompts the user to enter a login and password.
   - Allows a maximum of 3 attempts if the password is incorrect.
3. Retrieves the user profile (Admin or Guest) based on login credentials.
4. Displays a menu based on the user profile.
   - Admin: Can manage contacts (add, modify, delete, search).
   - Guest: Can only view and search contacts.
5. Sends user input data to the server for processing.

### Server-Side

The server-side application performs the following functions:
1. Listens for client connections.
2. Authenticates clients based on login and password.
   - Displays a custom menu for Admin or Guest users.
3. Displays real-time client history.
4. Manages contact data: allows adding, modifying, deleting, and searching contacts.
5. Processes client requests and updates data accordingly.

## Technologies

The project is created with:
- C programming language
- Socket programming
- Data structures

## Setup

To run this project, follow these steps:

1. Clone the repository:
   ```bash
   git clone https://github.com/iseeubad/Socket.git
   ```

2. Navigate to the project directory:
   ```bash
   cd Socket
   ```

3. Compile the client and server applications:
   ```bash
   gcc -o client client.c
   gcc -o server server.c
   ```

4. Run the server application:
   ```bash
   ./server
   ```

5. In a new terminal, run the client application:
   ```bash
   ./client
   ```

## Usage

1. **Client Authentication**:
   - Enter your login credentials.
   - If the password is incorrect, you have up to 3 attempts to retry.
   - Upon successful login, your profile (Admin or Guest) will be retrieved.

2. **Admin Menu**:
   - Add contacts
   - Modify contacts
   - Delete contacts
   - Search contacts

3. **Guest Menu**:
   - View contacts
   - Search contacts

4. **Server Operations**:
   - Receives and processes client requests.
   - Manages contact data in `contacts.txt`.
   - Displays real-time client request history.

## Features

- Secure login system with multiple attempts
- Profile-based menu options
- Real-time server-client communication
- Contact management (add, modify, delete, search)

## Contributing

Contributions are welcome! Please fork this repository and submit pull requests with proposed changes.

1. Fork the repository.
2. Create a new branch: `git checkout -b feature/your-feature`.
3. Commit your changes: `git commit -m 'Add some feature'`.
4. Push to the branch: `git push origin feature/your-feature`.
5. Open a pull request.

## Contact

 [Mohamed Salim Soulmani](https://github.com/iseeubad) - feel free to contact me!

---
