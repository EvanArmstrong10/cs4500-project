// lang::CwC

#pragma once
#include "helper.h"
#include "object.h"
#include "row.h"
#include "string.h"

class Row;

/*****************************************************************************
 * Fielder::
 * A field vistor invoked by Row.
 */
class Fielder : public Object {

  public:
    /** Called before visiting a row, the argument is the row offset in the
      dataframe. */
    virtual void start(size_t r) {
    }

    /** Called for fields of the argument's type with the value of the field. */
    virtual void accept(bool b) {
    }
    virtual void accept(float f) {
    }
    virtual void accept(int i) {
    }
    virtual void accept(String* s) {
    }

    /** Called when all fields have been seen. */
    virtual void done() {
    }
};

class PrintFielder : public Fielder {
  public:
    PrintFielder() {
    }

    /** Called for fields of the argument's type with the value of the field. */
    void accept(bool b) {
        printf("%i ", b);
    }
    void accept(float f) {
        printf("%f ", f);
    }
    void accept(int i) {
        printf("%i ", i);
    }
    void accept(String* s) {
        printf("%s ", s->c_str());
    }
    void start(size_t r) {
        // r_->visit(r, *this);
    }

    void done() {
    }
};
