#include "csapp.h"

void echo(int connfd){
    rio_t rio;
    rio_readinitb(&rio,connfd);
    int n;
    char buf[MAXLINE];
    while((n=Rio_readlineb(&rio,buf,MAXLINE))>0){
        printf("receive from client %s\n",buf);
        rio_writen(rio.rio_fd,buf,n);
    }

}

int main(int argc,char** argv){
    if(argc!=2){
        unix_error("format ./server <port>");
    }

    int listenfd=Open_listenfd(atoi(argv[1]));
    

    while(1){
        struct sockaddr clientAddr;
        socklen_t addrlen=sizeof(clientAddr);
        char host[MAXLINE];
        memset(&clientAddr,0,addrlen);

        int connfd=accept(listenfd,&clientAddr,&addrlen);
        getnameinfo(&clientAddr,addrlen,host,MAXLINE,NULL,0,0);
        printf("receive from client %s\n",host);

        echo(connfd);
        close(connfd);
    }
    exit(EXIT_FAILURE);
}