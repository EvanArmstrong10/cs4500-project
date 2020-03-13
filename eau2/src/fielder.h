// lang::CwC

#ifndef Fielder_H
#define Fielder_H

#pragma once
#include "helper.h"
#include "object.h"
#include "row.h"
#include "string.h"


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
    Row* r_;

    PrintFielder(Row* r) {
        r_ = r;
    }

    /** Called for fields of the argument's type with the value of the field. */
    void accept(bool b) {
        char a = ' ';
        printf("%i", b);
        printf("%c", a);
    }
    void accept(float f) {
        char a = ' ';
        printf("%f", f);
        printf("%c", a);
    }
    void accept(int i) {
        char a = ' ';
        printf("%i", i);
        printf("%c", a);
    }
    void accept(String* s) {
        char a = ' ';
        printf("%s", s->c_str());
        printf("%c", a);
    }
    void start(size_t r) {
        r_->visit(r, *this);
    }

    void done() {
    }
};