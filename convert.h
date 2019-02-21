#ifndef CONVERT_H
#define CONVERT_H

#include <vector>

std::vector<int> toVector(const char *str)
{
    std::vector<int> data;
    for (int i = 0; str[i] != '\0'; ++i)
        data.push_back(str[i] - '0');
    return data;
}

std::vector<int> toVector(int data)
{
    std::vector<int> d;
    while (data != 0)
    {
        d.insert(d.begin(), data%10);
        data /= 10;
    }
    return d;
}

#endif // CONVERT_H
