#include "csapp.h"

int main(int argc,char** argv){
    if(argc!=3){
        unix_error("format ./client <hostname> <port>");
    }

    char* hostName=argv[1];
    char* port=argv[2];
    
    int clientfd=Open_clientfd(hostName,atoi(port));
    char buf[MAXLINE];
    char* rptr=NULL;
    int n;
    rio_t rio;
    Rio_readinitb(&rio,clientfd);

    while((rptr=fgets(buf,MAXLINE,stdin))!=NULL){
        Rio_writen(rio.rio_fd, buf,strlen(buf));
        Rio_readlineb(&rio,buf,MAXLINE);
        printf("%s",buf);
    }
    exit(EXIT_SUCCESS);
}