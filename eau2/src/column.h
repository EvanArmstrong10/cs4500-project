// lang::CwC

#pragma once
#include "fielder.h"
#include "helper.h"
#include "object.h"
#include "string.h"
#include <stdarg.h>

Sys sys;

class IntColumn;
class BoolColumn;
class FloatColumn;
class StringColumn;


/**************************************************************************
 * Column ::
 * Represents one column of a data frame which holds values of a single type.
 * This abstract class defines methods overriden in subclasses. There is
 * one subclass per element type. Columns are mutable, equality is pointer
 * equality. */
class Column : public Object {
  public:
    /** Type converters: Return same column under its actual type, or
     *  nullptr if of the wrong type.  */
    virtual IntColumn* as_int() {
        return nullptr;
    }
    virtual BoolColumn* as_bool() {
        return nullptr;
    }
    virtual FloatColumn* as_float() {
        return nullptr;
    }
    virtual StringColumn* as_string() {
        return nullptr;
    }

    /** Type appropriate push_back methods. Calling the wrong method is
     * undefined behavior. **/
    virtual void push_back(int val) {
        sys.exit_if_not(0, "Pushed incorrect datatype.");
    }
    virtual void push_back(bool val) {
        sys.exit_if_not(0, "Pushed incorrect datatype.");
    }
    virtual void push_back(float val) {
        sys.exit_if_not(0, "Pushed incorrect datatype.");
    }
    virtual void push_back(String* val) {
        sys.exit_if_not(0, "Pushed incorrect datatype.");
    }

    /** Returns the number of elements in the column. */
    virtual size_t size() {
        sys.exit_if_not(0, "Bad size call");
    }

    /** Return the type of this column but ~virtual~ **/
    virtual char useful_get_type() {
        sys.exit_if_not(0, "Bad get_type call");
    }

    /** Return the type of this column as a char: 'S', 'B', 'I' and 'F'. */
    char get_type() {
        useful_get_type();
    }

    /** Visitor accept method **/
    void accept(Fielder visitor, size_t idx) {
        sys.exit_if_not(0, "Bad visitor call");
    }
};

/*************************************************************************
 * IntColumn::
 * Holds primitive int values, unwrapped.
 */
class IntColumn : public Column {
  public:
    int** data_;      // a variable size array of fixed length arrays to avoid payload copying
    size_t size_;     // number of data in this column
    size_t cap_;      // capacity of each fixed size data array
    size_t data_idx_; // index of the last allocated array of data_
    size_t num_arrs_; // number of arrays currently holding data

    IntColumn() {
        cap_ = 4096;
        num_arrs_ = 32;
        data_ = new int*[num_arrs_];
        data_[0] = new int[cap_];
        data_idx_ = 0;
        size_ = 0;
    }

    IntColumn(int n, ...) : IntColumn() {
        va_list args;
        va_start(args, n);
        for (int i = 0; i < n; i++) {
            push_back(va_arg(args, int));
        }
        va_end(args);
        size_ = n;
    }

    ~IntColumn() {
        for (int i = 0; i <= data_idx_; i++) {
            delete[] data_[i];
        }
        delete data_;
    }


    int get(size_t idx) {
        sys.exit_if_not(idx < size_, "Index out of bounds");
        size_t remainder = idx % cap_;
        return data_[data_idx_][remainder];
    }


    void set(size_t idx, int val) {
        sys.exit_if_not(idx < size_, "Index out of bounds");

        // indexing logic
        size_t quotent = idx / cap_;
        size_t remainder = idx % cap_;

        data_[quotent][remainder] = val;
    }

    /** Resizes the data structure that holds that the data arrays **/
    void resize_() {
        int** temp = new int*[num_arrs_ * 2];
        for (int ii = 0; ii < num_arrs_; ii++) {
            temp[ii] = new int[cap_];
            for (int jj = 0; jj < cap_; jj++) {
                temp[ii][jj] = data_[ii][jj];
            }
            delete[] data_[ii];
        }
        delete[] data_;
        num_arrs_ *= 2;
        data_ = temp;
    }

