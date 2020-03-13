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

/****************************************************************************
 * DataFrame::
 *
 * A DataFrame is table composed of columns of equal length. Each column
 * holds values of the same type (I, S, B, F). A dataframe has a schema that
 * describes it.
 */
class ModifiedDataFrame : public Object {

  public:
    Schema* schema_;
    Column** data_;
    size_t columns_; // number of columns
    size_t col_cap_; // max number of columns df can hold before reallocation


    /** Create a data frame with the same columns as the given df but no rows */
    ModifiedDataFrame(ModifiedDataFrame& df) {
        Column** temp = new Column*[df.columns_];
        data_ = temp;
        columns_ = df.columns_;
        schema_ = df.schema_;
        col_cap_ = df.columns_;
    }

    /** Create a data frame from a schema and columns. Results are undefined if
     * the columns do not match the schema. */
    ModifiedDataFrame(Schema& schema) {
        schema_ = new Schema(schema);
        columns_ = schema.cols_;
        data_ = new Column*[schema.cols_];
        col_cap_ = schema.cols_;
    }

    /** Returns the dataframe's schema. Modifying the schema after a dataframe
     * has been created in undefined. */
    Schema& get_schema() {
        return *schema_;
    }

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
        delete[] temp;
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
        for (int ii = 0; ii < columns_; ii++) {
            char type = row.col_type(idx);
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
        exit_if_not(row.size_ == columns_, "Row doesn't match schema");
        for (int ii = 0; ii < row.size_; ii++) {
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
        schema_->add_row(nullptr);
    }

    /** The number of rows in the dataframe. */
    size_t nrows() {
        return schema_->rows_;
    }

    /** The number of columns in the dataframe.*/
    size_t ncols() {
        return schema_->cols_;
    }

    /** Visit rows in order */
    void map(Rower& r) {
        Row& rtemp = *(new Row(*schema_));
        for (int ii = 0; ii < schema_->rows_; ii++) {
            fill_row(ii, rtemp);
            r.accept(rtemp);
        }
    }


    void pmap_help(Rower& r, size_t start, size_t end, std::mutex mtx) {
        Row* rtemp = new Row(*schema_);
        for (int ii = start; ii < end; ii++) {
            fill_row(ii, *rtemp);
            mtx.lock();
            r.accept(*rtemp);
            mtx.unlock();
        }
    }

    /** This method clones the Rower and executes the map in parallel. Join is
     * used at the end to merge the results. */
    void pmap(Rower& r) {

        size_t num_threads = 16;
        std::thread* pool[num_threads];
        Rower rowers[num_threads];

        for (int ii = 0; ii < num_threads; ii++) {
            pool[ii] = new std::thread(pmap_help);
            rowers[ii] = r.clone();
            std::mutex mtx;

            int inc = nrows() / num_threads;

            size_t start = inc * ii;
            size_t end = inc * (ii + 1);

            pool[ii]->start(rowers[ii], start, end, mtx);
        }

        for (int ii = 0; ii < 15; ii++) {
        }
        pool[ii].join->join();
    }


    /** Create a new dataframe, constructed from rows for which the given Rower
     * returned true from its accept method. */
    DataFrame* filter(Rower& r) {
        DataFrame* temp = new DataFrame(*schema_);
        Row* rtemp = new Row(*schema_);
        for (int ii = 0; ii < schema_->rows_; ii++) {
            fill_row(ii, *rtemp);
            if (r.accept(*rtemp)) {
                temp->add_row(*rtemp);
            }
        }
        delete rtemp;
        return temp;
    }

    /** Print the dataframe in SoR format to standard output. */
    void print() {
        /*
        Fielder* f = new PrintFielder(this);
        for (int ii = 0; ii < schema_->rows_; ii++) {
            f->start(ii);
        }
        */
    }
};