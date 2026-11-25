#ifndef DateTime_H
#define DateTime_H
#include <stdint.h>

class DateTime
{
public:
    // year(2 bytes), month(1bytes), day(1bytes)
    uint32_t source;
    DateTime(uint32_t x): source(x) {}
    DateTime(int y, int m, int d)
    {
        this->set(y,m,d);
    }
    uint32_t getSrc() { return source; }
    void set(int y, int m, int d)
    {
        source = (((y & 0xffff) << 16) | ((m & 0xff) << 8) | (d & 0xff));
    }
    int getYear() { return (0xffff0000 & source) >> 16; }
    int getMonth() { return (0xff00 & source) >> 8; }
    int getDay() { return (0xff & source); }
};
#endif // DateTime_H
