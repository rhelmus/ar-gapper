#ifndef UTILS_H
#define UTILS_H

inline bool check64Bit(uint64_t val, uint8_t bit) { return (val & (1ULL << bit)); }
inline void set64Bit(uint64_t &val, uint8_t bit) { val |= (1ULL << bit); }

void assetASCII(void);

#endif // UTILS_H
