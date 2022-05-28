#include "FixedBuffer.h"


const char digits[] = "9876543210123456789";
const char* zero = digits + 9;

// From muduo
template <typename T>
size_t convert(char buf[], T value) {
  T i = value;
  char* p = buf;

  do {
    int lsd = static_cast<int>(i % 10);
    i /= 10;
    *p++ = zero[lsd];
  } while (i != 0);

  if (value < 0) {
    *p++ = '-';
  }
  *p = '\0';
  std::reverse(buf, p);

  return p - buf;
}