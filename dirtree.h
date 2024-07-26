#ifndef _DIRTREE_H_
#define _DIRTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <inttypes.h>

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>

#include <fcntl.h>
#include <assert.h>
#include <libgen.h>
#include <getopt.h>

#include <sys/stat.h>

#define DEBUG 1

typedef struct {
    uint64_t ul_fid;
    uint64_t ul_size;
    uint64_t ul_atime;
    uint64_t ul_mtime;
    uint64_t ul_ctime;
    uint16_t us_mode;
    bool b_isdir;
} fileattr;

typedef struct _dirtree {
    char *name;
    fileattr attribute;
    struct _dirtree *parent;
    struct _dirtree *children;
    struct _dirtree *siblings;
} dirtree;

#if 0
typedef struct {
    char *matchname;
    char *matchext;
    dirtree *current;
} dirtree_finder;
#endif

dirtree *dirtree_create(void);

void dirtree_destroy(dirtree *tree);

dirtree *dirtree_find(const dirtree *tree, const char *path);
dirtree *dirtree_findpartial(const dirtree *tree, const char *path, const char **leftovers);

dirtree *dirtree_add(dirtree *tree, const char *path, fileattr *attributes);
dirtree *dirtree_addfile(dirtree *tree, const char *path, bool isdirectory);

bool dirtree_remove(dirtree *tree, dirtree *subtree);

void dirtree_setattr(dirtree *tree, fileattr *attributes);

void dirtree_getattr(dirtree *tree, fileattr *attributes);

bool dirtree_getname(dirtree *tree, char *buff, size_t maxlen);

bool dirtree_fileexists(const dirtree *tree, const char *path);

typedef bool (*dirtreeforeachcb)(const dirtree *tree, void *data);
void dirtree_readdir(const dirtree *tree, dirtreeforeachcb callback, void *data);

void dirtree_foreach(const dirtree *tree, dirtreeforeachcb callback, void *data);

#if 0
dirtree_finder *dirtree_createfinder(dirtree *tree, const char *path);
void dirtree_destroyfinder(dirtree_finder *finder);
dirtree *dirtree_findergetnextfile(dirtree_finder *finder);
#endif

#ifdef DEBUG
void dirtree_dump(const dirtree *tree);
#endif

#endif
