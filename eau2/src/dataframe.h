// lang::CwC

#pragma once
#include "column.h"
#include "helper.h"
#include "object.h"
#include "row.h"
#include "rower.h"
#include "schema.h"
#include "string.h"
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include "kv_store/kv.h"


/****************************************************************************
 * DataFrame::
 *
 * A DataFrame is table composed of columns of equal length. Each column
 * holds values of the same type (I, S, B, F). A dataframe has a schema that
 * describes it.
 */
class DataFrame : public Object {

  public:
    Schema* schema_;
    Column** data_;
    size_t columns_; // number of columns
    size_t col_cap_; // max number of columns df can hold before reallocation

    DataFrame() {
        data_ = new Column*[0];
        columns_ = 0;
        col_cap_ = 2;
        schema_ = new Schema();
    }


    /** Create a data frame with the same columns as the given df but no rows */
    DataFrame(DataFrame& df) {
        Column** temp = new Column*[df.columns_];
        data_ = temp;
        columns_ = df.columns_;
        schema_ = df.schema_;
        col_cap_ = df.columns_;
    }

    /** Create a data frame from a schema and columns. Results are undefined if
     * the columns do not match the schema. */
    DataFrame(Schema& schema) {
        schema_ = new Schema(schema);
        columns_ = schema.cols_;
        if (schema.cols_ == 0)
            col_cap_ = 2;
        else
            col_cap_ = schema.cols_;
        data_ = new Column*[columns_];
        for (int ii = 0; ii < columns_; ii++) {
            char type = schema.col_type(ii);
            switch (type) {
            case 'I':
                data_[ii] = new IntColumn();
                break;
            case 'B':
                data_[ii] = new BoolColumn();
                break;
            case 'F':
                data_[ii] = new FloatColumn();
                break;
            case 'S':
                data_[ii] = new StringColumn();
                break;
            default:
                break;
            }
        }
    }

    ~DataFrame() {
        delete[] data_;
    }

    /** Returns the dataframe's schema. Modifying the schema after a dataframe
     * has been created in undefined. */
    Schema& get_schema() {
        return *schema_;
    }

    /** Checks if the data structure is at capacity. */
    bool at_capacity() {
        return col_cap_ <= columns_;
    }

    void double_capacity() {
        col_cap_ *= 2;
        Column** temp = new Column*[col_cap_];
        for (size_t x = 0; x < columns_; x++) {
            temp[x] = data_[x];
        }
        delete[] data_;
        data_ = temp;
    }

    /** Adds a column this dataframe, updates the schema, the new column
     * is external, and appears as the last column of the dataframe, the
     * name is optional and external. A nullptr colum is undefined. */
    void add_column(Column* col, String* name) {
        if (at_capacity()) {
            double_capacity();
        }
        exit_if_not(col->size() == data_[0]->size(), "Invalid number of rows");
        data_[columns_] = col;
        char x = col->get_type();

        schema_->add_column(x, name);
        columns_++;
    }

    static DataFrame* fromArray(Key key, KV kv, size_t size, Column* vals) {
        DataFrame* df = new DataFrame();
        df->add_column(vals, nullptr);
        return df;
    }

    static DataFrame* fromArray(Key key, KV kv, size_t size, float* vals) {
        DataFrame* df = new DataFrame();
        FloatColumn* fcol = new FloatColumn();
        for (int i = 0; i < size; i++) {
            fc->push_back(vals[i]);
        }
        df->add_column(fc, nullptr);
        return df;
    }

    static DataFrame* fromArray(Key key, KV kv, size_t size, int* vals) {
        DataFrame* df = new DataFrame();
        IntColumn* icol = new IntColumn();
        for (int i = 0; i < size; i++) {
            icol->push_back(vals[i]);
        }
        df->add_column(icol, nullptr);
        return df;
    }

    static DataFrame* fromArray(Key key, KV kv, size_t size, bool* vals) {
        DataFrame* df = new DataFrame();
        BoolColumn* bcol = new BoolColumn();
        for (int i = 0; i < size; i++) {
            bcol->push_back(vals[i]);
        }
        df->add_column(bcol, nullptr);
        return df;
    }

    static DataFrame* fromArray(Key key, KV kv, size_t size, String** vals) {
        DataFrame* df = new DataFrame();
        StringColumn* scol = new StringColumn();
        for (int i = 0; i < size; i++) {
            scol->push_back(vals[i]);
        }
        df->add_column(scol, nullptr);
        return df;
    }

    /** Return the value at the given column and row. Accessing rows or
     *  columns out of bounds, or request the wrong type is undefined.*/
    int get_int(size_t col, size_t row) {
        return data_[col]->as_int()->get(row);
    }
    bool get_bool(size_t col, size_t row) {
        return data_[col]->as_bool()->get(row);
    }
    float get_float(size_t col, size_t row) {
        return data_[col]->as_float()->get(row);
    }
    String* get_string(size_t col, size_t row) {
        return data_[col]->as_string()->get(row);
    }

    /** Return the offset of the given column name or -1 if no such col. */
    int get_col(String& col) {
        return schema_->col_idx(col.c_str());
    }

    /** Return the offset of the given row name or -1 if no such row. */
    int get_row(String& col) {
        return schema_->row_idx(col.c_str());
    }

