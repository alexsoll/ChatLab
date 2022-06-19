#include "server.hpp"

namespace chatlab {

cl_status Server::Start() {
    int opt = 1;
    this->serv_port = CL_DEFAULT_PORT;
    this->serv_addr.sin_addr.s_addr = inet_addr(CL_DEFAULT_SERV_ADDR);
    this->serv_addr.sin_port = htons(serv_port);
    this->serv_addr.sin_family = CL_SOCK_TYPE;

    if ((serv_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        ERROR_PRINT("can't create socket");
        return cl_status::ERROR;
    }

    if (setsockopt(serv_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        ERROR_PRINT("can't set socket options");
        return cl_status::ERROR;
    }

    if (bind(serv_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        ERROR_PRINT("can't bind socket");
        return cl_status::ERROR;
    }

    if(listen(serv_socket, SOMAXCONN) < 0) {
        ERROR_PRINT("can't listen");
        return cl_status::ERROR;
    }

    serv_status = ServerStatus::run;
    DEBUG_PRINT("server addres: %s, server_port: %d\n",
                inet_ntoa(this->serv_addr.sin_addr), htons(this->serv_addr.sin_port));
    DEBUG_PRINT("server is running\n");

    while (serv_status == ServerStatus::run) {
        ServerClient new_client;

        new_client.socket = accept(serv_socket, (struct sockaddr*)&new_client.addr, &new_client.addr_len);
        if (new_client.socket >= 0 && serv_status == ServerStatus::run) {
            DEBUG_PRINT("accepted new connection from %s\n", inet_ntoa(new_client.addr.sin_addr));
            new_client.status = ClientStatus::connected;
            clients.push_back(new_client);
        } else {
            WARNING_PRINT("invalid connection from %s\n", inet_ntoa(new_client.addr.sin_addr));
        }
    }

    return cl_status::SUCCESS;
}

cl_status Server::Stop() {
    // TODO: Bcast shutdown
    if(DisconnectAll() != cl_status::SUCCESS) {
        return cl_status::ERROR;
    };
    close(serv_socket);

    return cl_status::SUCCESS;
}

cl_status Server::DisconnectClient(ServerClient &client) {
    return cl_status::SUCCESS;
}

cl_status Server::DisconnectAll() {
    return cl_status::SUCCESS;
}

cl_status Server::SendTo(ServerClient &client) {
    return cl_status::SUCCESS;
}

cl_status Server::Bcast() {
    return cl_status::SUCCESS;
}

}

int main(int argc, char const* argv[])
{
    chatlab::Server server;

    server.Start();

    return 0;
}
