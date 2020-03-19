#pragma once
#include "object.h"
#include "serializables.h"

/* */
class Serialize {
 public:
  static unsigned char* serialize(StringArray* arr) {
    // calculate and allocate necessary buffer memory
    size_t serial_size = sizeof(size_t);
    for (size_t ii = 0; ii < arr->size_; ii++) {
      serial_size += sizeof(size_t) + arr->vals_[ii]->size();
    }
    unsigned char* buf = new unsigned char[serial_size + 1];
    size_t offset = 0;
    // copy number of Strings in StringArray to buffer
    memcpy(buf, &arr->size_, sizeof(size_t));
    offset += sizeof(size_t);
    // for each String, copy length of String and the String itself to buffer
    for (size_t ii = 0; ii < arr->size_; ii++) {
      size_t str_size = arr->vals_[ii]->size();
      memcpy(buf + offset, &str_size, sizeof(size_t));
      offset += sizeof(size_t);
      memcpy(buf + offset, arr->vals_[ii]->c_str(), str_size);
      offset += str_size;
    }
    // null terminate and return serialized char*
    buf[serial_size] = '\0';
    return buf;
  }

  static unsigned char* serialize(DoubleArray* arr) {
    // calculate and allocate necessary buffer memory
    size_t serial_size = sizeof(size_t) * arr->size_ * sizeof(double);

    unsigned char* buf = new unsigned char[serial_size + 1];
    size_t offset = 0;

    // copy number of double in DoubleArray to buffer
    memcpy(buf, &arr->size_, sizeof(size_t));
    offset += sizeof(size_t);

    // copy each double
    for (size_t ii = 0; ii < arr->size_; ii++) {
      memcpy(buf + offset, &arr->vals_[ii], sizeof(double));
      offset += sizeof(double);
    }

    // null terminate and return serialized char*
    buf[serial_size] = '\0';
    return buf;
  }

  static unsigned char* serialize(MsgKind k) {
    static unsigned char* buf = new unsigned char[sizeof(MsgKind)];
    memcpy(buf, &k, sizeof(MsgKind));
    return buf;
  }

  static unsigned char* serialize(Message* msg) {
    // calculate and allocate necessary buffer memory
    size_t serial_size = sizeof(MsgKind) + 3 * sizeof(size_t);

    unsigned char* buf = new unsigned char[serial_size + 1];
    size_t offset = 0;

    // copy MsgKind
    memcpy(buf, &msg->kind_, sizeof(MsgKind));
    offset += sizeof(MsgKind);

    // copy size_t fields
    memcpy(buf + offset, &msg->sender_, sizeof(size_t));
    offset += sizeof(size_t);
    memcpy(buf + offset, &msg->target_, sizeof(size_t));
    offset += sizeof(size_t);
    memcpy(buf + offset, &msg->id_, sizeof(size_t));
    offset += sizeof(size_t);

    // null terminate and return serialized char*
    buf[serial_size] = '\0';
    return buf;
  }

  static unsigned char* serialize(Ack* msg) {
    unsigned char* buf = new unsigned char[1];
    buf[0] = '\0';
    return buf;
  }

  static unsigned char* serialize(Status* msg) {
    size_t serial_size = sizeof(size_t);
    size_t msg_len = msg->msg_->size();

    serial_size += msg_len;
    unsigned char* buf = new unsigned char[serial_size + 1];
    int offset = 0;

    // copy number of double in DoubleArray to buffer
    memcpy(buf + offset, &msg_len, sizeof(size_t));
    offset += sizeof(size_t);

    memcpy(buf + offset, msg->msg_->c_str(), msg_len);
    buf[serial_size] = '\0';
    return buf;
  }

  static unsigned char* serialize(Register* r) {
    size_t serial_size = sizeof(short) + sizeof(unsigned short) +
                         sizeof(unsigned long) + sizeof(size_t);
    unsigned char* buf = new unsigned char[serial_size + 1];
    int offset = 0;

    // sockaddr_in.sin_family
    short sfamily = r->client.sin_family;
    memcpy(buf + offset, &sfamily, sizeof(short));
    offset += sizeof(short);

    // sockaddr_in.sin_port
    unsigned short sport = r->client.sin_port;
    memcpy(buf + offset, &sport, sizeof(unsigned short));
    offset += sizeof(unsigned short);

    // sockaddr_in.sin_addr
    unsigned long saddr = r->client.sin_addr.s_addr;
    memcpy(buf + offset, &saddr, sizeof(unsigned long));
    offset += sizeof(unsigned long);

    memcpy(buf + offset, &r->port, sizeof(size_t));
    offset += sizeof(size_t);

    buf[serial_size] = '\0';
    return buf;
  }

