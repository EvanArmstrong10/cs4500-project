#pragma once
#include "object.h"
#include "string.h"
#include "array.h"
#include <stdlib.h>

// Hashmap_pair is a node, which is used to to store a pair of key and value.
// Later a list of Hashmap_pair will be stored into Hashmap data structure.
// It inherit Object class.
class Hashmap_pair : public Object {
  public:
    Object *key_;
    Object *val_;

    Hashmap_pair(Object *key, Object *val) : Object() {
        key_ = key;
        val_ = val;
    }
    ~Hashmap_pair() {
    }

    // getter method that help to get value from Hashmap_pair.
    Object* getVal() { return val_; }

    // getter method that help to get key from Hashmap_pair.
    Object* getKey() { return key_; }
    };

// Hashmap class stores a list of hashmap_pairs, which contains equal number
// of keys and values.
// It has size and capcity, which size is the number of key-value pairs,
// and capcity is the physical size of hashmap.
class Hashmap : public Object {
  public:
    
    Array* data;
    IntArray* indeces;
    size_t size_;
    size_t capacity_;

    // constructor
    // capacity will be initilized as 32, size is 0 by default.
    Hashmap() {
        capacity_ = 16;
        size_ = 0;
        data = new Array(capacity_);
        indeces = new IntArray();
    }

    // destructor
    ~Hashmap() {
        delete data;
        delete indeces;
    }

    // hash and return unique hashcode. two hashmaps with 
    // same hashmap_pairs should have the same hashcode.
    size_t hash() {
        size_t hash = 1;
        for (size_t ii = 0; ii < indeces->size(); ii++) {
            int temp = indeces->get(ii);
            hash += temp * temp;
        }
        return hash;
    }


    // calculates a pair's index in the data array based on its hash
    size_t index_key_(Object* o) {
        return o->hash() % capacity_ + 1;
    }

    // Double the capacity of hashmap when needed
    void expand() {
        capacity_ = capacity_ * 2;
        Array* new_data = new Array(capacity_);
        for (size_t ii = 0; ii < indeces->size(); ii++) {
            size_t index = indeces->get(ii);
            new_data->set(index, data->get(index));
        }
        delete data;
        data = new_data;
    }

    // Returns the value to which the specified key is mapped,
    // or null if this map contains no mapping for the key.
    Object *get(Object *key) {
        return data->get(index_key_(key));
    }

    // Associates the specified value with the specified key in this map.
    void put(Object *key, Object *val) {

        size_t index = index_key_(key);
        Object* temp = data->get(index);
        Hashmap_pair* new_pair = new Hashmap_pair(key, val);
        
        // no hash-pair present, set hash-pair and record index
        if (temp == NULL) {
            data->set(index, new_pair);
            indeces->push_back(index);
            size_++;
            return;
        }
        // hash-pair already exists, check if keys are equal
        // if so, delete existing pair and create new one
        Hashmap_pair* p = dynamic_cast<Hashmap_pair*>(temp);
        if (p->getKey()->equals(key)) {
            delete p;
            data->set(index, new_pair);
        }
        // if pair already exists and keys arent equal, resize and try again
        else {
            expand();
            put(key, val);
            size_++;
        }
    }

    // Removes the mapping for the specified key from this map if present.
    void remove(Object *key) {
        size_t index = index_key_(key);
        Object* pair = data->remove(index);
        if (data->remove(index) == NULL) return;

        delete pair;
        // remove index from indeces as well
        for (size_t ii = 0; ii < indeces->size(); ii++) {
            if (indeces->get(ii) == index) {
                indeces->remove(ii);
            }
        }
        size_--;
    }

    // Returns the number of key-value mappings in this map.
    size_t size() {
        return size_;
    }

    // Returns a list view of the keys contained in this map.
    Object **key_array() {
        Object** keys = new Object*[indeces->size()];
        for (size_t ii = 0; ii < indeces->size(); ii++) {
            int index = indeces->get(ii);
            Hashmap_pair* pair = dynamic_cast<Hashmap_pair*>(data->get(index));
            keys[ii] = pair->getKey();
        }
        return keys;
    }

    // Check if two Hashmaps are equal.
    // the input hashmap is an object.
    bool equals(Object *map) {
        Hashmap* m = dynamic_cast<Hashmap*>(map);
        if (m == NULL) return 0;
        return hash() == m->hash();
    }
};
