//lang::CwC

#pragma once
#include "../string.h"


class Key {

    public:

        const char* name;
        size_t node;

        Key(const char* name, size_t node) {
            name = name;
            node = node;
        }

        ~Key() {

        }

        bool equals(Key* k) {
            return (strcmp(k->name, name) == 0) && node == k->node;
        }
};