  static unsigned char* serialize(Directory* d) {
    size_t serial_size = 3 * sizeof(size_t) + d->num_ports * sizeof(size_t);

    for (size_t ii = 0; ii < d->num_addrs; ii++) {
      serial_size += d->addresses[ii]->size() + sizeof(size_t);
    }
    unsigned char* buf = new unsigned char[serial_size + 1];
    size_t offset = 0;

    memcpy(buf + offset, &d->client, sizeof(size_t));
    offset += sizeof(size_t);

    memcpy(buf + offset, &d->num_ports, sizeof(size_t));
    offset += sizeof(size_t);

    memcpy(buf + offset, d->ports, d->num_ports * sizeof(size_t));
    offset += d->num_ports * sizeof(size_t);

    memcpy(buf + offset, &d->num_addrs, d->num_addrs * sizeof(size_t));
    offset += d->num_addrs * sizeof(size_t);

    for (size_t ii = 0; ii < d->num_addrs; ii++) {
      size_t str_len = d->addresses[ii]->size();
      memcpy(buf + offset, &str_len, sizeof(size_t));
      offset += sizeof(size_t);
      memcpy(buf + offset, d->addresses[ii]->c_str(), str_len);
      offset += str_len;
    }

    buf[serial_size] = '\0';
    return buf;
  }

  static StringArray* deserialize_StringArray(unsigned char* serialized) {
    // deserialize number of Strings in StringArray
    size_t size_;
    memcpy(&size_, serialized, 8);
    String** vals_ = new String*[size_];
    serialized += 8;

    // deserialize each String in StringArray
    for (size_t ii = 0; ii < size_; ii++) {
      // copy the length of the string
      size_t str_len;
      memcpy(&str_len, serialized, 8);
      serialized += 8;
      char* str = new char[str_len + 1];
      // copy string
      memcpy(str, serialized, str_len);
      serialized += str_len;
      str[str_len] = '\0';
      vals_[ii] = new String(str);
    }
    return new StringArray(vals_, size_);
  }

  static DoubleArray* deserialize_DoubleArray(unsigned char* serialized) {
    // deserialize number of Strings in StringArray
    size_t size_;
    memcpy(&size_, serialized, 8);
    double* vals_ = new double[size_];
    serialized += 8;

    // deserialize each String in StringArray
    for (size_t ii = 0; ii < size_; ii++) {
      double d;
      memcpy(&d, serialized, 8);
      serialized += 8;
      vals_[ii] = d;
    }
    return new DoubleArray(vals_, size_);
  }

  static MsgKind deserialize_MsgKind(unsigned char* serialized) {
    MsgKind k;
    memcpy(&k, serialized, sizeof(MsgKind));
    return k;
  }

  static Message* deserialize_Message(unsigned char* serialized) {
    // deserialize number of Strings in StringArray
    MsgKind kind;
    memcpy(&kind, serialized, sizeof(MsgKind));
    serialized += sizeof(MsgKind);

    size_t sender;
    memcpy(&sender, serialized, sizeof(size_t));
    serialized += sizeof(size_t);

    size_t target;
    memcpy(&target, serialized, sizeof(size_t));
    serialized += sizeof(size_t);

    size_t id;
    memcpy(&id, serialized, sizeof(size_t));
    serialized += sizeof(size_t);

    return new Message(kind, sender, target, id);
  }

  static Ack* deserialize_Ack(unsigned char* serialized) { return new Ack(); }

  static Status* deserialize_Status(unsigned char* serialized) {
    size_t msg_len;
    memcpy(&msg_len, serialized, sizeof(size_t));
    serialized += sizeof(size_t);

    char* msg = new char[msg_len + 1];
    memcpy(msg, serialized, msg_len);
    msg[msg_len] = '\0';

    return new Status(new String(msg));
  }

  static Register* deserialize_Register(unsigned char* serialized) {
    sockaddr_in client;

    // sockaddr_in.sin_family
    short sfamily;
    memcpy(&sfamily, serialized, sizeof(short));
    serialized += sizeof(short);

    // sockaddr_in.sin_port
    unsigned short sport;
    memcpy(&sport, serialized, sizeof(unsigned short));
    serialized += sizeof(unsigned short);

    // sockaddr_in.sin_addr
    in_addr addr;
    unsigned long saddr;
    memcpy(&saddr, serialized, sizeof(long));
    serialized += sizeof(long);
    addr.s_addr = saddr;

    // port
    size_t port;
    memcpy(&port, serialized, sizeof(size_t));

    client.sin_family = sfamily;
    client.sin_port = sport;
    client.sin_addr = addr;
    return new Register(client, port);
  }

  static Directory* deserialize_Directory(unsigned char* serialized) {
    size_t client;
    memcpy(&client, serialized, sizeof(size_t));
    serialized += sizeof(size_t);

    size_t num_ports;
    memcpy(&num_ports, serialized, sizeof(size_t));
    serialized += sizeof(size_t);

    size_t* ports = new size_t[num_ports];
    memcpy(ports, serialized, num_ports * sizeof(size_t));
    serialized += num_ports * sizeof(size_t);

    size_t num_addrs;
    memcpy(&num_addrs, serialized, sizeof(size_t));
    serialized += sizeof(size_t);

    String** addresses = new String*[num_addrs];
    size_t str_len;
    for (size_t ii = 0; ii < num_addrs; ii++) {
      memcpy(&str_len, serialized, sizeof(size_t));
      serialized += sizeof(size_t);
      char* str = new char[str_len + 1];
      memcpy(str, serialized, str_len);
      str[str_len] = '\0';
      addresses[ii] = new String(str);
      serialized += str_len;
    }

    return new Directory(client, num_ports, ports, num_addrs, addresses);
  }
};