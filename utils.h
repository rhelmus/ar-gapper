#ifndef UTILS_H
#define UTILS_H

inline bool check64Bit(uint64_t val, uint8_t bit) { return (val & (1ULL << bit)); }
inline void set64Bit(uint64_t &val, uint8_t bit) { val |= (1ULL << bit); }

void assetASCII(void);
uint16_t atxy(uint8_t x, uint8_t y);
void putstr_P(int x, int y, const char *s);

#endif // UTILS_H
