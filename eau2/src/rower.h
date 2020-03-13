// lang::CwC

#pragma once
#include "dataframe.h"
#include "object.h"
#include "row.h"

/*******************************************************************************
 *  Rower::
 *  An interface for iterating through each row of a data frame. The intent
 *  is that this class should subclassed and the accept() method be given
 *  a meaningful implementation. Rowers can be cloned for parallel execution.
 */
class Rower : public Object {
  public:
    /** This method is called once per row. The row object is on loan and
        should not be retained as it is likely going to be reused in the next
        call. The return value is used in filters to indicate that a row
        should be kept. */
    virtual bool accept(Row& r) {
        return true;
    }

    /** Once traversal of the data frame is complete the rowers that were
        split off will be joined.  There will be one join per split. The
        original object will be the last to be called join on. The join method
        is reponsible for cleaning up memory. */
    virtual void join_delete(Rower* other) {
    }
};

// Computing the taxes
class Taxes : public Rower {
  public:
    DataFrame* df_;
    Taxes(DataFrame* df) : df_(df) {
    }

    size_t salary = 0, rate = 1, isded = 2, ded = 3, taxes = 4; // these are for convenience

    bool accept(Row& r) {
        int tx = (int)r.get_int(salary) * r.get_float(rate);
        tx -= r.get_bool(isded) ? r.get_int(ded) : 0;
        df_->set(taxes, r.get_idx(), tx);
        return true;
    }
};