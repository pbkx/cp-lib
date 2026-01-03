struct fast_input {
  public:
    fast_input() : in(stdin) {}

    void tie(std::nullptr_t) {}

    explicit operator bool() const { return c != EOF; }

    fast_input& operator>>(char& x) {
        if (skip_blanks()) {
            x = static_cast<char>(c);
            next_char();
        }
        return *this;
    }

    fast_input& operator>>(std::string& s) {
        s.clear();
        if (!skip_blanks()) return *this;
        do {
            s.push_back(static_cast<char>(c));
            next_char();
        } while (c != EOF && !is_blank(c));
        return *this;
    }

    template <class T,
              typename std::enable_if<std::is_integral<T>::value &&
                                      std::is_signed<T>::value>::type* = nullptr>
    fast_input& operator>>(T& x) {
        return read_signed_integer(x);
    }

    template <class T,
              typename std::enable_if<std::is_integral<T>::value &&
                                      std::is_unsigned<T>::value>::type* = nullptr>
    fast_input& operator>>(T& x) {
        return read_unsigned_integer(x);
    }

#if !defined(_WIN32) || defined(_WIN64)
    fast_input& operator>>(__int128& x) { return read_signed_integer(x); }
#endif

    template <class T,
              typename std::enable_if<std::is_floating_point<T>::value>::type* =
                  nullptr>
    fast_input& operator>>(T& x) {
        std::string s;
        *this >> s;
        if (s.empty()) {
            x = 0;
            return *this;
        }
        double y = 0;
        sscanf(s.c_str(), "%lf", &y);
        x = static_cast<T>(y);
        return *this;
    }

  private:
    static constexpr int buffer_size = 1 << 20;
    char buffer[buffer_size];
    int length = 0;
    int position = 0;
    FILE* in;
    int c = ' ';

    static bool is_blank(int x) { return x <= ' '; }

    int next_char() {
        if (position == length) {
            length = int(fread(buffer, 1, buffer_size, in));
            position = 0;
            if (length == 0) {
                c = EOF;
                return c;
            }
        }
        c = static_cast<unsigned char>(buffer[position++]);
        return c;
    }

    bool skip_blanks() {
        while (c != EOF && is_blank(c)) {
            next_char();
        }
        return c != EOF;
    }

    template <class T> fast_input& read_unsigned_integer(T& x) {
        x = 0;
        if (!skip_blanks()) return *this;
        while (c != EOF && !is_blank(c)) {
            x = x * 10 + static_cast<T>(c - '0');
            next_char();
        }
        return *this;
    }

    template <class T> fast_input& read_signed_integer(T& x) {
        x = 0;
        if (!skip_blanks()) return *this;
        int sign = 1;
        if (c == '-') {
            sign = -1;
            next_char();
        } else if (c == '+') {
            next_char();
        }
        while (c != EOF && !is_blank(c)) {
            x = x * 10 + static_cast<T>(c - '0');
            next_char();
        }
        if (sign == -1) x = -x;
        return *this;
    }
} in;

#define cin in