    virtual void push_back(int val) {
        size_t remainder;
        if (size_ == cap_ * (data_idx_ + 1)) {
            if (data_idx_ == num_arrs_) {
                resize_();
            }
            data_idx_++;
            data_[data_idx_] = new int[cap_];
        }
        if (size_ < cap_) {
            remainder = size_;
        } else {
            remainder = size_ % cap_;
        }
        data_[data_idx_][remainder] = val;
        size_++;
    }

    IntColumn* as_int() {
        return this;
    }

    /** Return the type of this column but ~virtual~ **/
    char useful_get_type() {
        return 'I';
    }

    /** Return the number of elements in this column **/
    size_t size() {
        return size_;
    }

    /** Visitor accept method **/
    void accept(Fielder visitor, size_t idx) {
        visitor.accept(get(idx));
    }
};

/*************************************************************************
 * BoolColumn::
 * Holds primitive bool values, unwrapped.
 */
class BoolColumn : public Column {
  public:
    bool** data_;
    size_t size_;     // number of elements in this column
    size_t cap_;      // capacity of each fixed size data array
    size_t data_idx_; // index of the last allocated array of data_
    size_t num_arrs_; // number of arrays currently holding data

    BoolColumn() {
        cap_ = 4096;
        num_arrs_ = 32;
        data_ = new bool*[num_arrs_];
        data_[0] = new bool[cap_];
        data_idx_ = 0;
        size_ = 0;
    }

    BoolColumn(int n, ...) : BoolColumn() {
        va_list args;
        va_start(args, n);
        for (int i = 0; i < n; i++) {
            bool b = va_arg(args, int);
            push_back(b);
        }
        va_end(args);
        size_ = n;
    }

    bool get(size_t idx) {
        sys.exit_if_not(idx < size_, "Index out of bounds");
        size_t remainder = idx % cap_;
        return data_[data_idx_][remainder];
    }


    void set(size_t idx, bool val) {
        sys.exit_if_not(idx < size_, "Index out of bounds");

        // indexing logic
        size_t quotent = idx / cap_;
        size_t remainder = idx % cap_;

        data_[quotent][remainder] = val;
    }

    void resize_() {
        bool** temp = new bool*[num_arrs_ * 2];
        for (int ii = 0; ii < num_arrs_; ii++) {
            temp[ii] = new bool[cap_];
            for (int jj = 0; jj < cap_; jj++) {
                temp[ii][jj] = data_[ii][jj];
            }
            delete[] data_[ii];
        }
        delete[] data_;
        num_arrs_ *= 2;
        data_ = temp;
    }

    virtual void push_back(bool val) {
        size_t remainder;
        if (size_ == cap_ * (data_idx_ + 1)) {
            if (data_idx_ == num_arrs_) {
                resize_();
            }
            data_idx_++;
            data_[data_idx_] = new bool[cap_];
        }
        if (size_ < cap_) {
            remainder = size_;
        } else {
            remainder = size_ % cap_;
        }
        data_[data_idx_][remainder] = val;
        size_++;
    }

    BoolColumn* as_bool() {
        return this;
    }

    /** Return the type of this column but ~virtual~ **/
    char useful_get_type() {
        return 'B';
    }

    /** Return the number of elements in this column **/
    size_t size() {
        return size_;
    }

    /** Visitor accept method **/
    void accept(Fielder visitor, size_t idx) {
        visitor.accept(get(idx));
    }
};

/*************************************************************************
 * FloatColumn::
 * Holds primitive int values, unwrapped.
 */
class FloatColumn : public Column {
  public:
    float** data_;
    size_t size_;     // number of elements in this column
    size_t cap_;      // capacity of each fixed size data array
    size_t data_idx_; // index of the last allocated array of data_
    size_t num_arrs_; // number of arrays currently holding data

    FloatColumn() {
        cap_ = 4096;
        num_arrs_ = 32;
        data_ = new float*[num_arrs_];
        data_[0] = new float[cap_];
        data_idx_ = 0;
        size_ = 0;
    }

    FloatColumn(int n, ...) : FloatColumn() {
        va_list args;
        va_start(args, n);
        for (int i = 0; i < n; i++) {
            float f = va_arg(args, double);
            push_back(f);
        }
        va_end(args);
        size_ = n;
    }

