# Distributed Chat Application

A distributed real-time chat application built using a multi-threaded C++ TCP server and a Python client with both CLI and GUI support.

This project demonstrates:

- TCP socket programming
- Multi-threaded server architecture
- Cross-language communication (C++ + Python)
- JSON-based messaging protocol
- Real-time messaging
- GUI application development
- Thread synchronization and concurrency

---

# Features

## C++17 Multi-Threaded Server

- Handles multiple concurrent clients
- Thread-safe client management using `std::mutex`
- Dedicated thread per client using `std::thread`
- Real-time message broadcasting
- Chat logging to `chat_log.txt`
- Cross-platform socket support:
  - Linux
  - macOS
  - Windows

---

## Python Client

### CLI Mode

- Lightweight terminal chat interface
- Non-blocking message receive/send using threading
- JSON protocol support

### GUI Mode

- User-friendly graphical interface
- Real-time incoming messages
- Clean chat window layout
- Username support
- Responsive message updates using threads

---

# Tech Stack

| Component | Technology |
|----------|-------------|
| Backend Server | C++17 |
| Frontend Client | Python 3 |
| Networking | TCP Sockets |
| Communication Protocol | JSON |
| Concurrency | Threads + Mutex |
| GUI | Tkinter / PyQt *(replace with yours)* |

---

# Project Structure

```text
distributed-chat-app/
│
├── server.cpp
├── client.py
├── gui_client.py
├── Makefile
├── README.md
├── .gitignore
└── chat_log.txt
```

---

# Requirements

## Server

- g++
- C++17 compatible compiler

## Client

- Python 3.x

---

# Build the Server

## Linux / macOS

```bash
make
```

## Windows (MinGW)

```bash
g++ -std=c++17 -pthread server.cpp -o chat_server -lws2_32
```

---

# Run the Server

## Linux / macOS

```bash
./chat_server
```

## Windows

```bash
chat_server.exe
```

---

# Run the CLI Client

```bash
python3 client.py
```

Windows:

```bash
python client.py
```

---

# Run the GUI Client

```bash
python3 gui_client.py
```

Windows:

```bash
python gui_client.py
```

---

# Communication Protocol

The application uses a simple JSON-based protocol.

Example:

```json
{
  "type": "msg",
  "user": "Alice",
  "content": "Hello everyone!"
}
```

---

# Server Architecture

- TCP socket listener
- Thread-per-client model
- Shared client pool
- Mutex-protected broadcasting
- Stream buffering for TCP packet fragmentation handling

---

# Screenshots

## GUI Client

>Add screenshots here for your GitHub portfolio.

Example:

```text
screenshots/chat_gui.png
```

---

# Future Improvements

- User authentication
- Private messaging
- Chat rooms
- End-to-end encryption
- File sharing
- Voice chat
- WebSocket support
- Docker deployment
- Database-backed message storage

---

# Learning Outcomes

This project helped demonstrate practical understanding of:

- Distributed systems fundamentals
- Concurrent programming
- Socket-level networking
- Cross-platform development
- Real-time application architecture
- GUI application integration

---

# License

MIT License

---

# Author

Your Name Here

GitHub: https://github.com/yourusername
