//lang::C++

#include "../src/kv_store/kv.h"

void testKey() {

    Key* k = new Key("test", 0);
    Key* l = new Key("different", 0);
    Key* m = new Key("test", 1);
    Key* n = new Key("different", 0);

    assert(k->node == 0);
    assert(strcmp(k->name, "test") == 0);

    assert(!(k->equals(l)));
    assert(!(k->equals(m)));
    assert(l->equals(n));


}

void testValue() {

    size_t size = 101;
    float* vals = new float[size];

    FloatColumn* fc = new FloatColumn();
    for (int i = 0; i < size; i++) {
            fc->push_back(vals[i]);
    }
    Value val(fc);

    assert(val.size == 101);

}

void testKV() {

    Key* k1 = new Key("test", 0);
    Key* k2 = new Key("different", 0);

    size_t size = 101;
    float* vals = new float[size];
    bool* vals1 = new bool[size];

    FloatColumn* fc = new FloatColumn();
    BoolColumn* bc = new BoolColumn();
    for (int i = 0; i < size; i++) {
            fc->push_back(vals[i]);
            bc->push_back(vals1[i]);
    }

    Value v1(fc);
    Value v2(bc);

    KV* kv = new KV();
    kv->add_pair(k1, &v1);
    kv->add_pair(k2, &v2);

    assert(kv->size_ == 2);
    assert(kv->keys_[1]->equals(k2));
    assert(kv->vals_[0]->vals->useful_get_type == 'F');
    assert(kv->get(k1)->columns_ == 1);
    assert(kv->get(k1)->schema_->cols_ == 'F');

}


void testFromArray() {
    size_t SZ = 1000*1000;
    float* vals = new float[SZ];
    float sum = 0;
    for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
    Key key("triv",0);
    KV* kv = new KV();
    FloatColumn* fc = new FloatColumn();
    for (int i = 0; i < SZ; i++) {
            fc->push_back(vals[i]);
    }
    Value val(fc);
    kv->add_pair(&key, &val);
    DataFrame* df = DataFrame::fromArray(&key, kv, SZ, vals);
    assert(df->get_float(0,1) == 1);
    DataFrame* df2 = kv->get(&key);
    for (size_t i = 0; i < SZ; ++i) sum -= df2->get_float(0,i);
    assert(sum==0);
    delete df; delete df2;

}


int main(int argc, char** argv) {

    testKey();
    testValue();
    testKV();
    testFromArray();

    return 0;
}