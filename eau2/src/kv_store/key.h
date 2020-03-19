//lang::CwC

#include "../string.h"
#include "value.h"


class Key {

    public:

        String* name;
        size_t node;
        Value* val;

        Key(String* name, size_t node) {
            name = name;
            node = node;
            val = new Value();
        }

        ~Key() {

        }

        bool equals(Key* k) {
            return name->equals(k->name) && node == k->node;
        }
};