    float get(size_t idx) {
        sys.exit_if_not(idx < size_, "Index out of bounds");
        size_t remainder = idx % cap_;
        return data_[data_idx_][remainder];
    }


    void set(size_t idx, float val) {
        sys.exit_if_not(idx < size_, "Index out of bounds");
        // indexing logic
        size_t quotent = idx / cap_;
        size_t remainder = idx % cap_;
        data_[quotent][remainder] = val;
    }

    void resize_() {
        float** temp = new float*[num_arrs_ * 2];
        for (int ii = 0; ii < num_arrs_; ii++) {
            temp[ii] = new float[cap_];
            for (int jj = 0; jj < cap_; jj++) {
                temp[ii][jj] = data_[ii][jj];
            }
            delete[] data_[ii];
        }
        delete[] data_;
        num_arrs_ *= 2;
        data_ = temp;
    }

    virtual void push_back(float val) {
        size_t remainder;
        if (size_ == cap_ * (data_idx_ + 1)) {
            if (data_idx_ == num_arrs_) {
                resize_();
            }
            data_idx_++;
            data_[data_idx_] = new float[cap_];
        }
        if (size_ < cap_) {
            remainder = size_;
        } else {
            remainder = size_ % cap_;
        }
        data_[data_idx_][remainder] = val;
        size_++;
    }

    FloatColumn* as_float() {
        return this;
    }

    /** Return the type of this column but ~virtual~ **/
    char useful_get_type() {
        return 'F';
    }

    /** Return the number of elements in this column **/
    size_t size() {
        return size_;
    }

    /** Visitor accept method **/
    void accept(Fielder visitor, size_t idx) {
        visitor.accept(get(idx));
    }
};

/*************************************************************************
 * StringColumn::
 * Holds string pointers. The strings are external.  Nullptr is a valid
 * value.
 */
class StringColumn : public Column {
  public:
    String*** data_;
    size_t size_;
    size_t cap_;
    size_t num_arrs_;
    size_t data_idx_;

    StringColumn() {
        cap_ = 4096;
        num_arrs_ = 32;
        data_ = new String**[num_arrs_];
        data_[0] = new String*[cap_];
        data_idx_ = 0;
        size_ = 0;
    }

    StringColumn(int n, ...) : StringColumn() {
        va_list args;
        va_start(args, n);
        for (int i = 0; i < n; i++) {
            String* s = va_arg(args, String*);
            push_back(s);
        }
        va_end(args);
        size_ = n;
    }

    String* get(size_t idx) {
        sys.exit_if_not(idx < size_, "Index out of bounds");
        size_t remainder = idx % cap_;
        return data_[data_idx_][remainder];
    }


    void set(size_t idx, String* val) {
        sys.exit_if_not(idx < size_, "Index out of bounds");
        // indexing logic
        size_t quotent = idx / cap_;
        size_t remainder = idx % cap_;
        data_[quotent][remainder] = val;
    }

    void resize_() {
        String*** temp = new String**[num_arrs_ * 2];
        for (int ii = 0; ii < num_arrs_; ii++) {
            temp[ii] = new String*[cap_];
            for (int jj = 0; jj < cap_; jj++) {
                temp[ii][jj] = data_[ii][jj];
            }
            delete[] data_[ii];
        }
        delete[] data_;
        num_arrs_ *= 2;
        data_ = temp;
    }

    virtual void push_back(String* val) {
        size_t remainder = 0;
        if (size_ == cap_ * (data_idx_ + 1)) {
            if (data_idx_ == num_arrs_) {
                resize_();
            }
            data_idx_++;
            data_[data_idx_] = new String*[cap_];
        }
        if (size_ < cap_) {
            remainder = size_;
        } else {
            remainder = size_ % cap_;
        }
        data_[data_idx_][remainder] = val;
        size_++;
    }

    StringColumn* as_string() {
        return this;
    }

    /** Return the type of this column but ~virtual~ **/
    char useful_get_type() {
        return 'S';
    }

    /** Return the number of elements in this column **/
    size_t size() {
        return size_;
    }

    /** Visitor accept method **/
    void accept(Fielder visitor, size_t idx) {
        visitor.accept(get(idx));
    }
};
