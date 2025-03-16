cmake_minimum_required(VERSION 3.15)
project(GrpcExample CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

# Find gRPC and Protobuf
find_package(gRPC REQUIRED)
find_package(protobuf REQUIRED)

# Paths
set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)

# Ensure generated directory exists
file(MAKE_DIRECTORY ${GENERATED_DIR})

# Define .proto files
set(PROTO_FILES ${PROTO_DIR}/example.proto)

# Generate sources using protoc
add_custom_command(
    OUTPUT ${GENERATED_DIR}/example.pb.cc ${GENERATED_DIR}/example.pb.h
           ${GENERATED_DIR}/example.grpc.pb.cc ${GENERATED_DIR}/example.grpc.pb.h
    COMMAND protobuf::protoc
    ARGS --proto_path=${PROTO_DIR}
         --cpp_out=${GENERATED_DIR}
         --grpc_out=${GENERATED_DIR}
         --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
         ${PROTO_FILES}
    DEPENDS ${PROTO_FILES}
    COMMENT "Generating protobuf and gRPC code"
    VERBATIM
)

# Add generated files as sources
set(GENERATED_SRCS
    ${GENERATED_DIR}/example.pb.cc
    ${GENERATED_DIR}/example.grpc.pb.cc
)

set(GENERATED_HDRS
    ${GENERATED_DIR}/example.pb.h
    ${GENERATED_DIR}/example.grpc.pb.h
)

# Create a library from the generated code
add_library(example_proto ${GENERATED_SRCS} ${GENERATED_HDRS})
target_include_directories(example_proto PUBLIC ${GENERATED_DIR})
target_link_libraries(example_proto PUBLIC gRPC::grpc++ protobuf::libprotobuf)

# Server executable
add_executable(server server.cpp)
target_link_libraries(server PRIVATE example_proto gRPC::grpc++ protobuf::libprotobuf)

# Client executable
add_executable(client client.cpp)
target_link_libraries(client PRIVATE example_proto gRPC::grpc++ protobuf::libprotobuf)
