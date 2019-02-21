#ifndef LONGNUMBER_H
#define LONGNUMBER_H

#include <vector>
#include <QDebug>

class LongNumber
{
    friend QDebug operator <<(QDebug debug, const LongNumber &number);
    friend LongNumber operator +(const LongNumber &first, const LongNumber &second);
    friend LongNumber operator *(const LongNumber &first, const LongNumber &second);
    friend LongNumber operator -(const LongNumber &first, const LongNumber &second);

public:
    LongNumber(const char *str);
    LongNumber(const std::vector<int> &data);
    LongNumber(int data);
    const char* toStr() const;
    int toInt() const;
    bool isZero() const;

    bool operator ==(const LongNumber &other) const;

private:
    int count() const;
    static std::vector<int> suma(const std::vector<int> &first, const std::vector<int> &second);
    void multiplyTen(int count);
    void initEmpty();
    void checkInvalidState(const char *where);
    static void getParameters(const LongNumber &first, const LongNumber &second, LongNumber &a,
                              LongNumber &b, LongNumber &c, LongNumber &d, int &n);

    // operators
    LongNumber& operator +(const LongNumber &number);
    LongNumber& operator *(const LongNumber &number);
    LongNumber& operator -(const LongNumber &number);

private:
    std::vector<int> _int;
};

QDebug operator <<(QDebug debug, const LongNumber &number);
LongNumber operator +(const LongNumber &first, const LongNumber &second);
LongNumber operator *(const LongNumber &first, const LongNumber &second);
LongNumber operator -(const LongNumber &first, const LongNumber &second);

#endif // LONGNUMBER_H
