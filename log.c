#include "log.h"

char *printf_hex(const uint8_t *buf, uint32_t size) {
    int i = 0, j = 0;
    size_t pn = 16; // The number of outputs per line
    size_t mn = 0, sz = 0;
    char *hexstr = NULL;
    char *tmp = NULL;

    if ((buf == NULL) || (size == 0)) {
        return;
    }

    mn = size / pn;
    mn = ((size % pn) ? (mn + 1) : mn);
    sz = (size * 4) + (mn * 20) + 80;
    // LOG_DEBUG("size = %d, sz = %d", size, sz);

#define __chksz_end         \
    ;                       \
    if (tmp - hexstr >= sz) \
        break;

    hexstr = malloc(sz);
    tmp = hexstr;

#ifndef isprint
#define __is_print(ch) ((uint32_t)((ch) - ' ') < 127u - ' ')
#else
#define __is_print(ch) isprint((ch))
#endif

    for (i = 0; i < size; i += pn) {
        __chksz_end;
        sprintf(tmp, "%08X: ", i);
        tmp += strlen(tmp);
        for (j = 0; j < pn; j++) {
            if (j % 8 == 0) {
                __chksz_end;
                sprintf(tmp, " ");
                tmp += strlen(tmp);
            }
            if (i + j < size) {
                __chksz_end;
                sprintf(tmp, "%02X ", buf[i + j]);
                tmp += strlen(tmp);
            } else {
                __chksz_end;
                sprintf(tmp, "   ");
                tmp += strlen(tmp);
            }
        }
        __chksz_end;
        sprintf(tmp, " |  ");
        tmp += strlen(tmp);

        for (j = 0; j < pn; j++) {
            if (i + j < size) {
                __chksz_end;
                sprintf(tmp, "%c", __is_print(buf[i + j]) ? buf[i + j] : '.');
                tmp += strlen(tmp);
            }
        }
        __chksz_end;
        sprintf(tmp, "\n");
        tmp += strlen(tmp);
    }
    return hexstr;
}

char *dump_hex(uint8_t *bytes, size_t len) {
    size_t i = 0;
    size_t sz = (2 * len + 1);

    char *hexstr = malloc(sz);
    char *tmp = hexstr;
    memset(tmp, 0, sz);

    // LOG_DEBUG("len = %d", len);

    for (i = 0; i < len; i++) {
        // LOG_DEBUG("bytes[%d] = 0x%02x", i, bytes[i]);
        sprintf(tmp, "%02X", bytes[i]);
        // LOG_DEBUG("hexstr[%d] = %s", i, tmp);
        tmp += 2;
    }
    *tmp = '\0';
    // LOG_DEBUG("hexstr = %s", hexstr);

    return hexstr;
}

uint8_t *scanf_hex(char *str, size_t *len) {
    size_t i = 0;
    size_t sz = (strlen(str) / 2);

    uint8_t *bytes = malloc(sz);
    memset(bytes, 0, sz);

    // LOG_DEBUG("sz = %d", sz);

    for (i = 0; i < sz; i++) {
        // LOG_DEBUG("str[%d] = %c%c", i, str[i * 2], str[i * 2 + 1]);
        sscanf(&(str[i * 2]), "%2hhx", &(bytes[i]));
        // LOG_DEBUG("bytes[%d] = 0x%02x", i, bytes[i]);
    }
    *len = sz;
    return bytes;
}
