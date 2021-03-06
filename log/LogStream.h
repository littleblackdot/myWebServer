#pragma once

#include "FixedBuffer.h"
#include "../noncopyable.h"
#include <assert.h>
#include <string>
#include <cstring>


class LogStream : noncopyable {
public:
  typedef FixedBuffer<kSmallBuffer> Buffer;

  LogStream& operator<<(bool v) {
    buffer_.append(v ? "1" : "0", 1);
    return *this;
  }

  LogStream& operator<<(short);
  LogStream& operator<<(unsigned short);
  LogStream& operator<<(int);
  LogStream& operator<<(unsigned int);
  LogStream& operator<<(long);
  LogStream& operator<<(unsigned long);
  LogStream& operator<<(long long);
  LogStream& operator<<(unsigned long long);

  LogStream& operator<<(const void*);

  LogStream& operator<<(float v) {
    *this << static_cast<double>(v);
    return *this;
  }
  LogStream& operator<<(double);
  LogStream& operator<<(long double);

  LogStream& operator<<(char v) {
    buffer_.append(&v, 1);
    return *this;
  }

  LogStream& operator<<(const char* str) {
    if (str)
      buffer_.append(str, strlen(str));
    else
      buffer_.append("(null)", 6);
    return *this;
  }

  LogStream& operator<<(const unsigned char* str) {
    return operator<<(reinterpret_cast<const char*>(str));
  }

  LogStream& operator<<(const std::string& v) {
    buffer_.append(v.c_str(), v.size());
    return *this;
  }

  void append(const char* data, int len) { buffer_.append(data, len); }
  const Buffer& buffer() const { return buffer_; }
  void resetBuffer() { buffer_.reset(); }

 private:
  void staticCheck();

  template <typename T>
  void formatInteger(T);

  Buffer buffer_;

  static const int kMaxNumericSize = 32;
};