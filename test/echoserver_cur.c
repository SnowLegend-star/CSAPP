#include"csapp.h"

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

void* thread(void* argvp){
    int connfd=*((int*) argvp);
    Free(argvp);
    echo(connfd);
    Close(connfd);
}

int main(int argc, char** argv){
    if(argc!=2){
        unix_error("usage: ./server <port>");
    }

    char* port=argv[1];
    int listenfd=Open_listenfd(port);
    int* connfd;
    struct sockaddr_storage clientaddr;
    socklen_t addrlen;
    pthread_t tid;

    while(1){
        addrlen=sizeof(struct sockaddr_storage);
        connfd=Malloc(sizeof(int));
        *connfd=Accept(listenfd,(SA*)&clientaddr,&addrlen);
        Pthread_create(&tid,NULL,thread,connfd);
    }

}