# A-chat-bot
This is a chatbot made in Py and Cpp.
# Distributed CLI Chat Application

A distributed multi-threaded chat application using:

- C++17 TCP Server
- Python 3 Client
- JSON-based messaging protocol
- Multi-client support
- Cross-platform socket handling

---

## Features

### Server (C++17)

- Multi-threaded TCP server
- Handles multiple clients concurrently
- Thread-safe broadcasting
- Chat logging to `chat_log.txt`
- Cross-platform support (Windows/Linux/Mac)

### Client (Python)

- CLI-based chat interface
- Simultaneous sending and receiving
- JSON communication protocol
- Graceful disconnect handling

---

# Project Structure

```text
distributed-chat-app/
├── server.cpp
├── client.py
├── Makefile
├── README.md
├── .gitignore
└── chat_log.txt
```

---

# Requirements

## Server

- g++ with C++17 support

## Client

- Python 3.x

---

# Compile Server

Linux / macOS:

```bash
make
```

Windows (MinGW):

```bash
g++ -std=c++17 -pthread server.cpp -o chat_server -lws2_32
```

---

# Run Server

```bash
./chat_server
```

Windows:

```bash
chat_server.exe
```

---

# Run Client

```bash
python3 client.py
```

Windows:

```bash
python client.py
```

---

# JSON Protocol

Example message:

```json
{
  "type": "msg",
  "user": "Alice",
  "content": "Hello world"
}
```

---

# Future Improvements

- Private messaging
- GUI frontend
- Authentication
- Encryption (TLS)
- Docker deployment

---

# License

MIT License
