#include "number.h"

int2023_t operator~(const int2023_t& int2023){
    int2023_t inversion;
    for (int i = 0; i < 253; i++) {
        inversion.storage[i] = ~(int2023.storage[i]);
    }
    inversion.storage[252]++;
    int i = 252;
    while (inversion.storage[i] == 0 && i > 0) {
        inversion.storage[i-1]++;
        i--;
    }
    return inversion;
}

int2023_t from_int(int32_t i) {
    int2023_t int2023_t_from_int;
    int32_t num = i;
    bool flag = false;
    if (num < 0) {
        num = -num;
        flag = true;
    }
    uint8_t sum = 0;
    int count = 252;
    for (int j = 0; j < 8; j++) {
        if (num&(1 << j)) {
            int k = 1;
            for (int t = 0; t < j%8; t++) {
                k *= 2;
            }
            sum += k;
        }
    }
    int2023_t_from_int.storage[252] = sum;
    sum = 0;
    for (int j = 8; j < 16; j++) {
        if (num&(1 << j)) {
            int k = 1;
            for (int t = 0; t < j%8; t++) {
                k *= 2;
            }
            sum += k;
        }
    }
    int2023_t_from_int.storage[251] = sum;
    sum = 0;
    for (int j = 16; j < 24; j++) {
        if (num&(1 << j)) {
            int k = 1;
            for (int t = 0; t < j%8; t++) {
                k *= 2;
            }
            sum += k;
        }
    }
    int2023_t_from_int.storage[250] = sum;
    sum = 0;
    for (int j = 24; j < 32; j++) {
        if (num&(1 << j)) {
            int k = 1;
            for (int t = 0; t < j%8; t++) {
                k *= 2;
            }
            sum += k;
        }
    }
    int2023_t_from_int.storage[249] = sum;
    if (flag) {
        return ~int2023_t_from_int;
    }
    return int2023_t_from_int;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t int2023_t_sum;
    uint8_t carry = 0;
    for (int i = 252; i >= 0; i--) {
        int2023_t_sum.storage[i] = (lhs.storage[i] + rhs.storage[i] + carry) % 256;
        carry = (lhs.storage[i] + rhs.storage[i] + carry) / 256;
    }
    return int2023_t_sum;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    return lhs + ~rhs;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    int count = 0;
    uint8_t intermediate[253];
    for (int i = 252; i >= 0; i--) {
        if (rhs.storage[i] != 0) {
            int carry = 0;
            for (int j = 252; j >= 0; j--) {
                int a = (int) lhs.storage[j] * rhs.storage[i] + carry;
                carry = a / 256;
                intermediate[j] = a % 256;
            }
            uint8_t carry1 = 0;
            for (int k = 252; k >= count; k--) {
                uint8_t copy = result.storage[k - count];
                result.storage[k - count] = (result.storage[k - count] + intermediate[k] + carry1) % 256;
                carry1 = (copy + intermediate[k] + carry1) / 256;
            }
        }
        count++;
    }
    return result;
}

int2023_t from_string(const char* buff) {
    int2023_t result;
    bool flag = false;
    if (buff[0] == '-') {
        flag = true;
        buff++;
    }
    size_t sizelen_of_int2023 = strlen(buff);
    for (int i = 0; i < sizelen_of_int2023; i++) {
        result = result*from_int(10);
        result = result + from_int(buff[i] - '0');
    }
    if (flag) {
        return  ~result;
    }
    return result;
}



int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t divide;
    if (lhs.storage[0] >= 128 && rhs.storage[0] >= 128) {
        return ~lhs / ~rhs;
    } else if (lhs.storage[0] >= 128 && rhs.storage[0] < 128) {
        return ~(~lhs / rhs);
    } else if (lhs.storage[0] < 128 && rhs.storage[0] >= 128) {
        return ~(lhs / ~rhs);
    }
    int i = 0;
    int j = 0;
    while (i < 253) {
        if (lhs.storage[i] != 0) {
            break;
        }
        i++;
    }
    while (j < 253) {
        if (rhs.storage[j] != 0) {
            break;
        }
        j++;
    }
    if (i > j) {
        return from_int(0);
    }
    if (rhs == from_int(1)) {
        return lhs;
    }
    for (int k = 253 - (j - i) - 1; k < 253; k++) {
        int l = 0;
        int r = 255;
        while (r - l > 0) {
            if (r-l == 1) {
                divide.storage[k] = l;
                break;
            }
            int m = (l+r)/2;
            divide.storage[k] = m;
            if ((lhs - rhs*divide).storage[0] >= 128) {
                r = m;
            }
            else {
                l = m;
            }
        }
    }
    return divide;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    for (int i = 0; i < 253; i++) {
        if (lhs.storage[i] != rhs.storage[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    for (uint8_t i: value.storage) {
        stream << (int) i << " ";
    }
    return stream;
}
