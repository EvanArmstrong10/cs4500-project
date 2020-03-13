#include "dataframe.h"

class ExpensiveSquare : public Rower {
  public:
    DataFrame& df_;
    ExpensiveSquare(DataFrame& df) : df_(df) {
    }

    /** Accepts a row and expensively squares each field */
    bool accept(Row& r) {
        for (int ii = 0; ii < r.size_; ii++) {
            int s = r.get_int(ii);
            int sq = 0;
            for (int jj = 0; jj < s; jj++) {
                sq = sq + s;
            }
            df_.set(ii, r.get_idx(), sq);
        }
        return true;
    }

    /** Deletes the given rower */
    void join_delete(Rower* other) {
        delete other;
    }

    /** Clones this Rower */
    Object* clone() {
        return new ExpensiveSquare(df_);
    }
};

class CheapSquare : public Rower {
  public:
    DataFrame& df_;
    CheapSquare(DataFrame& df) : df_(df) {
    }

    /** Accepts a row and squares each field */
    bool accept(Row& r) {
        for (int ii = 0; ii < r.width(); ii++) {
            int s = r.get_int(ii);
            int sq = s * s;
            df_.set(ii, r.get_idx(), sq);
        }
        return true;
    }

    /** Deletes the given rower */
    void join_delete(Rower* other) {
        delete other;
    }

    /** Clones this Rower */
    Object* clone() {
        return new CheapSquare(df_);
    }
};
