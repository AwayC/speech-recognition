#ifndef __COMPLEX_H
#define __COMPLEX_H

#include <cmath>
#include <cassert>

class Complex {
public: 
    float real, imag;
    
    Complex(float real = 0.0, float imag = 0.0) : real(real), imag(imag) {

    }
    ~Complex() { 

    }

    float abs() const {
        float ret = sqrtl(real * real + imag * imag) ;
        return ret; 
    }

    float deno() const {
        float ret = real * real + imag * imag;
        return ret; 
    }
    Complex operator + (const Complex c) const { 
        return Complex(real + c.real, imag + c.imag); 
    }
    Complex operator += (const Complex c) { 
        real += c.real; 
        imag += c.imag; 
        return *this; 
    }

    Complex operator - (const Complex c) const { 
        return Complex(real - c.real, imag - c.imag); 
    }

    Complex operator -= (const Complex c) { 
        real -= c.real; 
        imag -= c.imag; 
        return *this; 
    }
    
    Complex operator * (const Complex c) const { 
        return Complex(real * c.real - imag * c.imag , real * c.imag + c.real * imag); 
    }

    Complex operator *= (const Complex c) { 
        *this = *this * c; 
        return *this; 
    }

    Complex operator / (const Complex c) const { 
        float denominator = c.deno();
        assert(fabs(denominator) > 1e-10); 
        return (Complex) { 
            (real * c.real + imag * c.imag) / denominator, 
            (imag * c.real - real * c.imag) / denominator
        }; 
    }

    Complex operator /= (const Complex c) { 
        *this = *this / c; 
        return *this; 
    }

    Complex conj() { 
        return Complex(this->real, -this->imag); 
    }
}; 

#endif