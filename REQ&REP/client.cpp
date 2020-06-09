#include "zmq/zmq.h"
#include <iostream>

int main(void)
{
  void *context = zmq_ctx_new();

  //  连接至服务端的套接字
  std::cout << "connect to server..." << std::endl;
  void *client = zmq_socket(context, ZMQ_REQ);
  zmq_connect(client, "tcp://localhost:5555");

  while(true)
  {
    zmq_msg_t send_msg;
    std::string hello = "hello";
    auto len = hello.length()+1; // 包含结束'\0'
    zmq_msg_init_size(&send_msg, len);
    memcpy(zmq_msg_data(&send_msg), hello.c_str(), len);
    std::cout << "client send: " << (char*)zmq_msg_data(&send_msg) << std::endl;
    zmq_msg_send(&send_msg, client, 0);
    zmq_msg_close(&send_msg);

    zmq_sleep(1);

    zmq_msg_t reply;
    zmq_msg_init(&reply);
    zmq_msg_recv(&reply, client, 0);
    std::cout << "client recv: " << (char*)zmq_msg_data(&reply) << std::endl;
    zmq_msg_close(&reply);
  }
  zmq_close(client);
  zmq_term(context);
  return 0;
}
