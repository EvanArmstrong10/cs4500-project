//lang::CwC

#include "../string.h"


class Key {

    public:

        String* name;
        size_t node;

        Key(String* name, size_t node) {
            name = name;
            node = node;
        }

        ~Key() {

        }

        bool equals(Key* k) {
            return name->equals(k->name) && node == k->node;
        }
};
