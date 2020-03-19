//lang::CwC

#pragma once
#include "../column.h"

class Value {

    public:
        size_t size;
        Column* vals;

        Value() {
            size = 0;
        }

        Value(Column* col) {
            size = col->size;
            vals = col;
        }
};