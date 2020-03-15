#pragma once
#include <netinet/in.h>
#include <stdio.h>
#include "string.h"

class StringArray : public Object {
 public:
  String** vals_;
  size_t size_;

  // Default constructor
  // Hardcoded values for simplified testing
  StringArray() {
    size_ = 3;
    vals_ = new String*[3];
    vals_[0] = new String("camp");
    vals_[1] = new String("aquarium");
    vals_[2] = new String("pool");
  }

  // constructor for easy rebuilding from serialized StringArray
  StringArray(String** v, size_t s) {
    vals_ = v;
    size_ = s;
  }

  ~StringArray() {
    for (size_t ii = 0; ii < size_; ii++) {
      delete vals_[ii];
    }
    delete[] vals_;
  }

  bool equals(Object* o) {
    StringArray* arr = dynamic_cast<StringArray*>(o);
    if (arr == nullptr) return false;
    for (size_t ii = 0; ii < size_; ii++) {
      if (!vals_[ii]->equals(arr->vals_[ii])) return false;
    }
    return true;
  }
};

class DoubleArray : public Object {
 public:
  double* vals_;
  size_t size_;

  // Default constructor
  // Hardcoded values for simplified testing
  DoubleArray() {
    size_ = 3;
    vals_ = new double[3];
    vals_[0] = 12.12;
    vals_[1] = 21.21;
    vals_[2] = 1;
  }

  // constructor for easy rebuilding from serialized StringArray
  DoubleArray(double* v, size_t s) {
    vals_ = v;
    size_ = s;
  }

  ~DoubleArray() { delete[] vals_; }

  bool equals(Object* o) {
    DoubleArray* arr = dynamic_cast<DoubleArray*>(o);
    for (size_t ii = 0; ii < size_; ii++) {
      if (!(vals_[ii] == arr->vals_[ii])) return false;
    }
    return true;
  }
};

enum class MsgKind {
  Ack,
  Nack,
  Put,

  Reply,
  Get,
  WaitAndGet,
  Status,

  Kill,
  Register,
  Directory
};

class Message : public Object {
 public:
  MsgKind kind_;  // the message kind

  size_t sender_;  // the index of the sender node

  size_t target_;  // the index of the receiver node

  size_t id_;  // an id t unique within the node

  Message() {
    kind_ = MsgKind::Ack;
    sender_ = 0;
    target_ = 0;
    id_ = 0;
  }

  Message(MsgKind k, size_t s, size_t t, size_t i) {
    kind_ = k;
    sender_ = s;
    target_ = t;
    id_ = i;
  }

  bool equals(Object* o) {
    Message* msg = dynamic_cast<Message*>(o);
    if (msg == nullptr) return false;
    if (kind_ == msg->kind_ && sender_ == msg->sender_ &&
        target_ == msg->target_ && id_ == msg->id_) {
      return true;
    }
    return false;
  }
};

class Ack : public Message {};

class Status : public Message {
 public:
  String* msg_;  // owned

  Status() { msg_ = new String("golfcart"); }

  Status(String* msg) { msg_ = msg; }
};

class Register : public Message {
 public:
  sockaddr_in client;
  size_t port;

  Register() {
    port = 25565;
    client.sin_family = 2;
    client.sin_port = 255;
    in_addr a;
    a.s_addr = 300;
    client.sin_addr = a;
  }

  Register(sockaddr_in c, size_t p) { client = c, port = p; }

  bool equals(Object* o) {
    Register* r = dynamic_cast<Register*>(o);
    if (r == nullptr) return false;
    if (client.sin_family != r->client.sin_family ||
        client.sin_port != r->client.sin_port || port != r->port) {
      return false;
    }
    return true;
  }

  void print() { printf("port: %lu\n", port); }
};

class Directory : public Message {
 public:
  size_t client;

  size_t num_ports;
  size_t* ports;  // owned

  size_t num_addrs;
  String** addresses;  // owned; strings owned

  Directory() {
    client = 1234;
    num_ports = 1;
    ports = new size_t[1];
    ports[0] = 25565;
    num_addrs = 1;
    addresses = new String*[1];
    addresses[0] = new String("bologna");
  }

  Directory(size_t c, size_t np, size_t* p, size_t na, String** a) {
    client = c;
    num_ports = np;
    ports = p;
    num_addrs = na;
    addresses = a;
  }

  bool equals(Object* other) {
    Directory* d = dynamic_cast<Directory*>(other);
    if (d == nullptr) return false;
    if (client != d->client || num_ports != d->num_ports ||
        num_addrs != d->num_addrs) {
      return false;
    }
    for (size_t ii = 0; ii < num_ports; ii++) {
      if (ports[ii] != d->ports[ii]) {
        return false;
      }
    }
    for (size_t ii = 0; ii < num_addrs; ii++) {
      if (!addresses[ii]->equals(d->addresses[ii])) {
        return false;
      }
    }

    return true;
  }
};