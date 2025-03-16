#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "example.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using example::ExampleService;
using example::HelloRequest;
using example::HelloReply;

class ExampleClient {
public:
    ExampleClient(std::shared_ptr<Channel> channel)
        : stub_(ExampleService::NewStub(channel)) {}

    std::string SayHello(const std::string& name) {
        HelloRequest request;
        request.set_name(name);
        HelloReply reply;
        ClientContext context;

        Status status = stub_->SayHello(&context, request, &reply);

        if (status.ok()) {
            return reply.message();
        } else {
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<ExampleService::Stub> stub_;
};

int main() {
    ExampleClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    std::string user = "Adam";
    std::string reply = client.SayHello(user);

    std::cout << "Server responded: " << reply << std::endl;

    return 0;
}
