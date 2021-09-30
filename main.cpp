#include <iostream>
#include <algorithm>

class square_matrix {
    int size;
    int ** array;

    void checkSize(int that_size) const {
        if (size != that_size) {
            std::cout << "Error, not matching sizes";
            exit(-1);
        }
    }

    void transpose() const {
        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                std::swap(array[i][j], array[j][i]);
            }
        }
    }

    void createMinor(const square_matrix& minor, int row, int column) const {
        for (int i = 0, k = 0; k < size - 1; i++, k++) {
            if (i == row) {
                i++;
            }
            for (int j = 0, l = 0; l < size - 1; j++, l++) {
                if (j == column) {
                    j++;
                }
                minor.array[k][l] = array[i][j];
            }
        }
    }

public:

    square_matrix(int n, const int * a):
            size(n),
            array(new int*[n]) {

        for (int i = 0; i < n; i++) {
            array[i] = new int[n];
            for (int j = 0; j < n; j++) {
                if (j == i) {
                    array[i][i] = a[i];
                    continue;
                }
                array[i][j] = 0;
            }
        }
    }

    square_matrix() : square_matrix(0) {}

    explicit square_matrix(int n) :
            size(n),
            array(new int*[n]) {

        for (int i = 0; i < n; i++) {
            array[i] = new int[n];
            for (int j = 0; j < n; j++) {
                if (j == i) {
                    array[i][i] = 1;
                    continue;
                }
                array[i][j] = 0;
            }
        }
    }

    ~square_matrix() {
        for (int i = 0; i < size; i++) {
            delete[] array[i];
        }
        delete[] array;
    }

    square_matrix(const square_matrix& that) :
            size(that.size),
            array(new int*[size]) {

        for (int i = 0; i < size; i++) {
            array[i] = new int[that.size];
            for (int j = 0; j < size; j++) {
                array[i][j] = that.array[i][j];
            }
        }
    }

    friend void swap(square_matrix& a, square_matrix& b) {
        using std::swap;
        swap(a.size, b.size);
        swap(a.array, b.array);
    }

    square_matrix& operator= (square_matrix that) {
        swap(*this, that);
        return *this;
    }

    square_matrix operator+(const square_matrix& that) const {
        checkSize(that.size);
        square_matrix result(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                result.array[i][j] = array[i][j] + that.array[i][j];
            }
        }
        return result;
    }

    square_matrix operator- (const square_matrix& that) const {
        checkSize(that.size);
        square_matrix result(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                result.array[i][j] = array[i][j] - that.array[i][j];
            }
        }
        return result;
    }

    square_matrix operator*(const square_matrix& that) const {
        checkSize(that.size);
        square_matrix result(size);
        for (int i = 0; i < size; i++) {
            result.array[i][i] = 0;
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                for (int k = 0; k < size; k++) {
                    result.array[i][j] += array[i][k] * that.array[k][j];
                }
            }
        }
        return result;
    }

    bool operator==(const square_matrix& that) const {
        if (size != that.size) {
            return false;
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (array[i][j] != that.array[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const square_matrix& that) const {
        if (*this == that) {
            return false;
        }
        return true;
    }

    square_matrix operator~() const {
        transpose();
        return *this;
    }

    friend std::istream& operator>> (std::istream& is, const square_matrix& that)
    {
        for (int i = 0; i < that.size; i++) {
            for (int j = 0; j < that.size; j++) {
                is >> that.array[i][j];
            }
        }
        return is;
    }

    friend std::ostream& operator<< (std::ostream& os, const square_matrix& that)
    {
        for (int i = 0; i < that.size; i++) {
            for (int j = 0; j < that.size; j++) {
                os << that.array[i][j] << ' ';
            }
            os << std::endl;
        }
        return os;
    }

    square_matrix operator() (int row, int column) const {
        square_matrix minor(size - 1);
        createMinor(minor, row, column);
        return minor;
    }


};

int main() {
    int N, k;
    std::cin >> N >> k;
    int * a = new int[N];
    for (int i = 0; i < N; i++) {
        a[i] = k;
    }
    square_matrix A(N), B(N), C(N), D(N), K(N, a);
    delete[] a;
    std::cin >> A >> B >> C >> D;
    std::cout << (A + B * ~C + K) * ~D;
    return 0;
}