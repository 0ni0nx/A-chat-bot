/**
 * server.cpp
 * 
 * A multi-threaded TCP chat server using standard C++17.
 * Features: Zero external dependencies, Cross-platform (Windows/Linux/Mac),
 * Thread-safe client management, and TCP stream fragmentation handling.
 */

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cstring>

// Cross-platform socket abstraction
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
    #define CLOSE_SOCKET closesocket
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define CLOSE_SOCKET close
#endif

// Global State
std::vector<SOCKET> active_clients;
std::mutex clients_mutex;
std::mutex log_mutex;
const int PORT = 8080;
const std::string LOG_FILE = "chat_log.txt";

// Helper: Get current timestamp
std::string get_timestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::string ts = std::ctime(&now_time);
    ts.pop_back(); // Remove trailing newline
    return "[" + ts + "] ";
}

// Thread-safe logging to file
void log_message(const std::string& message) {
    std::lock_guard<std::mutex> lock(log_mutex);
    std::ofstream file(LOG_FILE, std::ios::app);
    if (file.is_open()) {
        file << get_timestamp() << message << std::endl;
    }
}

// Thread-safe broadcast to all connected clients
void broadcast(const std::string& message, SOCKET sender_socket) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (SOCKET client : active_clients) {
        if (client != sender_socket) {
            // Send message with newline delimiter for TCP boundary handling
            std::string payload = message + "\n";
            send(client, payload.c_str(), payload.length(), 0);
        }
    }
}

// Client handler thread
void handle_client(SOCKET client_socket) {
    char temp_buffer[1024];
    std::string stream_buffer;

    while (true) {
        int bytes_received = recv(client_socket, temp_buffer, sizeof(temp_buffer), 0);
        
        // Handle graceful disconnection or network error
        if (bytes_received <= 0) {
            break; 
        }

        // Handle TCP Stream Fragmentation: buffer incoming bytes and parse by newline delimiter
        stream_buffer.append(temp_buffer, bytes_received);
        size_t pos;
        while ((pos = stream_buffer.find('\n')) != std::string::npos) {
            std::string message = stream_buffer.substr(0, pos);
            stream_buffer.erase(0, pos + 1);

            if (!message.empty()) {
                log_message("Received: " + message);
                broadcast(message, client_socket);
            }
        }
    }

    // Cleanup on disconnect
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        active_clients.erase(std::remove(active_clients.begin(), active_clients.end(), client_socket), active_clients.end());
    }
    CLOSE_SOCKET(client_socket);
    std::cout << "Client disconnected. Active clients: " << active_clients.size() << std::endl;
}

int main() {
    // Initialize Winsock if on Windows
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }
#endif

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Allow port reuse
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed." << std::endl;
        CLOSE_SOCKET(server_socket);
        return 1;
    }

    if (listen(server_socket, 10) == SOCKET_ERROR) {
        std::cerr << "Listen failed." << std::endl;
        CLOSE_SOCKET(server_socket);
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "..." << std::endl;
    log_message("Server started.");

    while (true) {
        sockaddr_in client_addr;
        socklen_t client_size = sizeof(client_addr);
        SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_size);
        
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed." << std::endl;
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            active_clients.push_back(client_socket);
        }

        std::cout << "New client connected. Active clients: " << active_clients.size() << std::endl;

        // Spawn a detached thread for the new client
        std::thread(handle_client, client_socket).detach();
    }

    CLOSE_SOCKET(server_socket);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
