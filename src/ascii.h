#pragma once

inline bool IsASCIIAlpha(char c) {
    return (c | 0x20) >= 'a' && (c | 0x20) <= 'z';
}

inline bool IsASCIIDigit(char c) {
    return c >= '0' && c <= '9';
}