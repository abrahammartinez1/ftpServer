#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Vincular con la bibliotecaS  de Winsock

#define PORT 20000
#define BUFFER_SIZE 1024

void handleUpload(SOCKET client_fd, const char* filename);
void handleCheck(SOCKET client_fd, const char* filename);
void handleRemove(SOCKET client_fd, const char* filename);
void handleDownload(SOCKET client_fd, const char* filename);

int main() {
    WSADATA wsaData;
    SOCKET server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        exit(EXIT_FAILURE);
    }

    // Crear socket del servidor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("socket failed");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Configurar opciones del socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        closesocket(server_fd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Vincular el socket al puerto
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        closesocket(server_fd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        closesocket(server_fd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en el puerto %d\n", PORT);

    while (1) {
        printf("Esperando conexiones...\n");

        if ((client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen)) == INVALID_SOCKET) {
            perror("accept");
            continue;
        }

        recv(client_fd, buffer, BUFFER_SIZE, 0);
        printf("Comando recibido: %s\n", buffer);

        // Aquí deberás analizar el buffer y determinar si es U, C, R, D
        if (strncmp(buffer, "U ", 2) == 0) {
            handleUpload(client_fd, buffer + 2);
        } else if (strncmp(buffer, "C ", 2) == 0) {
            handleCheck(client_fd, buffer + 2);
        } else if (strncmp(buffer, "R ", 2) == 0) {
            handleRemove(client_fd, buffer + 2);
        } else if (strncmp(buffer, "D ", 2) == 0) {
            handleDownload(client_fd, buffer + 2);
        } else {
            printf("Comando no reconocido.\n");
        }

        closesocket(client_fd);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}

void handleUpload(SOCKET client_fd, const char* filename) {
    // Lógica para manejar la subida de archivos
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    FILE *file;

    // Crear o abrir el archivo para escribir
    file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        // Enviar un mensaje de error al cliente
        send(client_fd, "Error al abrir el archivo\n", 27, 0);
        return;
    }

    // Recibir datos del archivo y escribirlos en el archivo
    while ((bytesReceived = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytesReceived, file);
    }

    if (bytesReceived < 0) {
        perror("Error al recibir datos");
    }

    // Cerrar el archivo
    fclose(file);

    // Enviar confirmación al cliente
    send(client_fd, "Archivo subido con éxito\n", 26, 0);
}

void handleCheck(SOCKET client_fd, const char* filename) {
    // Lógica para manejar la verificación de archivos
}

void handleRemove(SOCKET client_fd, const char* filename) {
    // Lógica para manejar la eliminación de archivos
}

void handleDownload(SOCKET client_fd, const char* filename) {
    // Lógica para manejar la descarga de archivos
}
