struct fast_output {
  public:
    fast_output() : out(stdout) {}

    ~fast_output() { flush(); }

    void flush() {
        if (position > 0) {
            fwrite(buffer, 1, position, out);
            position = 0;
        }
    }

    fast_output& operator<<(char x) {
        push_char(x);
        return *this;
    }

    fast_output& operator<<(const char* s) {
        while (*s) {
            push_char(*s++);
        }
        return *this;
    }

    fast_output& operator<<(const std::string& s) {
        for (char x : s) {
            push_char(x);
        }
        return *this;
    }

    template <class T,
              typename std::enable_if<std::is_integral<T>::value &&
                                      std::is_signed<T>::value>::type* = nullptr>
    fast_output& operator<<(T x) {
        write_signed_integer(x);
        return *this;
    }

    template <class T,
              typename std::enable_if<std::is_integral<T>::value &&
                                      std::is_unsigned<T>::value>::type* = nullptr>
    fast_output& operator<<(T x) {
        write_unsigned_integer(x);
        return *this;
    }

#if !defined(_WIN32) || defined(_WIN64)
    fast_output& operator<<(__int128 x) {
        if (x < 0) {
            push_char('-');
            write_unsigned_integer(static_cast<unsigned __int128>(0) -
                                   static_cast<unsigned __int128>(x));
        } else {
            write_unsigned_integer(static_cast<unsigned __int128>(x));
        }
        return *this;
    }
#endif

    template <class T,
              typename std::enable_if<std::is_floating_point<T>::value>::type* =
                  nullptr>
    fast_output& operator<<(T x) {
        int n = snprintf(temp, temp_size, "%.17f", static_cast<double>(x));
        for (int i = 0; i < n; i++) {
            push_char(temp[i]);
        }
        return *this;
    }

  private:
    static constexpr int buffer_size = 1 << 20;
    static constexpr int temp_size = 1 << 20;

    char buffer[buffer_size];
    int position = 0;
    char temp[temp_size];
    FILE* out;

    void push_char(char x) {
        buffer[position++] = x;
        if (position == buffer_size) {
            fwrite(buffer, 1, position, out);
            position = 0;
        }
    }

    template <class T> void write_unsigned_integer(T x) {
        if (x == 0) {
            push_char('0');
            return;
        }
        int n = 0;
        while (x > 0) {
            temp[n++] = static_cast<char>('0' + x % 10);
            x /= 10;
        }
        while (n) {
            push_char(temp[--n]);
        }
    }

    template <class T> void write_signed_integer(T x) {
        using U = typename std::make_unsigned<T>::type;
        if (x < 0) {
            push_char('-');
            write_unsigned_integer(static_cast<U>(0) - static_cast<U>(x));
        } else {
            write_unsigned_integer(static_cast<U>(x));
        }
    }
} out;

#define cout out
