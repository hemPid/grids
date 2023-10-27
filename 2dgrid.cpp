#include <iostream>
#include <cassert>

template <typename T>
class Grid final {
public:
    using size_type = unsigned;
private:
    T * data;
    size_type y_size, x_size;

    class Iterator {
    private:
        T * str;
    public:
        Iterator(T*str): str(str) {}
        T & operator[](size_type idx) {
            return str[idx];
        }
    };
public:
    Grid (T * data , size_type y_size , size_type x_size ) : data(data), y_size(y_size), x_size(x_size) {}

    Grid(Grid<T> const & obj) : data(obj.data),  x_size(obj.x_size), y_size(obj.y_size) {
        for (size_t i = 0; i != x_size*y_size; i++)
        {
            data[i] = obj.data[i];
        }
    }

    Grid(T const & t) : data(new T[1]), x_size(1), y_size(1) {
        data[0] = t;
    }

    Grid(size_type y_size, size_type x_size, T const & val) : x_size(x_size), y_size(y_size), data(new T[x_size*y_size]) {
        for (size_t i = 0; i < x_size*y_size; i++)
        {
            data[i] = val;
        }
    }

    Grid(size_type y_size, size_type x_size) : Grid(y_size,x_size,T()) {}

    Grid(Grid<T> && obj) {
        std::swap(data, obj.data);
        std::swap(x_size, obj.x_size);
        std::swap(y_size, obj.y_size);
    }

    Grid<T> & operator=(Grid<T> & obj) {
        if (data != nullptr)
        {
            delete [] data;
        }
        x_size = obj.x_size;
        y_size = obj.y_size;
        data = new T[x_size*y_size];
        for (size_t i = 0; i < x_size*y_size; i++)
        {
            data[i] = obj.data[i];
        }
        return *this;
    }

    Grid<T> & operator=(Grid<T> && obj) {
        std::swap(data, obj.data);
        std::swap(x_size, obj.x_size);
        std::swap(y_size, obj.y_size);
        return *this;
    }

    ~Grid() {
        delete [] data;
    }

    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];
    }

    T & operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];
    }

    Iterator operator[](size_type idx) {
        return Iterator(&data[idx*x_size]);
    }

    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }
};



int main(){
    Grid<float> g(3,2,0.0f);
    assert(3 == g.get_y_size());
    assert(2 == g.get_x_size());

    using gsize_t = Grid<int>::size_type;

    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx) {
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx) {
            assert(0.0f == g[y_idx][x_idx]);
        }
    }

    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx) {
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx) {
            g[y_idx][x_idx] = 1.0f;
        }
    }

    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx) {
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx) {
            assert(1.0f == g[y_idx][x_idx]);
        }
    }
    return 0;
}