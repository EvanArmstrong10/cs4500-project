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

    /** Copying constructor */
    Schema(Schema& from) {
        rows_ = from.rows_;
        cols_ = from.cols_;
        row_cap = from.row_cap;
        col_cap = from.col_cap;
        col_types_ = from.col_types_;
    }

    /** Create an empty schema **/
    Schema() {
        rows_ = 0;
        cols_ = 0;
        row_cap = 2;
        col_cap = 2;
        col_types_ = new char[2];
    }

    /** Create a schema from a string of types. A string that contains
     * characters other than those identifying the four type results in
     * undefined behavior. The argument is external, a nullptr argument is
     * undefined. **/
    Schema(const char* types) {
        cols_ = strlen(types);
        rows_ = 0;
        col_types_ = duplicate(types);
        row_cap = 2;
        col_cap = strlen(types) + 2;
    }

    ~Schema() {
        delete[] col_types_;
    }

    bool row_at_capacity() {
        return rows_ >= row_cap;
    }

    bool col_at_capacity() {
        return cols_ >= col_cap;
    }

    /** Add a column of the given type and name (can be nullptr), name
     * is external. */
    void add_column(char typ, String* name) {
        strncat(col_types_, &typ, 1);
        cols_++;
    }

    /** Add a row with a name (possibly nullptr), name is external. */
    void add_row(String* name) {
        rows_++;
    }

    /** Return type of column at idx. An idx >= width is undefined. */
    char col_type(size_t idx) {
        exit_if_not(idx < cols_, "index out of bounds");
        return col_types_[idx];
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