    /** Set the value at the given column and row to the given value.
     * If the column is not  of the right type or the indices are out of
     * bound, the result is undefined. */
    void set(size_t col, size_t row, int val) {
        IntColumn* x = data_[col]->as_int();
        x->set(row, val);
    }
    void set(size_t col, size_t row, bool val) {
        BoolColumn* x = data_[col]->as_bool();
        x->set(row, val);
    }
    void set(size_t col, size_t row, float val) {
        FloatColumn* x = data_[col]->as_float();
        x->set(row, val);
    }
    void set(size_t col, size_t row, String* val) {
        StringColumn* x = data_[col]->as_string();
        x->set(row, val);
    }


    /** Set the fields of the given row object with values from the columns at
     * the given offset.  If the row is not form the same schema as the
     * dataframe, results are undefined.
     */
    void fill_row(size_t idx, Row& row) {
        exit_if_not(row.size_ == columns_, "Row doesn't match the schema");
        for (int ii = 0; ii < row.width(); ii++) {
            char type = get_schema().col_type(ii);
            row.set_idx(idx);
            switch (type) {
            case 'I': {
                IntColumn* col = data_[ii]->as_int();
                row.set(ii, col->get(idx));
                break;
            }
            case 'B': {
                BoolColumn* col = data_[ii]->as_bool();
                row.set(ii, col->get(idx));
                break;
            }
            case 'F': {
                FloatColumn* col = data_[ii]->as_float();
                row.set(ii, col->get(idx));
                break;
            }
            case 'S': {
                StringColumn* col = data_[ii]->as_string();
                row.set(ii, col->get(idx));
                break;
            }
            default:
                break;
            }
        }
    }

    /** Add a row at the end of this dataframe. The row is expected to have
     *  the right schema and be filled with values, otherwise undedined.  */
    void add_row(Row& row) {
        if (row.width() != columns_) {
            puts("no no no");
            return;
        }
        for (int ii = 0; ii < row.width(); ii++) {
            char type = row.col_type(ii);
            switch (type) {
            case 'I': {
                IntColumn* i = data_[ii]->as_int();
                i->push_back(row.get_int(ii));
                break;
            }
            case 'B': {
                BoolColumn* b = data_[ii]->as_bool();
                b->push_back(row.get_bool(ii));
                break;
            }
            case 'F': {
                FloatColumn* f = data_[ii]->as_float();
                f->push_back(row.get_float(ii));
                break;
            }
            case 'S': {
                StringColumn* s = data_[ii]->as_string();
                s->push_back(row.get_string(ii));
                break;
            }
            default:
                break;
            }
        }
        schema_->add_row(nullptr); // change to blank string
    }

    /** The number of rows in the dataframe. */
    size_t nrows() {
        return schema_->length();
    }

    /** The number of columns in the dataframe.*/
    size_t ncols() {
        return schema_->width();
    }

    /** Visit rows in order */
    void map(Rower& r) {
        Row row(*schema_);
        for (int ii = 0; ii < schema_->rows_; ii++) {
            fill_row(ii, row);
            r.accept(row);
        }
    }

    /** Create a new dataframe, constructed from rows for which the given Rower
     * returned true from its accept method. */
    DataFrame* filter(Rower& r) {
        DataFrame* df = new DataFrame(*schema_);
        Row row(*schema_);
        for (int ii = 0; ii < schema_->rows_; ii++) {
            fill_row(ii, row);
            if (r.accept(row)) {
                df->add_row(row);
            }
        }
        return df;
    }

    /** Print the dataframe in SoR format to standard output. */
    void print() {
        Row r(*schema_);
        for (int ii = 0; ii < schema_->rows_; ii++) {
            fill_row(ii, r);
            r.print();
        }
    }
};


class ModifiedDataFrame : public DataFrame {

  public:
    // fields used for threading
    size_t finished_thread_counter_;
    std::condition_variable_any cv;
    std::mutex cv_mtx;

    // constant thread count
    size_t NUM_THREADS_ = 8;


    /** Create a data frame with the same columns as the given df but no rows */
    ModifiedDataFrame(DataFrame& df) : DataFrame(df) {
        finished_thread_counter_ = 0;
    }

    /** Create a data frame from a schema and columns. Results are undefined if
     * the columns do not match the schema. */
    ModifiedDataFrame(Schema& schema) : DataFrame(schema) {
        finished_thread_counter_ = 0;
    }

    /** Visit rows starting from index start and ending at index end */
    void thread_helper(Rower* r, size_t start, size_t end) {
        Row row(*schema_);
        for (size_t ii = start; ii < end; ii++) {
            fill_row(ii, row);
            r->accept(row);
        }

        // this thread has performed all necessary work, increment counter
        finished_thread_counter_++;

        // if all threads have completed, wake main thread
        if (finished_thread_counter_ == NUM_THREADS_) cv.notify_all();
    }

    /** Visit rows in order */
    void pmap(Rower& r) {
        std::thread* threads[NUM_THREADS_];
        Rower** rowers = new Rower*[NUM_THREADS_];

        size_t increment = (schema_->rows_ / NUM_THREADS_) + 1;
        size_t start = 0;
        size_t end = increment;

        // loop for spawning threads
        for (int ii = 0; ii < NUM_THREADS_; ii++) {
            rowers[ii] = dynamic_cast<Rower*>(r.clone());

            // spawn thread into thread_helper
            threads[ii] = new std::thread(&ModifiedDataFrame::thread_helper, this, rowers[ii], start, end);

            start += increment;
            end += increment;
            if (end > schema_->rows_) end = schema_->rows_;
        }

        // wait for threads to perform work
        cv.wait(cv_mtx);

        // loop for clone and thread deletion
        for (int ii = 0; ii < NUM_THREADS_; ii++) {
            r.join_delete(rowers[ii]);
            threads[ii]->join();
        }
    }
};