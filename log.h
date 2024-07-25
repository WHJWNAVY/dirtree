#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <inttypes.h>
#include <ctype.h>

#define LOGP(LEVEL, FMT, ...)                                                    \
    do {                                                                         \
        fprintf(stderr, "(%s:%d) " FMT "\n", __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define LOG_DEBUG(FMT, ...) LOGP(LOG_DEBUG, FMT, ##__VA_ARGS__)
#define LOG_INFO(FMT, ...) LOGP(LOG_INFO, FMT, ##__VA_ARGS__)
#define LOG_WARN(FMT, ...) LOGP(LOG_WARNING, FMT, ##__VA_ARGS__)
#define LOG_ERROR(FMT, ...) LOGP(LOG_ERR, FMT, ##__VA_ARGS__)

char *printf_hex(const uint8_t *buf, uint32_t size);
char *dump_hex(uint8_t *bytes, size_t len);
uint8_t *scanf_hex(char *str, size_t *len);
#endif