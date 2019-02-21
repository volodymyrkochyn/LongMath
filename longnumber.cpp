#include "longnumber.h"
#include "convert.h"
#include "cassert"

LongNumber::LongNumber(const char *str) :
    _int(toVector(str))
{
    initEmpty();
}

LongNumber::LongNumber(const std::vector<int> &data) :
    _int(data)
{
    initEmpty();
}

LongNumber::LongNumber(int data) :
    _int(toVector(data))
{
    initEmpty();
}

LongNumber &LongNumber::operator +(const LongNumber &number)
{
    _int = (_int.size() > number._int.size()) ? suma(_int, number._int) : suma(number._int, _int);
    checkInvalidState(__PRETTY_FUNCTION__);
    return *this;
}

LongNumber &LongNumber::operator *(const LongNumber &number)
{
    // X * Y = 10^n*ac+10^n/2*(ad+bc) + bd
    // (ad+bc) = (a + b)*(c + d) - ac - bd

    if (_int.size() == 1 && number._int.size() == 1)
    {
        int value = _int.at(0)*number._int.at(0);
        if (value < 10)
            _int = {value};
        else
            _int = {value/10, value%10};
    }
    else
    {
        LongNumber a{0};
        LongNumber b{0};
        LongNumber c{0};
        LongNumber d{0};
        int n = 0;
        getParameters(*this, number, a, b, c, d, n);

        LongNumber ac = qAsConst(a) * c;
        const LongNumber bd = qAsConst(b) * d;
        const LongNumber ab = qAsConst(a) + b;
        const LongNumber cd = qAsConst(c)+ d;
        LongNumber adbc = ab * cd;
        adbc = qAsConst(adbc) - ac;
        adbc = qAsConst(adbc)- bd;

        ac.multiplyTen(n);
        adbc.multiplyTen(n/2);
        *this = ac + adbc + bd;
    }
    checkInvalidState(__PRETTY_FUNCTION__);
    return *this;
}

/*!
 * \brief LongNumber::operator - \a number is never bigger than *this
 */
LongNumber &LongNumber::operator -(const LongNumber &number)
{
    if (_int.size() < number._int.size())
    {
        qDebug() << toStr() << number.toStr();
        assert (false);
    }
    int prev = 0;
    int back = _int.size() - 1;
    for (int sec = number._int.size() - 1; back >= 0 && sec >= 0;
         --back, --sec)
    {
        if (prev != 0)
            _int[back]--;

        prev = 0;
        int value = _int.at(back);
        if (value < number._int.at(sec))
        {
            prev++;
            value += 10;
        }
        _int[back] = value - number._int.at(sec);
    }

    if ((prev != 0) && (back >= 0))
        _int[back]--;

    // remove all 0 from the begining
    if (_int.size() > 1)
        while ((_int.front() == 0) && (_int.size() != 0))
            _int.erase(_int.begin());
    initEmpty();
    checkInvalidState(__PRETTY_FUNCTION__);
    return *this;
}

const char *LongNumber::toStr() const
{
    const int size = (_int.size() + 1);
    char *s = new char[size];
    for (size_t i = 0; i < _int.size(); ++i)
        s[i] = '0' + _int.at(i);
    s[size-1] = '\0';
    return s;
}

int LongNumber::toInt() const
{
    int i = 0;
    int des = 1;
    for (int index = _int.size() - 1; index >= 0; --index)
    {
        i += des * _int.at(index);
        des *= 10;
    }
    return i;
}

bool LongNumber::isZero() const
{
    return (_int.size() == 1) && (_int.front() == 0);
}

bool LongNumber::operator ==(const LongNumber &other) const
{
    return _int == other._int;
}

int LongNumber::count() const
{
    return _int.size();
}

/*!
 * \brief LongNumber::suma calculate suma of \a first and \a second vectors.
 * First size must be bigger then second size.
 * \return result vector
 */
std::vector<int> LongNumber::suma(const std::vector<int> &first, const std::vector<int> &second)
{
    std::vector<int> suma;
    std::vector<int> m_second = second;
    int base {0};
    while (m_second.size() < first.size())
        m_second.insert(m_second.begin(), 0);
    for (int f = first.size() - 1; f >=0; --f)
    {
        suma.insert(suma.begin(), (first.at(f)+m_second.at(f)+base)%10);
        base = (first.at(f)+m_second.at(f)+base)/10;
    }
    if (base != 0)
        suma.insert(suma.begin(), base);
    return suma;
}

void LongNumber::multiplyTen(int count)
{
    if (!isZero())
        _int.insert(_int.end(), count, 0);
}

void LongNumber::initEmpty()
{
    if (_int.size() == 0)
        _int.push_back(0);
}

void LongNumber::checkInvalidState(const char *where)
{
    if (_int.size() > 1)
    {
        if (_int.front() == 0)
        {
            qDebug() << where << toStr();
            assert(_int.front() != 0);
        }
    }
}

void LongNumber::getParameters(const LongNumber &first, const LongNumber &second, LongNumber &a,
                               LongNumber &b, LongNumber &c, LongNumber &d, int &n)
{
    const int max = std::max(first.count(), second.count());
    n = max / 2;

    auto divide = [=](const LongNumber &number, LongNumber &partFirst, LongNumber &partSecond)
    {
        const int mid = max - n;
        if (number.count() == max)
        {
            partFirst = LongNumber(std::vector<int>(number._int.begin(), number._int.begin() + mid));
            partSecond = LongNumber(std::vector<int>(number._int.begin() + mid, number._int.end()));
        }
        else
        {
            int count = n;
            auto backIter = number._int.rbegin();
            partSecond._int.clear();
            while (backIter != number._int.rend() && count != 0)
            {
                partSecond._int.insert(partSecond._int.begin(), *backIter);
                count--;
                backIter++;
            }
            partFirst = LongNumber(0);
            if (backIter != number._int.rend())
            {
                std::vector<int> out(number._int.rend() - backIter);
                std::reverse_copy(backIter, number._int.rend(), std::begin(out));
                partFirst = LongNumber(out);
            }
        }
    };

    divide(first, a, b);
    divide(second, c, d);
    n = n * 2;
}

QDebug operator <<(QDebug debug, const LongNumber &number)
{
    debug << number._int;
    return debug;
}

LongNumber operator +(const LongNumber &first, const LongNumber &second)
{
    LongNumber l_number{first};
    return l_number + second;
}

LongNumber operator *(const LongNumber &first, const LongNumber &second)
{
    LongNumber l_number{first};
    return l_number * second;
}

LongNumber operator -(const LongNumber &first, const LongNumber &second)
{
    LongNumber l_number{first};
    return l_number - second;
}
