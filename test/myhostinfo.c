#include"csapp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

int main(int argc, char** argv){
    if(argc!=2){
        unix_error("format ./myhostinfo www.website.com");
    }

    //argv[1]->hostname
    //socket address
    //getaddrinfo
    struct addrinfo hints;
    struct addrinfo *result,*cur;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;

    getaddrinfo(argv[1],NULL,&hints,&result);
    char hostName[MAXLINE];
    int flags=NI_NUMERICHOST;
    for(cur=result;cur!=NULL;cur=cur->ai_next){
        getnameinfo(cur->ai_addr,cur->ai_addrlen,hostName,MAXLINE,NULL,0,flags);
        printf("canonname: %s, hostName: %s, addrlen: %d \n",cur->ai_canonname,hostName,cur->ai_addrlen);
    }
    freeaddrinfo(result);
    exit(EXIT_SUCCESS);
}