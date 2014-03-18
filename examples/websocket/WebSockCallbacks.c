#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "WebSockCallbacks.h"

#ifdef __APPLE__
#include <string.h>
#endif

#ifdef _WIN32
#include <Windows.h>
typedef HANDLE pthread_mutex_t;
static int pthread_mutex_init(pthread_mutex_t *mutex, void *unused) {
    unused = NULL;
    *mutex = CreateMutex(NULL, FALSE, NULL);
    return *mutex == NULL ? -1 : 0;
}

static int pthread_mutex_destroy(pthread_mutex_t *mutex) {
    return CloseHandle(*mutex) == 0 ? -1 : 0;
}

static int pthread_mutex_lock(pthread_mutex_t *mutex) {
    return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0? 0 : -1;
}

static int pthread_mutex_unlock(pthread_mutex_t *mutex) {
    return ReleaseMutex(*mutex) == 0 ? -1 : 0;
}
#define mg_sleep(x) Sleep(x)
#else
#include <unistd.h>
#include <pthread.h>
#define mg_sleep(x) usleep((x) * 1000)
#endif


typedef struct tWebSockInfo {
    int webSockState;
    unsigned long initId;
    struct mg_connection *conn;
} tWebSockInfo;

static pthread_mutex_t sMutex;

#define MAX_NUM_OF_WEBSOCKS (256)
static tWebSockInfo *socketList[MAX_NUM_OF_WEBSOCKS];


static void send_to_all_websockets(const char * data, int data_len) {

    int i;

    for (i=0;i<MAX_NUM_OF_WEBSOCKS;i++) {
        if (socketList[i] && (socketList[i]->webSockState==2)) {
            mg_websocket_write(socketList[i]->conn, WEBSOCKET_OPCODE_TEXT, data, data_len);
        }
    }
}


void websocket_ready_handler(struct mg_connection *conn) {

    int i;
    struct mg_request_info * rq = mg_get_request_info(conn);
    tWebSockInfo * wsock = malloc(sizeof(tWebSockInfo));
    assert(wsock);
    wsock->webSockState = 0;
    rq->conn_data = wsock;

    pthread_mutex_lock(&sMutex);
    for (i=0;i<MAX_NUM_OF_WEBSOCKS;i++) {
        if (0==socketList[i]) {
            socketList[i] = wsock;
            wsock->conn = conn;
            wsock->webSockState = 1;
            break;
        }
    }
    printf("\nNew websocket attached: %08lx:%u\n", rq->remote_ip, rq->remote_port);
    pthread_mutex_unlock(&sMutex);
}


static void websocket_done(tWebSockInfo * wsock) {
    int i;
    if (wsock) {
        wsock->webSockState = 99;
        for (i=0;i<MAX_NUM_OF_WEBSOCKS;i++) {
            if (wsock==socketList[i]) {
                socketList[i] = 0;
                break;
            }
        }
        printf("\nClose websocket attached: %08lx:%u\n", mg_get_request_info(wsock->conn)->remote_ip, mg_get_request_info(wsock->conn)->remote_port);
        free(wsock);
    }
}


int websocket_data_handler(struct mg_connection *conn, int flags, char *data, size_t data_len) {
    struct mg_request_info * rq = mg_get_request_info(conn);
    tWebSockInfo * wsock = (tWebSockInfo*)rq->conn_data;
    char msg[128];

    pthread_mutex_lock(&sMutex);
    if (flags==136) {
        // close websock
        websocket_done(wsock);
        rq->conn_data = 0;
        pthread_mutex_unlock(&sMutex);
        return 1;
    }
    if (((data_len>=5) && (data_len<100) && (flags==129)) || (flags==130)) {

        // init command
        if ((wsock->webSockState==1) && (!memcmp(data,"init ",5))) {
            char * chk;
            unsigned long gid;
            memcpy(msg,data+5,data_len-5);
            msg[data_len-5]=0;
            gid = strtoul(msg,&chk,10);
            wsock->initId = gid;
            if (gid>0 && chk!=NULL && *chk==0) {
                wsock->webSockState = 2;
            }
            pthread_mutex_unlock(&sMutex);
            return 1;
        }

        // chat message
        if ((wsock->webSockState==2) && (!memcmp(data,"msg ",4))) {
            send_to_all_websockets(data, data_len);
            pthread_mutex_unlock(&sMutex);
            return 1;
        }
    }

    // keep alive
    if ((data_len==4) && !memcmp(data,"ping",4)) {
        pthread_mutex_unlock(&sMutex);
        return 1;
    }

    pthread_mutex_unlock(&sMutex);
    return 0;
}


void connection_close_handler(struct mg_connection *conn) {
    struct mg_request_info * rq = mg_get_request_info(conn);
    tWebSockInfo * wsock = (tWebSockInfo*)rq->conn_data;

    pthread_mutex_lock(&sMutex);
    websocket_done(wsock);
    rq->conn_data = 0;
    pthread_mutex_unlock(&sMutex);
}


static int runLoop = 0;

static void * eventMain(void * _ignored) {
    int i;
    char msg[256];

    runLoop = 1;
    while (runLoop) {
        time_t t = time(0);
        struct tm * timestr = localtime(&t);
        sprintf(msg,"title %s",asctime(timestr));

        pthread_mutex_lock(&sMutex);
        for (i=0;i<MAX_NUM_OF_WEBSOCKS;i++) {
            if (socketList[i] && (socketList[i]->webSockState==2)) {
                mg_websocket_write(socketList[i]->conn, WEBSOCKET_OPCODE_TEXT, msg, strlen(msg));
            }
        }
        pthread_mutex_unlock(&sMutex);

        mg_sleep(1000);
    }

    return _ignored;
}

void websock_send_broadcast(const char * data, int data_len) {

    char buffer[260];

    if (data_len<=256) {
        strcpy(buffer, "msg ");
        memcpy(buffer+4, data, data_len);

        pthread_mutex_lock(&sMutex);
        send_to_all_websockets(buffer, data_len+4);
        pthread_mutex_unlock(&sMutex);
    }
}

void websock_init_lib(void) {

    int ret;
    ret = pthread_mutex_init(&sMutex, 0);
    assert(ret==0);

    memset(socketList,0,sizeof(socketList));

    mg_start_thread(eventMain, 0);
}

void websock_exit_lib(void) {

    runLoop = 0;
}
