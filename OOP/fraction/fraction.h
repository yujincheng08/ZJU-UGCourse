#ifndef FRACTION_H
#define FRACTION_H

#include <ostream>
#include <istream>

class Fraction
{
private:
    int Numerator;
    int Denominator;
    class LiteralDenominator;
    friend LiteralDenominator operator ""_f(unsigned long long int x);
public:
    inline const int &numerator() const {return Numerator;}
    inline const int &denominator() const {return Denominator;}
    Fraction(int numerator = 0, int denominator = 1);
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
    operator double() const;

    friend std::ostream &operator<<(std::ostream &out,
                                    const Fraction &fraction);
    friend std::istream &operator>>(std::istream &in,
                                    Fraction &fraction);
    friend Fraction operator/(int const numerator,const LiteralDenominator &denominator);
private:
    void Reducte();
    int Gcd(int a, int b) const;
};

#define Exoperator(type, op, newop) \
    inline Fraction operator op (const type &num, const Fraction &fraction)\
    { return fraction newop num; }
Exoperator(int,+,+)
Exoperator(int,-,-)
Exoperator(int,*,*)
Exoperator(int,/,/)
Exoperator(int,==,==)
Exoperator(int,!=,!=)
Exoperator(int,<,>)
Exoperator(int,>,<)
Exoperator(int,<=,>=)
Exoperator(int,>=,<=)
#undef Exoperator

class Fraction::LiteralDenominator
{
    int Denominator;
    constexpr LiteralDenominator(int denominator)
        :Denominator(denominator){}
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
