#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>

using namespace std;

const char* response[] = {
    "HTTP/1.0 200 OK\r\n",
    "Content-type: text/plain\r\n",
    "\r\n",
    "You look nice today!\r\n",
    NULL
};

void handle(int s) {
    cout << "Handling a request from fd #" << s << endl;
    char buf[4096];
    read(s, &buf, 4096);
    for(int i = 0; response[i] != NULL; i++) {
        send(s, response[i], strlen(response[i]), 0);
    }
    close(s);
}

int main() {
    int s, r, err;
    struct sockaddr_in me;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s == -1) perror("socket");

    memset(&me, 0, sizeof(me));
    me.sin_family = AF_INET;
    me.sin_port = htons(atoi(getenv("PORT")));
    err = bind(s, (struct sockaddr *)&me, sizeof(me));
    if(err != 0) perror("bind");

    err = listen(s, 10);
    if(err != 0) perror("listen");

    cout << "I'm listening." << endl;

    while(r = accept(s, NULL, NULL)) {
        thread(handle, move(r)).detach();
        cout << r << endl;
    }
}
