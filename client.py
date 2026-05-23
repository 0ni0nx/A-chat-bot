#!/usr/bin/env python3
"""
client.py

A multithreaded TCP chat client.
Handles asynchronous receiving and sending of JSON-encoded payloads.
"""

import socket
import threading
import json
import sys

HOST = '127.0.0.1'
PORT = 8080

def receive_messages(sock):
    """
    Background thread to listen for incoming messages.
    Handles TCP stream fragmentation by buffering and splitting by newline.
    """
    buffer = ""
    while True:
        try:
            data = sock.recv(1024).decode('utf-8')
            if not data:
                print("\n[System] Disconnected from server.")
                break
            
            buffer += data
            while '\n' in buffer:
                message_str, buffer = buffer.split('\n', 1)
                if message_str.strip():
                    try:
                        msg = json.loads(message_str)
                        # Clear current terminal line to prevent overwriting user input
                        sys.stdout.write('\r\033[K')
                        
                        if msg.get("type") == "msg":
                            print(f"\033[36m[{msg['user']}]\033[0m: {msg['content']}")
                        elif msg.get("type") == "system":
                            print(f"\033[33m[System]\033[0m: {msg['content']}")
                            
                        # Reprint the input prompt
                        sys.stdout.write("You: ")
                        sys.stdout.flush()
                    except json.JSONDecodeError:
                        pass # Ignore malformed JSON
        except Exception as e:
            print(f"\n[System] Error receiving data: {e}")
            break

def main():
    print("=== Distributed CLI Chat ===")
    user_name = input("Enter your username: ").strip()
    if not user_name:
        user_name = "Anonymous"

    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((HOST, PORT))
    except Exception as e:
        print(f"Failed to connect to {HOST}:{PORT} - {e}")
        return

    # Send an initial join message
    join_msg = json.dumps({"type": "system", "user": user_name, "content": f"{user_name} joined the chat!"})
    client_socket.sendall((join_msg + "\n").encode('utf-8'))

    # Start listening thread as daemon so it exits when main program exits
    recv_thread = threading.Thread(target=receive_messages, args=(client_socket,), daemon=True)
    recv_thread.start()

    print("\nConnected! Type your messages below. (Type 'quit' to exit)\n")
    
    try:
        while True:
            sys.stdout.write("You: ")
            sys.stdout.flush()
            content = input()
            
            if content.lower() == 'quit':
                break
            
            if content.strip():
                # Construct JSON protocol payload
                payload = json.dumps({
                    "type": "msg",
                    "user": user_name,
                    "content": content
                })
                # Append newline to signify end of message for the TCP stream
                client_socket.sendall((payload + "\n").encode('utf-8'))
                
    except KeyboardInterrupt:
        pass
    finally:
        print("\nDisconnecting...")
        leave_msg = json.dumps({"type": "system", "user": user_name, "content": f"{user_name} left the chat."})
        client_socket.sendall((leave_msg + "\n").encode('utf-8'))
        client_socket.close()

if __name__ == "__main__":
    main()
