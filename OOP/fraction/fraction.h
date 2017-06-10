#ifndef FRACTION_H
#define FRACTION_H

#include <ostream>
#include <istream>

class Fraction
{
private:
    //Two private elements of a fraction
    int Numerator;
    int Denominator;
    //LiteralDenominator for leteral operator
    class LiteralDenominator;
    //Leteral operator for _f
    friend LiteralDenominator operator ""_f(unsigned long long int x);
public:
    // return the numverator and denominator
    inline const int &numerator() const {return Numerator;}
    inline const int &denominator() const {return Denominator;}
    // Default 0/1
    Fraction(int numerator = 0, int denominator = 1);
    //operators overlaod for some ordinary usages.
    Fraction operator+(const int &num) const;
    Fraction operator+(const Fraction &fraction) const;
    Fraction operator-(const int &num) const;
    Fraction operator-(const Fraction &fraction) const;
    Fraction operator-() const;
    Fraction operator*(const int &num) const;
    Fraction operator*(const Fraction &fraction) const;
    Fraction operator/(const int &num) const;
    Fraction operator/(const Fraction &fraction) const;
    Fraction operator~() const;
    //Comparison operators
    bool operator==(const Fraction &fraction) const;
    bool operator==(const int &num) const;
    bool operator!=(const Fraction &fraction) const;
    bool operator!=(const int &num) const;
    bool operator<(const Fraction &fraction) const;
    bool operator<(const int &num) const;
    bool operator>(const Fraction &fraction) const;
    bool operator>(const int &num) const;
    bool operator<=(const Fraction &fraction) const;
    bool operator<=(const int &num) const;
    bool operator>=(const Fraction &fraction) const;
    bool operator>=(const int &num) const;
    //convertion operators
    operator double() const;
    operator bool() const;
    //input and ouput operators
    friend std::ostream &operator<<(std::ostream &out,
                                    const Fraction &fraction);
    friend std::istream &operator>>(std::istream &in,
                                    Fraction &fraction);
    //For literal overload.
    friend Fraction operator/(int const numerator,const LiteralDenominator &denominator);
private:
    //reducte the function
    void Reducte();
    //get the binary gcd of two positive intergers.
    int Gcd(int a, int b) const;
};

class Fraction::LiteralDenominator
{
    int Denominator;
    constexpr LiteralDenominator(int denominator)
        :Denominator(denominator){}
    //leteral operator overload
    friend LiteralDenominator operator ""_f(unsigned long long int x);
    friend Fraction operator/(int const numerator,const LiteralDenominator &denominator);
};

inline Fraction::LiteralDenominator operator ""_f(unsigned long long int x)
{
    return Fraction::LiteralDenominator((int)x);
}


inline Fraction operator/(const int  numerator,
                                    const Fraction::LiteralDenominator &denominator)
{
    return Fraction(numerator,denominator.Denominator);
}


#endif // FRACTION_H
