#include "zmq/zmq.h"
#include <iostream>

int main(void)
{
  void *context = zmq_ctx_new();

  //  与客户端通信的套接字
  void *server = zmq_socket(context, ZMQ_REP);
  zmq_bind(server, "tcp://*:5555");

  while (1) {
    //  等待客户端请求
    zmq_msg_t recv_msg;
    zmq_msg_init(&recv_msg);
    // 阻塞模式，非阻塞使用ZMQ_DONTWAIT，处理errno==EAGAIN，这里为了简单演示，没有使用非阻塞模式
    int rc = zmq_msg_recv(&recv_msg, server, 0);
    std::cout << "server recv: " << (char*)zmq_msg_data(&recv_msg) << std::endl;
    zmq_msg_close(&recv_msg);

    //  做些“处理”
    zmq_sleep(1);

    //  返回应答
    zmq_msg_t reply;
    std::string world = "World";
    auto len = world.length()+1;  // 包含结束'\0'
    zmq_msg_init_size(&reply, len);
    memcpy(zmq_msg_data(&reply), world.c_str(), len);
    std::cout << "server send: " << (char*)zmq_msg_data(&reply) << std::endl;
    zmq_msg_send(&reply, server, 0);
    zmq_msg_close(&reply);
  }
  //  程序不会运行到这里，以下只是演示我们应该如何结束
  zmq_close(server);
  zmq_term(context);
  return 0;
}
