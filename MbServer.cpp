#include "MbServer.h"
#include <stdio.h>
//#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#if defined(_WIN32)
#include <ws2tcpip.h>
#else
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#define NB_CONNECTION    5

MbServer::MbServer():
    QThread(NULL),
    ctx(NULL),
    server_socket(-1)
{
    m_port = config->m_server_port;
    for(int i = 0; i < NB_SLAVE; i++)
    {
        mb_mapping[i] = NULL;
    }
}

MbServer::~MbServer()
{

}
int MbServer::Init(int port)
{
    if(ctx != NULL)
    {
        modbus_free(ctx);
        ctx = NULL;
    }
     ctx = modbus_new_tcp("127.0.0.1", port);

     for(int i = 0; i < NB_SLAVE; i++)
     {
         mb_mapping[i] = modbus_mapping_new(MODBUS_MAX_READ_BITS, 0,
                                         MODBUS_MAX_READ_REGISTERS, 0);
         if (mb_mapping[i] == NULL) {
             fprintf(stderr, "Failed to allocate the mapping: %s\n",
                     modbus_strerror(errno));
             modbus_free(ctx);
             return -1;
         }
     }


     server_socket = modbus_tcp_listen(ctx, NB_CONNECTION);
     if (server_socket == -1) {
         fprintf(stderr, "Unable to listen TCP connection\n");
         modbus_free(ctx);
         return -2;
     }

     //signal(SIGINT, close_sigint);

     /* Clear the reference set of socket */
     FD_ZERO(&refset);
     /* Add the server socket */
     FD_SET(server_socket, &refset);

     /* Keep track of the max file descriptor */
     fdmax = server_socket;

     return 0;

}
void MbServer::run()
{

    while ( !QThread::currentThread()->isInterruptionRequested() ) {
        if(doWork()) // 判断是否工作已经做完.做完就退出线程.
        {
            return;
        }
    }


}
#include <stdio.h>
void MbServer::close_sigint(int dummy)
{
    if (server_socket != -1) {
        closesocket(server_socket);
    }
    modbus_free(ctx);
    for(int i = 0; i < NB_SLAVE; i++)
    {
        modbus_mapping_free(mb_mapping[i]);
        mb_mapping[i] = NULL;
    }

    exit(dummy);
}
bool MbServer::doWork()
{
    rdset = refset;
    if (select(fdmax+1, &rdset, NULL, NULL, NULL) == -1) {
        perror("Server select() failure.");
        close_sigint(1);
    }

    /* Run through the existing connections looking for data to be
     * read */
    for (master_socket = 0; master_socket <= fdmax; master_socket++) {

        if (!FD_ISSET(master_socket, &rdset)) {
            continue;
        }

        if (master_socket == server_socket) {
            /* A client is asking a new connection */
            socklen_t addrlen;
            struct sockaddr_in clientaddr;
            int newfd;

            /* Handle new connections */
            addrlen = sizeof(clientaddr);
            memset(&clientaddr, 0, sizeof(clientaddr));
            newfd = accept(server_socket, (struct sockaddr *)&clientaddr, &addrlen);
            if (newfd == -1) {
                perror("Server accept() error");
                return true;
            } else {
                FD_SET(newfd, &refset);

                if (newfd > fdmax) {
                    /* Keep track of the maximum */
                    fdmax = newfd;
                }
                printf("New connection from %s:%d on socket %d\n",
                       inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port, newfd);
            }
        } else {
            modbus_set_socket(ctx, master_socket);
            rc = modbus_receive(ctx, query);
            if (rc > 0) {
                int offset = modbus_get_header_length(ctx);
                int slave = query[offset - 1] - 1;
                if(slave >=0 && slave < NB_SLAVE)
                {
                   modbus_reply(ctx, query, rc, mb_mapping[slave]);
                }

            } else if (rc == -1) {
                /* This example server in ended on connection closing or
                 * any errors. */
                printf("Connection closed on socket %d\n", master_socket);
                closesocket(master_socket);

                /* Remove from reference set */
                FD_CLR(master_socket, &refset);

                if (master_socket == fdmax) {
                    fdmax--;
                }
            }
        }
    }
    return false;
}
bool MbServer::StartServer(int port)
{
    if(Init(port) != 0){
        return false;
    }
    this->start();
    return true;
}

bool MbServer::StopServer()
{
    this->requestInterruption();
    return this->wait(3000);
}
