#include <cstring>
#include <errno.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <netinet/ip.h>

// using namespace std;

const char *response[] = {"HTTP/1.0 200 OK\r\n", "Content-type: text/plain\r\n",
                          "\r\n", "You look nice today!\r\n", NULL};

void handle(int s) {
  std::cout << "Handling a request from fd #" << s << std::endl;
  char buf[4096];
  read(s, &buf, 4096);
  for (int i = 0; response[i] != NULL; i++) {
    send(s, response[i], strlen(response[i]), 0);
  }
  close(s);
}

int main() {
  int s, r;
  struct sockaddr_in me;

  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1)
    perror("socket");

  memset(&me, 0, sizeof(me));
  me.sin_family = AF_INET;
  me.sin_port = htons(atoi(getenv("PORT")));
  int err = bind(s, (struct sockaddr *)&me, sizeof(me));
  if (err != 0)
    perror("bind");

  err = listen(s, 10);
  if (err != 0)
    perror("listen");

  std::cout << "I'm listening." << std::endl;

  while ((r = accept(s, NULL, NULL))) {
    std::thread(handle, std::move(r)).detach();
    std::cout << r << std::endl;
  }
}
