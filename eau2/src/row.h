// lang::CwC

#pragma once
#include "column.h"
#include "fielder.h"
#include "object.h"
#include "schema.h"
#include "string.h"


/*************************************************************************
 * Row::
 *
 * This class represents a single row of data constructed according to a
 * dataframe's schema. The purpose of this class is to make it easier to add
 * read/write complete rows. Internally a dataframe hold data in columns.
 * Rows have pointer equality.
 */
class Row : public Object {
  public:
    size_t size_;
    size_t idx_;

    size_t* order_;
    size_t* meta_idxs_;
    size_t* counts_;
    Column** vals_;


    void init_() {
        size_ = 0;
        order_ = new size_t[size_];
        meta_idxs_ = new size_t[size_];

        vals_ = new Column*[4];
        vals_[0] = new IntColumn();
        vals_[1] = new FloatColumn();
        vals_[2] = new BoolColumn();
        vals_[3] = new StringColumn();
        counts_ = new size_t[4];
        for (size_t ii = 0; ii < 4; ii++) {
            counts_[ii] = 0;
        }
    }

    /** Build a row following a schema. */
    Row(Schema& scm) {
        size_ = scm.cols_;
        init_();
    }

    /** Setters: set the given column with the given value. Setting a column with
     * a value of the wrong type is undefined. */
    void set(size_t col, int val) {
        IntColumn* ic = vals_[0]->as_int();
        ic->push_back(val);
        meta_idxs_[col] = counts_[0];
        order_[col] = 0;
        counts_[0]++;
    }

    void set(size_t col, float val) {
        FloatColumn* fc = vals_[1]->as_float();
        fc->push_back(val);
        meta_idxs_[col] = counts_[1];
        order_[col] = 1;
        counts_[1]++;
    }

    void set(size_t col, bool val) {
        BoolColumn* bc = vals_[2]->as_bool();
        bc->push_back(val);
        meta_idxs_[col] = counts_[2];
        order_[col] = 2;
        counts_[2]++;
    }

    /** Acquire ownership of the string. */
    void set(size_t col, String* val) {
        StringColumn* sc = vals_[3]->as_string();
        sc->push_back(val);
        meta_idxs_[col] = counts_[3];
        order_[col] = 3;
        counts_[3]++;
    }

    /** Set/get the index of this row (ie. its position in the dataframe. This is
     *  only used for informational purposes, unused otherwise */
    void set_idx(size_t idx) {
        idx_ = idx;
    }
    size_t get_idx() {
        return idx_;
    }

    /** Getters: get the value at the given column. If the column is not
     * of the requested type, the result is undefined. */
    int get_int(size_t col) {
        size_t type_idx = order_[col];
        size_t val_idx = meta_idxs_[col];
        return vals_[type_idx]->as_int()->get(val_idx);
    }

    bool get_bool(size_t col) {
        size_t type_idx = order_[col];
        size_t val_idx = meta_idxs_[col];
        return vals_[type_idx]->as_bool()->get(val_idx);
    }

    float get_float(size_t col) {
        size_t type_idx = order_[col];
        size_t val_idx = meta_idxs_[col];
        return vals_[type_idx]->as_float()->get(val_idx);
    }

    String* get_string(size_t col) {
        size_t type_idx = order_[col];
        size_t val_idx = meta_idxs_[col];
        return vals_[type_idx]->as_string()->get(val_idx);
    }

    /** Number of fields in the row. */
    size_t width() {
        return size_;
    }

    /** Type of the field at the given position. An idx >= width is  undefined. */
    char col_type(size_t idx) {
        return vals_[order_[idx]]->useful_get_type();
    }

    /** Given a Fielder, visit every field of this row.
     * Calling this method before the row's fields have been set is undefined. */
    void visit(size_t idx, Fielder& f) {
        for (int ii = 0; ii < size_; ii++) {
            size_t type = order_[ii];
            switch (type) {
            case 0: {
                IntColumn* col = vals_[type]->as_int();
                f.accept(col->get(idx));
            }
            case 1: {
                FloatColumn* col = vals_[type]->as_float();
                f.accept(col->get(idx));
            }
            case 2: {
                BoolColumn* col = vals_[type]->as_bool();
                f.accept(col->get(idx));
            }
            case 3: {
                StringColumn* col = vals_[type]->as_string();
                f.accept(col->get(idx));
            }
            default: {}
            }
        }
    }
};