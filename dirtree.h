#ifndef DIRTREE_H
#define DIRTREE_H

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

#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif

#ifndef _MAX_DRIVE
#define _MAX_DRIVE 3
#endif

#ifndef _MAX_DIR
#define _MAX_DIR 256
#endif

#ifndef _MAX_FNAME
#define _MAX_FNAME 256
#endif

#ifndef _MAX_EXT
#define _MAX_EXT 256
#endif

typedef struct {
    uint64_t ul_fid;
    uint64_t ul_size;
    uint64_t ul_atime;
    uint64_t ul_mtime;
    uint64_t ul_ctime;
    uint16_t us_mode;
    bool b_isdir;
} FileAttr;

typedef struct _DirTree {
    char *Name;
    FileAttr Attribute;
    struct _DirTree *Parent;
    struct _DirTree *Children;
    struct _DirTree *Siblings;
} DirTree;

#if 0
typedef struct {
    char *MatchName;
    char *MatchExt;
    DirTree *Current;
} DirTree_Finder;
#endif

DirTree *DirTree_Create(void);

void DirTree_Destroy(DirTree *Tree);

DirTree *DirTree_FindExact(const DirTree *Tree, const char *Path);
DirTree *DirTree_FindPartial(const DirTree *Tree, const char *Path, const char **LeftOvers);

DirTree *DirTree_Add(DirTree *Tree, const char *Path, FileAttr *Attributes);
DirTree *DirTree_AddFile(DirTree *Tree, const char *Path, bool IsDirectory);

bool DirTree_Remove(DirTree *Tree, DirTree *SubTree);

void DirTree_SetFileAttr(DirTree *Tree, FileAttr *Attributes);

void DirTree_GetFileAttr(DirTree *Tree, FileAttr *Attributes);

bool DirTree_GetName(DirTree *Tree, char *Buff, int MaxLen);

bool DirTree_FileExists(const DirTree *Tree, const char *Path);

typedef bool (*DirTreeForeachCb)(const DirTree *Tree, void *Data);
void DirTree_ReadDir(const DirTree *Tree, DirTreeForeachCb CallBack, void *Data);

void DirTree_Foreach(const DirTree *Tree, DirTreeForeachCb CallBack, void *Data);

#if 0
DirTree_Finder *DirTree_CreateFinder(DirTree *Tree, const char *Path);
void DirTree_DestroyFinder(DirTree_Finder *Finder);
DirTree *DirTree_FinderGetNextFile(DirTree_Finder *Finder);
#endif

#ifdef DEBUG
void DirTree_Dump(const DirTree *Tree);
#endif

#endif
