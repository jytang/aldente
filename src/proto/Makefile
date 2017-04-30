ifeq ($(OS),Windows_NT)
	SHELL=C:/Windows/System32/cmd.exe
endif

PROTO_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

proto:
	protoc -I=$(PROTO_DIR) --cpp_out=$(PROTO_DIR) $(PROTO_DIR)/net.proto
	mv $(PROTO_DIR)/net.pb.cc $(PROTO_DIR)/net.pb.cpp
