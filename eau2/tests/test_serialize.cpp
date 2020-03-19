#include "../src/serial.h"
#include "../src/serializables.h"

void test_StringArray() {
  StringArray* s1 = new StringArray();
  unsigned char* serialized = Serialize::serialize(s1);
  StringArray* s2 = Serialize::deserialize_StringArray(serialized);

  assert(s1->equals(s2));

  puts("StringArray serialization test passed");

  delete s1;
  delete s2;
}

void test_DoubleArray() {
  DoubleArray* d1 = new DoubleArray();
  unsigned char* serialized = Serialize::serialize(d1);
  DoubleArray* d2 = Serialize::deserialize_DoubleArray(serialized);

  assert(d1->equals(d2));
  puts("DoubleArray serialization test passed");

  delete d1;
  delete d2;
}

void test_MsgKind() {
  MsgKind k1 = MsgKind::Nack;

  unsigned char* serialized = Serialize::serialize(k1);
  MsgKind k2 = Serialize::deserialize_MsgKind(serialized);

  assert(k1 == k2);
  puts("MsgKind serialization test passed");
}

void test_Message() {
  Message* m1 = new Message();

  unsigned char* serialized = Serialize::serialize(m1);
  Message* m2 = Serialize::deserialize_Message(serialized);

  assert(m1->equals(m2));
  puts("Message serialization test passed");
}

void test_Status() {
  Status* s1 = new Status();

  unsigned char* serialized = Serialize::serialize(s1);

  Status* s2 = Serialize::deserialize_Status(serialized);
  assert(s1->msg_->equals(s2->msg_));
  puts("Status serialization test passed");
}

void test_Register() {
  Register* r1 = new Register();
  unsigned char* serialized = Serialize::serialize(r1);
  Register* r2 = Serialize::deserialize_Register(serialized);
  assert(r1->equals(r2));
  puts("Register serialization test passed");
}

void test_Directory() {
  Directory* d1 = new Directory();
  unsigned char* serialized = Serialize::serialize(d1);
  Directory* d2 = Serialize::deserialize_Directory(serialized);
  assert(d1->equals(d2));
  puts("Directory serialization test passed");
}

int main(int argc, char** argv) {
  test_StringArray();
  test_DoubleArray();
  test_MsgKind();
  test_Message();
  test_Status();
  test_Register();
  test_Directory();

  return 0;
}
