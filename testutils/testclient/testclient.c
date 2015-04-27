#include <WinSock2.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

char * HOST = "127.0.0.1";
unsigned short PORT = 8080;
static const char * RESOURCELIST[] = {
    "/hello.txt",
    "/imagetest/00.png",
    "/resource_script_demo.lua/r1.txt",
    "/"
};
static const char * METHODLIST[] = {
    "GET",
    "POST",
    "PUT"
};


static int CLIENTCOUNT = 0; // 20;
static int TESTCYCLES = 1;
static int RESOURCEINDEX = 2;
static int METHODINDEX = 2;


int sockvprintf(SOCKET soc, const char * fmt, va_list vl) {

    char buf[1024*8];
    int len = vsprintf_s(buf, sizeof(buf), fmt, vl);
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
static CRITICAL_SECTION cs = {0};
static size_t expectedData = 0;
static DWORD_PTR availableCPUs = 1;
static DWORD_PTR totalCPUs = 1;

static unsigned good = 0;
static unsigned bad = 0;
unsigned long postSize = 0;
unsigned long extraHeadSize = 0;
unsigned long queryStringSize = 0;
unsigned long keep_alive = 0;
int chunked = 1;


int WINAPI ClientMain(void * clientNo) {

    SOCKET soc;
    time_t lastData;
    size_t totalData = 0;
    size_t bodyData = 0;
    int isBody = 0;
    int isTest = (clientNo == 0);
    int cpu = ((int)clientNo) % 100;
    int timeOut = 10;
    const char * resource = 0;
    const char * method = 0;
    unsigned long i, j;

    // Method: PUT or GET
    if (METHODINDEX < sizeof(METHODLIST)/sizeof(METHODLIST[0])) {
        method = METHODLIST[METHODINDEX];
    }
    if (method == 0) {
        EnterCriticalSection(&cs);
        printf("\r\nClient %u: bad method\a\r\n", (int)clientNo);
        LeaveCriticalSection(&cs);
        return 1;
    }

    // Resource
    if (RESOURCEINDEX < sizeof(RESOURCELIST)/sizeof(RESOURCELIST[0])) {
        resource = RESOURCELIST[RESOURCEINDEX];
    }
    if (resource == 0) {
        EnterCriticalSection(&cs);
        printf("\r\nClient %u: bad resource\a\r\n", (int)clientNo);
        LeaveCriticalSection(&cs);
        return 2;
    }

    // CPU
    if ((!isTest) && (((1ULL<<cpu) & availableCPUs)!=0)) {
        SetThreadAffinityMask(GetCurrentThread(), 1ULL<<cpu);
    }

    // TCP
    soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (soc==INVALID_SOCKET) {
        EnterCriticalSection(&cs);
        printf("\r\nClient %u: cannot create socket\a\r\n", (int)clientNo);
        LeaveCriticalSection(&cs);
        return 3;
    }

    // comment in to disable Nagle:
    {int disable_Nagle = 1; setsockopt(soc, IPPROTO_TCP, TCP_NODELAY, (char *) &disable_Nagle, sizeof(disable_Nagle));}

    if (connect(soc, (SOCKADDR*)&target, sizeof(target))) {
        EnterCriticalSection(&cs);
        printf("\r\nClient %u: cannot connect to server %s:%u\a\r\n", (int)clientNo, HOST, PORT);
        LeaveCriticalSection(&cs);
        return 4;
    }

    for (j=0; j<((keep_alive>0)?keep_alive:1); j++) {

        // HTTP request
        if (queryStringSize>0) {
            sockprintf(soc, "%s %s?", method, resource);

            for (i=0;i<(queryStringSize/10);i++) {sockprintf(soc, "1234567890");}
            for (i=0;i<(queryStringSize%10);i++) {sockprintf(soc, "_");}

            sockprintf(soc, " HTTP/1.1\r\nHost: %s\r\n", HOST);
        } else {
            sockprintf(soc, "%s %s HTTP/1.1\r\nHost: %s\r\n", method, resource, HOST);
        }
        if (keep_alive) {
            sockprintf(soc, "Connection: Keep-Alive\r\n");
        } else {
            sockprintf(soc, "Connection: Close\r\n");
        }

        for (i=0;i<(extraHeadSize/25);i++) {sockprintf(soc, "Comment%04u: 1234567890\r\n", i % 10000);} /* omit (extraHeadSize%25) */

        if (!strcmp(method,"GET")) {
            sockprintf(soc, "\r\n");
        } else if (chunked) {

            sockprintf(soc, "Transfer-Encoding: chunked\r\n\r\n", postSize);

            for (i=0;i<(postSize/10);i++) {sockprintf(soc, "A\r\n1234567890\r\n");}
            if ((postSize%10)>0) {
                sockprintf(soc, "%x\r\n", postSize%10);
                for (i=0;i<(postSize%10);i++) {sockprintf(soc, "_");}
                sockprintf(soc, "\r\n");
            }

        } else {
            // not GET
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


        EnterCriticalSection(&cs);
        if (isTest) {
            expectedData = totalData;
        } else if (totalData != expectedData) {
            printf("Error: Client %u got %u bytes instead of %u\r\n", (int)clientNo, totalData, expectedData);
            bad++;
        } else {
            good++;
        }
        LeaveCriticalSection(&cs);

        if (keep_alive) {
            Sleep(10);
        }
    }

    shutdown(soc, SD_BOTH);
    closesocket(soc);

    return 0;
}


void RunMultiClientTest(int loop) {

    HANDLE *hThread = calloc(CLIENTCOUNT, sizeof(hThread[0]));
    int i;
    DWORD res;

    for (i=0;i<CLIENTCOUNT;i++) {
        DWORD dummy;
        hThread[i] = CreateThread(NULL, 1024*32, (LPTHREAD_START_ROUTINE)ClientMain, (void*)(1000*loop+i), 0, &dummy);
    }

    WaitForMultipleObjects(CLIENTCOUNT, hThread, TRUE, 15000);
    for (i=0;i<CLIENTCOUNT;i++) {
        res = WaitForSingleObject(hThread[i], 0);
        if (res == WAIT_OBJECT_0) {
            CloseHandle(hThread[i]);
            hThread[i]=0;
        }
    }
    for (i=0;i<CLIENTCOUNT;i++) {
        if (hThread[i]) {
            EnterCriticalSection(&cs);
            SuspendThread(hThread[i]); // -> check this thread in the debugger
            printf("Thread %i did not finish!\r\n", (int)(1000*loop+i));
            LeaveCriticalSection(&cs);
        }
    }
    EnterCriticalSection(&cs);
    printf("Test cylce %u completed\r\n\r\n", loop);
    LeaveCriticalSection(&cs);

    free(hThread);
}


int MultiClientTestAutomatic(unsigned long initialPostSize) {

    FILE        * log;
    int           cycle;

    postSize = initialPostSize;

    do {
        printf("Preparing test with %u bytes of data ...", postSize);
        ClientMain(0);
        if (expectedData==0) {
            printf(" Error: Could not read any data\a\r\n");
            return 1;
        }
        printf(" OK: %u bytes of data\r\n", expectedData);
        printf("Starting multi client test: %i cycles, %i clients each\r\n\r\n", (int)TESTCYCLES, (int)CLIENTCOUNT);
        good=bad=0;

        for (cycle=1;cycle<=TESTCYCLES;cycle++) {
            RunMultiClientTest(cycle);
        }

        printf("\r\n--------\r\n%u errors\r\n%u OK\r\n--------\r\n\r\n", bad, good);
        log = fopen("testclient.log", "at");
        if (log) {
            fprintf(log, "%u\t%u\t%u\r\n", postSize, good, bad);
            fclose(log);
        }

        postSize = (postSize!=0) ? (postSize<<1) : 1;

    } while (postSize!=0);

    return 0;
}


int SingleClientTestAutomatic(unsigned long initialPostSize) {

    FILE        * log;
    int           cycle;
    int           i;

    postSize = initialPostSize;
    for (cycle=1;cycle<=TESTCYCLES;cycle++) {
        good=bad=0;
        for (i=0;i<1 /* 000 */;i++) {
            expectedData=17;
            ClientMain((void*)1);
        }
        log = fopen("testclient.log", "at");
        if (log) {
            fprintf(log, "Cylce<%u>\t%u\t%u\r\n", cycle, good, bad);
            fclose(log);
        }
        printf("test cycle %u: %u good, %u bad\r\n", cycle, good, bad);
    }

    return 0;
}


int main(int argc, char * argv[]) {

    WSADATA       wsaData = {0};
    HOSTENT     * lpHost = 0;

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR) {
        printf("\r\nCannot init WinSock\a\r\n");
        return 1;
    }

    lpHost = gethostbyname(HOST);
    if (lpHost == NULL) {
        printf("\r\nCannot find host %s\a\r\n",HOST);
        return 2;
    }

    target.sin_family = AF_INET;
    target.sin_addr.s_addr = *((u_long FAR *) (lpHost->h_addr));
    target.sin_port = htons(PORT);

    GetProcessAffinityMask(GetCurrentProcess(), &availableCPUs, &totalCPUs);
    printf("CPUs (bit masks): process=%x, system=%x\r\n", availableCPUs, totalCPUs);

    InitializeCriticalSectionAndSpinCount(&cs, 100);

    /* Do the actual test here */
    if (CLIENTCOUNT>0) {
        MultiClientTestAutomatic(2000);
    } else {
        SingleClientTestAutomatic(2000);
    }

    /* Cleanup */
    DeleteCriticalSection(&cs);
    WSACleanup();
    return 0;
}
