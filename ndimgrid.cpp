#include <iostream>
#include <cassert>


//ndim grid

template <typename T, unsigned N>
class Grid
{
public:
    using size_type = unsigned;
private:
    Grid<T, N-1> * data;
    size_type dim_size;
public:
    Grid() : data(nullptr) {}

    template <typename ... Args> Grid(size_type dim_size, Args ... args) : data(new Grid<T, N-1>[dim_size]), dim_size(dim_size) {
        Grid<T, N-1> el(args...);
        for (size_t i = 0; i != dim_size; i++)
        {
            data[i] = el;
        }
    }

    Grid(Grid<T,N> const & obj): dim_size(obj.dim_size) {
        for (size_t i = 0; i != dim_size; i++)
        {
            data[i] = obj.data[i];
        }
    }

    Grid(Grid<T,N> && obj) {
        std::swap(data, obj.data);
        std::swap(dim_size, obj.dim_size);
    }

    Grid<T,N> & operator=(Grid<T,N> & obj) {
        if (data != nullptr)
        {
            delete [] data;
        }
        dim_size = obj.dim_size;
        data = new Grid<T, N-1>[dim_size];
        for (size_t i = 0; i != dim_size; i++)
        {
            data[i] = obj.data[i];
        }
        return *this;
    }

    Grid<T,N> & operator=(Grid<T,N> && obj) {
        std::swap(data, obj.data);
        std::swap(dim_size, obj.dim_size);
        return *this;
    }

    ~Grid() {
        if (data != nullptr)
        {
            delete [] data;
        }
    }

    template <typename ... Args> T operator()(size_type dim_idx, Args ... args) const {
        return data[dim_idx](args...);
    }

    template <typename ... Args> T & operator()(size_type dim_idx, Args ... args) {
        return data[dim_idx](args...);
    }

    Grid<T, N-1> & operator[](size_type idx) {
        return data[idx];
    }
};


//2d specialization

template <typename T>
class Grid<T,2> final {
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
    Grid() : data(nullptr), x_size(0), y_size(0) {}

    Grid (T * data , size_type y_size , size_type x_size ) : data(data), y_size(y_size), x_size(x_size) {}

    Grid(Grid<T,2> const & obj) : data(new T[x_size*y_size]), x_size(obj.x_size), y_size(obj.y_size) {
        for (size_t i = 0; i < x_size*y_size; i++)
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

    Grid(Grid<T,2> && obj) {
        std::swap(data, obj.data);
        std::swap(x_size, obj.x_size);
        std::swap(y_size, obj.y_size);
    }

    Grid<T,2> & operator=(Grid<T,2> & obj) {
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

    Grid<T,2> & operator=(Grid<T,2> && obj) {
        std::swap(data, obj.data);
        std::swap(x_size, obj.x_size);
        std::swap(y_size, obj.y_size);
        return *this;
    }

    ~Grid() {
        if (data != nullptr)
        {
            delete [] data;
        }
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
};

int main() {
    Grid<float, 4> const g4(2, 3, 4, 5, 1.0f);
    assert(1.0f == g4(1,1,1,1));
    return 0;
}