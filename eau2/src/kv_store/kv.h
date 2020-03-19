//lang::CwC

#include "key.h"
#include "../dataframe.h"

class KV {

    public:

        size_t size_;
        size_t cap_;
        Key** keys_;

        KV () {
            size_ = 0;
            cap_ = 2;
            keys_ = new Key*[0];
        }

        /** Checks if the key array is at capacity. */
        bool at_capacity() {
        return cap_ <= size_;
        }

        void double_capacity() {
            cap_ *= 2;
            Key** temp = new Key*[cap_];
            for (size_t x = 0; x < size_; x++) {
                temp[x] = keys_[x];
            }
            delete[] keys_;
            keys_ = temp;
        }

        void add_key(Key* key) {
            if (at_capacity()) {
                double_capacity();
            }
            keys_[size_] = key;
            size_++;
        }

        DataFrame* get(Key* key) {
            
            for (int ii = 0; ii < size_; ii++) {
                if (keys_[ii]->equals(key)) {
                    DataFrame temp = DataFrame::fromArray(&keys_[ii], this, keys_[ii]->val->size, keys_[ii]->val->vals);
                }
            }
        }






};