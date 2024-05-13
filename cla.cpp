#include <iostream>

template <typename T>
class Complex {
private:
    T real;
    T imag;

public:
    Complex(T r, T i) : real(r), imag(i) {}

    Complex<T> operator+(const Complex<T>& other) const {
        return Complex<T>(real + other.real, imag + other.imag);
    }

    Complex<T> operator-(const Complex<T>& other) const {
        return Complex<T>(real - other.real, imag - other.imag);
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex<T>& complex) {
        os << complex.real;
        if (complex.imag >= 0) {
            os << " + " << complex.imag << "i";
        } else {
            os << " - " << -complex.imag << "i";
        }
        return os;
    }
};

int main() {
    Complex<int> a(1, 2);
    Complex<int> b(3, 4);
    Complex<int> c = a + b;
    Complex<int> d = a - b;

    std::cout << "a: " << a << std::endl;
    std::cout << "b: " << b << std::endl;
    std::cout << "a + b: " << c << std::endl;
    std::cout << "a - b: " << d << std::endl;

    return 0;
}
