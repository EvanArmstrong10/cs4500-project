//lang::CwC

#pragma once
#include "../dataframe.h"
#include "key.h"
#include "value.h"

class DataFrame;

class KV {

    public:

        size_t size_;
        size_t cap_;
        Key** keys_;
        Value** vals_;


        KV () {
            size_ = 0;
            cap_ = 2;
            keys_ = new Key*[0];
            vals_ = new Value*[0];
        }

        /** Checks if the key array is at capacity. */
        bool at_capacity() {
            return cap_ <= size_;
        }

        void double_capacity() {
            cap_ *= 2;
            Key** temp = new Key*[cap_];
            Value** valtemp = new Value*[cap_];
            for (size_t x = 0; x < size_; x++) {
                temp[x] = keys_[x];
                valtemp[x] = vals_[x];
            }
            delete[] keys_;
            delete[] vals_;
            keys_ = temp;
            vals_ = valtemp;
        }

        void add_pair(Key* key, Value* val) {
            if (at_capacity()) {
                double_capacity();
            }
            keys_[size_] = key;
            vals_[size_] = val;
            size_++;
        }

        DataFrame* get(Key* key) {
            DataFrame* temp = new DataFrame();
            for (int ii = 0; ii < size_; ii++) {
                if (keys_[ii]->equals(key)) {
                    temp = DataFrame::fromArray(keys_[ii], this, vals_[ii]->size, vals_[ii]->vals);
                }
            }
            return temp;
        }






};