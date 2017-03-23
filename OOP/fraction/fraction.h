#ifndef FRACTION_H
#define FRACTION_H

#include <ostream>
#include <istream>

class Fraction
{
private:
    int Numerator;
    int Denominator;
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

#endif // FRACTION_H
