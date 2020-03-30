#pragma once
#include "object.h"
#include "string.h"

class Array : public Object {
    public:
        int size_;
        int capacity_; 
        Object** data_;

        // array constructor allocates a list of size 1 of Objects
        Array() {
            capacity_ = 1;
            data_ = new Object*[1];
            size_ = 0;
        }

        // array constructor allocates a list of size of Objects
        Array(size_t size) {
            size_ = size;
            capacity_ = size;
            data_ = new Object*[size];
        }

        // virtual destructor
        virtual ~Array() {
            delete[] data_;
        }

        // doubles the capacity of the data
        virtual void resize_() {
            capacity_ = capacity_ * 2;
            Object** new_data = new Object*[capacity_];
            for (int ii = 0; ii < size_; ii++) {
                new_data[ii] = data_[ii];
            }
            delete[] data_;
            data_ = new_data;
        }

        // returns the element at the given index
        virtual Object* get(int index) {
            if (index >= 0 && index < size_) {
                return data_[index];
            }
            else {
                return NULL;
            }
        }

        // sets the given index of this array to the given element
        virtual void set(int index, Object* element) {
            if (index >= 0 && index <= size_) {
                if (size_ == capacity_) {
                    resize_();
                }
                data_[index] = element;
                size_++;
            }
        }
        
        // adds the given element to the back of the array
        virtual void push_back(Object* element) {
            if (capacity_ == size_) {
                resize_();
            }
            data_[size_] = element;
            size_++;
        }

        // swaps the values at the given indices 
        virtual void swap(int index1, int index2) {
            Object* temp = data_[index1];
            data_[index1] = data_[index2];
            data_[index2] = temp;
        }

        // removes value at given index, moves rest of array down after
        // returns value at given index
        virtual Object* remove(int index) {
            if (index < 0 || index >= size_) return NULL;
            Object* temp = data_[index];
            for (int ii = index + 1; ii < size_; ii++) {
                data_[ii-1] = data_[ii];
            }
            size_--;
            return temp;
        }

        // returns the size of this array
        virtual size_t size() {
            return size_;
        }
};

class StrArray : public Array {
    public:
        String** data_;

        // String array constructor allocates a list of size 1 of Strings
        StrArray() {
            data_ = new String*[1];
            size_ = 0;
            capacity_ = 1;
        }

        // destructor
        ~StrArray() {
            delete[] data_;
        }

        // doubles the capacity of the data
        void resize_() {
            capacity_ = capacity_ * 2;
            String** new_data = new String*[capacity_];
            for (int ii = 0; ii < size_; ii++) {
                new_data[ii] = data_[ii];
            }
            delete[] data_;
            data_ = new_data;
        }

        // returns the String at the given index
        String* get(int index) {
            if (index >= 0 && index < size_) {
                return data_[index];
            }
            else {
                return NULL;
            }
        }

        // sets the value at the given index to the given string
        void set(int index, String* element) {
            if (index >= 0 && index <= size_) {
                if (size_ == capacity_) {
                    resize_();
                }
                data_[index] = element;
                size_++;
            }
        }

        // adds the given string to the back of the array
        void push_back(String* element) {
            if (capacity_ == size_) {
                resize_();
            }
            data_[size_] = element;
            size_++;
        }

        // removes the string at the given index, push the rest of the array down
        // returns the string at the given index
        String* remove(int index) {
            String* temp = data_[index];
            for (int ii = index + 1; ii < size_; ii++) {
                data_[ii-1] = data_[ii];
            }
            size_--;
            return temp;
        }
};

class IntArray : public Object {
    public:
        int size_;
        int capacity_; 
        int* data_;

        // int array constructor allocates a list of size 1 of ints
        IntArray() {
            data_ = new int[1];
            size_ = 0;
            capacity_ = 1;
        }

        // deconstructor
        ~IntArray() {
            delete[] data_;
        }

        // doubles the capacity of this array
        void resize_() {
            capacity_ = capacity_ * 2;
            int* new_data = new int[capacity_];
            for (int ii = 0; ii < size_; ii++) {
                new_data[ii] = data_[ii];
            }
            delete[] data_;
            data_ = new_data;
        }

        // returns the int at the given index
        int get(int index) {
            if (index >= 0 && index < size_) {
                return data_[index];
            }
            return -1;
        }

