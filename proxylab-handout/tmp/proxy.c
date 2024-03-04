#include <stdio.h>
#include"csapp.h"
#include"sbuf.h"
#include"cache.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define HTTP_PREFIX "http://"
#define NTHREADS 4
#define SBUFSIZE 16

sbuf_t sbuf;    //shared buffer of connected descriptor
void *thread(void *vargp);
cache_t cache;
int readcnt;
sem_t mutex, W;

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

void handleRequest(int fd);
void parseRequest(char* buf, char* host, char* port, char* method, char* url, char* version, char* fileName);
void clientError(int fd, char* cause, char* errnum, char* errmsg, char* errmsg_datail);
int readAndFormatRequestHeader(rio_t* rio, char* clientRequest, char* Host, char* port,
                                char* method, char* url, char* version, char* filename);
void readAndWriteResponse(int fd, rio_t* rio_Proxy2Server, char* url);
//和tiny.c里面的那个差不多
void clientError(int fd, char* cause, char* errnum, char* errmsg, char* errmsg_datail){
    char buf[MAXLINE];

    //打印HTTP的响应头
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, errmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    //打印HTTP响应的主体
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd,buf,strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd,buf,strlen(buf));
    sprintf(buf, "%s: %s\r\n", errmsg, errmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", errmsg_datail, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web Server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}
cache_Item* readCache(char* url_target, int clientfd);


void handleRequest(int fd){
    //处理各种定位的指针
    char* pos=NULL;

    //分割http请求的参数
    char buf[MAXLINE],method[MAXLINE],url[MAXLINE],version[MAXLINE],fileName[MAXLINE];

    //客户端的几个请求头的主体
    char clientRequest[MAXLINE];
    char hostName[MAXLINE],port[MAXLINE];

    //proxy-client和proxy-server的IO
    rio_t rio_Proxy2Client,rio_Proxy2Server;

    //Step1: proxy读入来自client的请求
    Rio_readinitb(&rio_Proxy2Client,fd);
    if(Rio_readlineb(&rio_Proxy2Client,buf,MAXLINE)==0){
        //这个请求是空的
        printf("Oops! empty request\n");
        return ;
    }

    //如果http的版本是1.1，处理成1.0
    if((pos=strstr(buf,"HTTP/1.1"))!=NULL){
        buf[pos-buf-1+strlen("HTTP/1.1")]='0';
    }

    //Step2: 分割请求
    parseRequest(buf, hostName, port, method, url, version, fileName);

    //判断请求是否有效
    if(strcasecmp(method,"GET")!=0){
        clientError(fd, method, "501", "Not Implement", "Tiny Does not implement this method\n");
        return ;
    }

    int rv=readAndFormatRequestHeader(&rio_Proxy2Client, clientRequest, hostName, port, method, url, version, fileName);
    if(rv==0){
        return ;
    }

    //尝试读取cache
    cache_Item* item=readCache(url, fd);
    if(item!=NULL){
        Rio_writen(fd,item->item_Content, item->item_Size);
        return ;
    }

    //Step3: tiny server和proxy建立连接
    int clientfd=Open_clientfd(hostName, port);

    Rio_readinitb(&rio_Proxy2Server, clientfd);
    Rio_writen(rio_Proxy2Server.rio_fd, clientRequest, strlen(clientRequest));

    //Step4: 从tiny server读入response，并且把它发送给client
    printf("The Proxy is ready to relay the response\n");

    // char tinyResponse[MAXLINE];
    // int n;

    // while((n=Rio_readlineb(&rio_Proxy2Server, tinyResponse, MAXLINE))!=0){
    //     Rio_writen(fd, tinyResponse, n);
    // }
    readAndWriteResponse(fd, &rio_Proxy2Server, url);
}



void parseRequest(char* buf, char* host, char* port, char* method, char* url, char* version, char* fileName){
    //client request is like this
    //GET http://www.cmu.edu/hub/index.html HTTP/1.1
    sscanf(buf,"%s %s %s", method, url, version);
    //method = "GET", url = "http://localhost:15213/home.html", version = "HTTP1.0"

    char* host_pos =strstr(url,HTTP_PREFIX)+strlen(HTTP_PREFIX);    //主机名开始的位置
    char* port_pos =strstr(host_pos,":");                           //端口开始的位置
    char* slash_pos=strstr(host_pos,"/");                           //suffix开始的位置
   
    //判断url有没有带端口号，如果没带就是默认端口80
    if(port_pos==NULL){ //没带端口号
        strcpy(port,"80");
        strncmp(host,host_pos,slash_pos-host_pos);      
    }
    else{
        strncpy(host,host_pos,port_pos-host_pos);
        strncpy(port,port_pos+1,slash_pos-port_pos-1);
    }

    strcpy(fileName,slash_pos);
    printf("HostName: %s",host);
    printf("Port: %s",port);
    printf("fileName: %s",fileName);
}

int readAndFormatRequestHeader(rio_t* rio, char* clientRequest, char* Host, char* port,
                                char* method, char* url, char* version, char* fileName){

    int UserAgent=0, Connection=0, proxyConnection=0, hostInfo=0;
    char buf[MAXLINE/2];
    int n;
    char* findpos;

    sprintf(clientRequest, "GET %s HTTP/1.0\r\n",fileName);

    n=Rio_readlineb(rio, buf, MAXLINE);
    printf("receive buf %s\n", buf);
    printf("n = %d", n);

    while(strcmp("\r\n",buf)!=0&&n!=0){
        strcat(clientRequest, buf);
        printf("receive buf %s\n", buf);
        
        //判断要求的四个请求头是否存在
        if((findpos=strstr(buf, "User-Agent:"))!=NULL){
            UserAgent=1;
        }
        if((findpos=strstr(buf,"Proxy-Connection:"))!=NULL){
            proxyConnection=1;
        }
        if((findpos=strstr(buf,"Connection"))!=NULL){
            Connection=1;
        }
        if((findpos=strstr(buf, "Host"))!=NULL){
            hostInfo=1;
        }

        n=Rio_readlineb(rio, buf ,MAXLINE);
    }

    if(n==0){
        return 0;
    }


    //如果缺失了这四个头部，则进行添加    
    if(hostInfo==0){
        sprintf(buf, "Host: %s\r\n", Host);
        strcat(clientRequest, buf);
    }

    if(UserAgent==0){
        strcat(clientRequest, user_agent_hdr);
    }

    if(Connection==0){
        sprintf(buf, "Connection: close\r\n");
        strcat(clientRequest, buf);
    }

    if(proxyConnection==0){
        sprintf(buf, "Proxy-Connection: close\r\n");
        strcat(clientRequest, buf);
    }

    //添加最后的空行
    strcat(clientRequest,"\r\n");
    return 1;
}

//第一类读者问题
cache_Item* readCache(char* url_target, int clientfd){
    P(&mutex);
    readcnt++;
    if(readcnt==1){
        P(&W);
    }
    V(&mutex);

    rio_t rio;
    Rio_readinitb(&rio, clientfd);
    for(int i=0;i<cache.cache_Item_Using;i++){
        if(strcmp(url_target,cache.cache_Set[i].url)==0){
            return &cache.cache_Set[i];
        }
    }

    P(&mutex);
    readcnt--;
    if(readcnt==0){
        V(&W);
    }
    V(&mutex);
    return NULL;
}

void readAndWriteResponse(int fd, rio_t* rio_Proxy2Server, char* url){
    char tinyResponse[MAXLINE];
    int bodySize_Cur=0, n;
    cache_Item* item=(cache_Item*)malloc(sizeof(cache_Item));

    
    while((n=Rio_readlineb(rio_Proxy2Server, tinyResponse, MAXLINE))!=0){
        Rio_writen(fd, tinyResponse, n);
        strcat(item->item_Content, tinyResponse);
        //如果respons的头部读完了，准备缓存item
        if(strcmp(tinyResponse,"\r\n")==0){
            break ;
        }
    }

    memset(tinyResponse, 0, MAXLINE);
    while((n=Rio_readlineb(rio_Proxy2Server, tinyResponse, MAXLINE))!=0){
        Rio_writen(fd, tinyResponse, n);
        bodySize_Cur+=n;
        strcat(item->item_Content, tinyResponse);
    }
    item->item_Size=bodySize_Cur;
    strncpy(item->url,url,sizeof(url));
    item->tiemTamp=time(NULL);
    if(bodySize_Cur>MAX_OBJECT_SIZE){   //这个response的body太大了，不可以缓存
        Free(item);
        return ; 
    }

    //把这个item写入cache
    P(&W);
    cache_insert(&cache, item);
    V(&W);
}


int main(int argc,char** argv){
    if(argc!=2){
        unix_error("proxy usage: ./proxy <port>");
    }

    int listenfd=Open_listenfd(argv[1]), i;
    pthread_t tid;
    struct sockaddr_storage clientaddr;
    char hostName[MAXLINE], port[MAXLINE];
    sbuf_init(&sbuf, SBUFSIZE);
    cache_init(&cache);

    for(i=0;i<NTHREADS;i++){    //创建工作线程
        Pthread_create(&tid, NULL, thread, NULL);
    }

    while(1){
        socklen_t clientlen=sizeof(struct sockaddr_storage);
        int connfd=Accept(listenfd,(SA*) &clientaddr,&clientlen);
        //Getnameinfo((SA*)&clientaddr, clientlen, hostName, MAXLINE, port, MAXLINE, 0);
        sbuf_insert(&sbuf, connfd);
    }
    return 0;
}

void *thread(void *vargp){
    pthread_detach(pthread_self());
    while(1){
        int connfd=sbuf_remove(&sbuf);
        handleRequest(connfd);
        Close(connfd);
    }
}