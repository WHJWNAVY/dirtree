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

#define DIRTREE_DEBUG 1

#ifndef MAX_PATH // PATH_MAX
#define MAX_PATH 260
#endif

#ifndef MAX_DRIVE
#define MAX_DRIVE 3
#endif

#ifndef MAX_DIR
#define MAX_DIR 256
#endif

#ifndef MAX_FNAME // NAME_MAX
#define MAX_FNAME 256
#endif

#ifndef MAX_EXT
#define MAX_EXT 256
#endif

typedef struct {
    uint64_t ul_fid;
    uint64_t ul_size;
    uint64_t ul_offset;
    uint64_t ul_atime;
    uint64_t ul_mtime;
    uint64_t ul_ctime;
    uint32_t ui_mode;
    int32_t si_nlink;
    uint32_t ui_uid;
    uint32_t ui_gid;
    bool b_isdir;
    bool b_flock;
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

#define FILEATTR_MODE_DEF (0777)
#define FILEATTR_MODE_DIR (S_IFDIR | FILEATTR_MODE_DEF) /* Directory. */
#define FILEATTR_MODE_REG (S_IFREG | FILEATTR_MODE_DEF) /* Regular file. */

#define FILEATTR_SETMODE(_attr, _isdir)                                        \
    do {                                                                       \
        (_attr)->b_isdir = (_isdir);                                           \
        (_attr)->ui_mode = ((_isdir) ? FILEATTR_MODE_DIR : FILEATTR_MODE_REG); \
        (_attr)->si_nlink = 0;                                                 \
        (_attr)->ul_offset = 0;                                                \
        (_attr)->ui_gid = getgid();                                            \
        (_attr)->ui_uid = getuid();                                            \
    } while (0)

#define DIRTREE_SETMODE(_tree, _isdir) FILEATTR_SETMODE(&((_tree)->attribute), _isdir)

#define FILEATTR_SETDIR(_attr) FILEATTR_SETMODE(_attr, true)
#define DIRTREE_SETDIR(_tree) DIRTREE_SETMODE(_tree, true)

#define FILEATTR_SETFILE(_attr) FILEATTR_SETMODE(_attr, false)
#define DIRTREE_SETFILE(_tree) DIRTREE_SETMODE(_tree, false)

// #define FILEATTR_ISDIR(_attr) S_ISDIR((_attr)->ui_mode)
#define FILEATTR_ISDIR(_attr) ((_attr)->b_isdir)
#define DIRTREE_ISDIR(_tree) FILEATTR_ISDIR(&((_tree)->attribute))

#define FILEATTR_ISVALID(_attr) ((_attr)->ul_fid > 0)
#define DIRTREE_ISVALID(_tree) FILEATTR_ISVALID(&((_tree)->attribute))

dirtree *dirtree_create(void);

void dirtree_destroy(dirtree *tree);

dirtree *dirtree_find(const dirtree *tree, const char *path);
dirtree *dirtree_findpartial(const dirtree *tree, const char *path, const char **leftovers);

dirtree *dirtree_add(dirtree *tree, const char *path, fileattr *attributes);
dirtree *dirtree_addfile(dirtree *tree, const char *path, bool isdirectory);

bool dirtree_remove(dirtree *tree, dirtree *subtree);
bool dirtree_remsub(dirtree *tree);

void dirtree_setattr(dirtree *tree, fileattr *attributes);

void dirtree_getattr(dirtree *tree, fileattr *attributes);

bool dirtree_getname(dirtree *tree, char *buff, size_t maxlen);
bool dirtree_getpath(dirtree *tree, char *buff, size_t maxlen);

bool dirtree_fileexists(const dirtree *tree, const char *path);

typedef bool (*dirtreeforeachcb)(const dirtree *tree, void *data);
void dirtree_readdir(const dirtree *tree, dirtreeforeachcb callback, void *data);

void dirtree_foreach(const dirtree *tree, dirtreeforeachcb callback, void *data);

#if 0
dirtree_finder *dirtree_createfinder(dirtree *tree, const char *path);
void dirtree_destroyfinder(dirtree_finder *finder);
dirtree *dirtree_findergetnextfile(dirtree_finder *finder);
#endif

#ifdef DIRTREE_DEBUG
void dirtree_dump(const dirtree *tree);
#else
#define dirtree_dump(tree)
#endif

#endif
