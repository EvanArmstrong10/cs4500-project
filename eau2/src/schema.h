// lang::CwC

#pragma once
#include "object.h"
#include "string.h"
#include <string.h>
#include "helper.h"

/*************************************************************************
 * Schema::
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'F'.
 */
class Schema : public Object {
  public:
    size_t rows_;
    size_t cols_;
    size_t row_cap;
    size_t col_cap;
    char* col_types_;
    String** cnames_;
    String** rnames_;

    /** Copying constructor */
    Schema(Schema& from) {
        rows_ = from.rows_;
        cols_ = from.cols_;
        row_cap = from.row_cap;
        col_cap = from.col_cap;
        col_types_ = from.col_types_;
        cnames_ = from.cnames_;
        rnames_ = from.rnames_;
    }

    /** Create an empty schema **/
    Schema() {
        rows_ = 0;
        cols_ = 0;
        row_cap = 2;
        col_cap = 2;
        col_types_ = new char[2];
        cnames_ = new String*[2];
        rnames_ = new String*[2];
    }

    /** Create a schema from a string of types. A string that contains
     * characters other than those identifying the four type results in
     * undefined behavior. The argument is external, a nullptr argument is
     * undefined. **/
    Schema(const char* types) {
        cols_ = strlen(types);
        rows_ = 0;
        col_types_ = duplicate(types);
        cnames_ = new String*[strlen(types)];
        rnames_ = new String*[2];
        row_cap = 2;
        col_cap = strlen(types) + 2;
    }

    ~Schema() {
        // delete[] col_types_;
        delete[] cnames_;
    }

    bool row_at_capacity() {
        return rows_ >= row_cap;
    }

    bool col_at_capacity() {
        return cols_ >= col_cap;
    }

    void double_col_capacity() {
        col_cap *= 2;
        String** temp = new String*[col_cap];
        for (size_t x = 0; x < cols_; x++) {
            temp[x] = cnames_[x];
        }
        cnames_ = temp;
    }

    void double_row_capacity() {
        row_cap *= 2;
        String** temp = new String*[row_cap];
        for (size_t x = 0; x < rows_; x++) {
            temp[x] = rnames_[x];
        }
        rnames_ = temp;
    }

    /** Add a column of the given type and name (can be nullptr), name
     * is external. */
    void add_column(char typ, String* name) {
        if (col_at_capacity()) {
            double_col_capacity();
        }
        strncat(col_types_, &typ, 1);
        cnames_[cols_] = name;
        cols_++;
    }

    /** Add a row with a name (possibly nullptr), name is external. */
    void add_row(String* name) {
        if (row_at_capacity()) {
            double_row_capacity();
        }
        rnames_[rows_] = name;
        rows_++;
    }

    /** Return name of row at idx; nullptr indicates no name. An idx >= width
     * is undefined. */
    String* row_name(size_t idx) {
        exit_if_not(idx > rows_ - 1, "index out of bounds");
        if (rnames_[idx] == 0) {
            return nullptr;
        }
        return rnames_[idx];
    }

    /** Return name of column at idx; nullptr indicates no name given.
     *  An idx >= width is undefined.*/
    String* col_name(size_t idx) {
        exit_if_not(idx > cols_ - 1, "index out of bounds");
        if (cnames_[idx] == 0) {
            return nullptr;
        }
        return cnames_[idx];
    }

    /** Return type of column at idx. An idx >= width is undefined. */
    char col_type(size_t idx) {
        exit_if_not(idx < cols_, "index out of bounds");
        return col_types_[idx];
    }

    /** Given a column name return its index, or -1. */
    int col_idx(const char* name) {
        String* temp = new String(name);
        for (int ii = 0; ii < cols_; ii++) {
            if (cnames_[ii] == temp) {
                return ii;
            }
        }
        return -1;
    }

    /** Given a row name return its index, or -1. */
    int row_idx(const char* name) {
        String* temp = new String(name);
        for (int ii = 0; ii < rows_; ii++) {
            if (rnames_[ii] == temp) {
                return ii;
            }
        }
        return -1;
    }

    /** The number of columns */
    size_t width() {
        return cols_;
    }

    /** The number of rows */
    size_t length() {
        return rows_;
    }
};