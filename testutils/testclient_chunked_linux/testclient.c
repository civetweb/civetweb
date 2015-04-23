/* quick and dirty test for chunked encoding */

#ifdef WIN32
#include <WinSock2.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
typedef int SOCKET;
typedef struct sockaddr SOCKADDR;
typedef struct hostent HOSTENT;
const int INVALID_SOCKET = -1;
const int SD_SEND = SHUT_WR;
const int SD_BOTH = SHUT_RDWR;
#define closesocket(x) close(x)
#define Sleep(x) usleep((x)*1000)
#define ioctlsocket(a,b,c) ioctl(a,b,c)
#endif
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

char * HOST = "127.0.0.1";
unsigned short PORT = 8080;
const char * RESOURCE = "/resource_script_demo.lua/r1.txt";
const char * METHOD = "PUT";

unsigned postSize = 9876;
unsigned extraHeadSize = 0;
unsigned queryStringSize = 0;
int keep_alive = 0;
int chunked = 1;


int sockvprintf(SOCKET soc, const char * fmt, va_list vl) {

    char buf[1024*8];
#ifdef WIN32
    int len = vsprintf_s(buf, sizeof(buf), fmt, vl);
#else
    int len = vsprintf(buf, fmt, vl);
#endif
    int ret = send(soc, buf, len, 0);
    return ret;
}


int sockprintf(SOCKET soc, const char * fmt, ...) {

    int ret = -1;
    va_list vl;
    va_start(vl, fmt);
    ret = sockvprintf(soc, fmt, vl);
    va_end(vl);
    return ret;
}


static struct sockaddr_in target = {0};


int TestClient(unsigned clientNo) {

    SOCKET soc;
    time_t lastData;
    size_t totalData = 0;
    size_t bodyData = 0;
    int isBody = 0;
    int timeOut = 10;
    unsigned long i;

    // TCP
    soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (soc==INVALID_SOCKET) {
        printf("\r\nClient %u: cannot create socket\a\r\n", (int)clientNo);
        return 3;
    }

    // comment in to disable Nagle:
    {int disable_Nagle = 1; setsockopt(soc, IPPROTO_TCP, TCP_NODELAY, (char *) &disable_Nagle, sizeof(disable_Nagle));}

    if (connect(soc, (SOCKADDR*)&target, sizeof(target))) {
        printf("\r\nClient %u: cannot connect to server %s:%u\a\r\n", (int)clientNo, HOST, PORT);
        return 4;
    }

    sockprintf(soc, "%s %s", METHOD, RESOURCE);

    if (queryStringSize>0) {
        sockprintf(soc, "?", METHOD, RESOURCE);
        for (i=0;i<(queryStringSize/10);i++) {sockprintf(soc, "1234567890");}
        for (i=0;i<(queryStringSize%10);i++) {sockprintf(soc, "_");}
    }

    sockprintf(soc, " HTTP/1.1\r\nHost: %s\r\n", HOST);

    if (keep_alive) {
        sockprintf(soc, "Connection: Keep-Alive\r\n");
    } else {
        sockprintf(soc, "Connection: Close\r\n");
    }

    for (i=0;i<(extraHeadSize/25);i++) {
        sockprintf(soc, "Comment%04u: 1234567890\r\n", i % 10000);
    }

    if (!strcmp(METHOD, "GET")) {
        sockprintf(soc, "\r\n");

    } else if (chunked) {

        unsigned remaining_postSize = postSize;
        sockprintf(soc, "Transfer-Encoding: chunked\r\n\r\n", postSize);

        while (remaining_postSize > 0) {
            unsigned chunk = rand()%200 + 1;
            if (chunk>remaining_postSize) chunk = remaining_postSize;

            sockprintf(soc, "%x\r\n", chunk);
            for (i=0;i<chunk;i++) {sockprintf(soc, "_");}
            sockprintf(soc, "\r\n");

            remaining_postSize -= chunk;
        }
        sockprintf(soc, "0\r\n\r\n", postSize%10);


    } else {

        sockprintf(soc, "Content-Length: %u\r\n\r\n", postSize);

        for (i=0;i<postSize/10;i++) {sockprintf(soc, "1234567890");}
        for (i=0;i<postSize%10;i++) {sockprintf(soc, ".");}

        timeOut += postSize/10000;
    }

    if (!keep_alive) {
        shutdown(soc, SD_SEND);
    } else {
        timeOut = 2;
    }

    // wait for response from the server
    bodyData = totalData = 0;
    isBody = 0;
    lastData = time(0);
    for (;;) {
        char buf[20480];
        int chunkSize = 0;
        unsigned long dataReady = 0;

        Sleep(1);

        if (ioctlsocket(soc, FIONREAD, &dataReady) < 0) break;
        if (dataReady) {
            chunkSize = recv(soc, buf+totalData, sizeof(buf)-totalData, 0);
            if (chunkSize<0) {
                printf("Error: recv failed for client %i\r\n", (int)clientNo);
                break;
            } else if (!isBody) {
                char * headEnd = strstr(buf,"\xD\xA\xD\xA");
                if (headEnd) {
                    headEnd+=4;
                    chunkSize -= ((int)headEnd - (int)buf);
                    if (chunkSize>0) {
                        //fwrite(headEnd,1,got,STORE);
                        bodyData += chunkSize;
                    }
                    isBody=1;
                }
            } else {
                //fwrite(buf,1,got,STORE);
                bodyData += chunkSize;
            }
            lastData = time(0);
            totalData += chunkSize;
        } else {
            time_t current = time(0);
            if (difftime(current, lastData) > timeOut) {
                break;
            }
            Sleep(10);
        }
    }

    shutdown(soc, SD_BOTH);
    closesocket(soc);

    return 0;
}



int main(int argc, char * argv[]) {

    HOSTENT     * lpHost = 0;

#ifdef WIN32
    WSADATA       wsaData = {0};

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR) {
        printf("\r\nCannot init WinSock\a\r\n");
        return 1;
    }
#endif

    srand((unsigned int)time(NULL));

    lpHost = gethostbyname(HOST);
    if (lpHost == NULL) {
        printf("\r\nCannot find host %s\a\r\n",HOST);
        return 2;
    }

    target.sin_family = AF_INET;
#ifdef WIN32
    target.sin_addr.s_addr = *((u_long FAR *) (lpHost->h_addr));
#else
    target.sin_addr.s_addr = htonl(0x7F000001);
#endif
    target.sin_port = htons(PORT);
    printf("\r\nConnect to %s\r\n", inet_ntoa(target.sin_addr));

    TestClient(1);

#ifdef WIN32
    WSACleanup();
#endif

    return 0;
}