        // sets the value at the given index to the given int
        void set(int index, int element) {
            if (index >= 0 && index <= size_) {
                if (size_ == capacity_) {
                    resize_();
                }
                data_[index] = element;
                size_++;
            }
        }

        // adds the given int to the back of the array
        void push_back(int element) {
            if (capacity_ == size_) {
                resize_();
            }
            data_[size_] = element;
            size_++;
        }

        // swaps the int at the first given index with the second given index
        void swap(int index1, int index2) {
            int temp = data_[index1];
            data_[index1] = data_[index2];
            data_[index2] = temp;
        }

        // removes int at given index, pushes the rest of the array down
        // returns the int at the given index
        int remove(int index) {
            int temp = data_[index];
            for (int ii = index + 1; ii < size_; ii++) {
                data_[ii-1] = data_[ii];
            }
            size_--;
            return temp;
        }

        // returns the size of the array
        size_t size() {
            return size_;
        }
};

class FloatArray : public Object {
    public:
        int size_;
        int capacity_; 
        float* data_;

        // float array constructor allocates a list of size 1 of floats
        FloatArray() {
            data_ = new float[1];
            size_ = 0;
            capacity_ = 1;
        }

        // deconstructor
        ~FloatArray() {
            delete[] data_;
        }

        // doubles the capacity
        void resize_() {
            capacity_ = capacity_ * 2;
            float* new_data = new float[capacity_];
            for (int ii = 0; ii < size_; ii++) {
                new_data[ii] = data_[ii];
            }
            delete[] data_;
            data_ = new_data;
        }

        // returns the float at the given index
        float get(int index) {
            if (index >= 0 && index < size_) {
                return data_[index];
            }
        }
        
        // sets the value at the given index to the given float
        void set(int index, float element) {
            if (index >= 0 && index <= size_) {
                if (size_ == capacity_) {
                    resize_();
                }
                data_[index] = element;
                size_++;
            }
        }

        // adds the given float to the end of the array
        void push_back(float element) {
            if (capacity_ == size_) {
                resize_();
            }
            data_[size_] = element;
            size_++;
        }

        // swaps the value at the first given index with the value at the second given index
        void swap(int index1, int index2) {
            int temp = data_[index1];
            data_[index1] = data_[index2];
            data_[index2] = temp;
        }

        // removes the float at the given index and returns it
        // pushes the rest of the array down
        float remove(int index) {
            float temp = data_[index];
            for (int ii = index + 1; ii < size_; ii++) {
                data_[ii-1] = data_[ii];
            }
            size_--;
            return temp;
        }

        // returns the size of the array
        size_t size() {
            return size_;
        }
};

class BoolArray : public Object {
    public:
        int size_;
        int capacity_; 
        bool* data_;

        // bool array constructor allocates a list of size 1 of bools
        BoolArray() {
            data_ = new bool[1];
            size_ = 0;
            capacity_ = 1;
        }

        // deconstructor
        ~BoolArray() {
            delete[] data_;
        }

        // doubles the capacity
        void resize_() {
            capacity_ = capacity_ * 2;
            bool* new_data = new bool[capacity_];
            for (int ii = 0; ii < size_; ii++) {
                new_data[ii] = data_[ii];
            }
            delete[] data_;
            data_ = new_data;
        }

        // returns the bool at the given index
        bool get(int index) {
            if (index >= 0 && index < size_) {
                return data_[index];
            }
        }

        // sets the value of the given index to the given bool
        void set(int index, bool element) {
            if (index >= 0 && index <= size_) {
                if (size_ == capacity_) {
                    resize_();
                }
                data_[index] = element;
                size_++;
            }
        }

        // adds the given bool to the end of the array
        void push_back(bool element) {
            if (capacity_ == size_) {
                resize_();
            }
            data_[size_] = element;
            size_++;
        }

        // swaps the bool at the first given index with the bool at the second given index
        void swap(int index1, int index2) {
            int temp = data_[index1];
            data_[index1] = data_[index2];
            data_[index2] = temp;
        }

        // removes and returns the bool at the given index
        // pushes the rest of the array down
        bool remove(int index) {
            bool temp = data_[index];
            for (int ii = index + 1; ii < size_; ii++) {
                data_[ii-1] = data_[ii];
            }
            size_--;
            return temp;
        }

        // returns the size of the array
        size_t size() {
            return size_;
        }
};