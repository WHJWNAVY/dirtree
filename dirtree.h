/****************************************************************************************/
/*  DIRTREE.H                                                                           */
/*                                                                                      */
/*  Author: Eli Boling                                                                  */
/*  Description: Directory tree interface                                               */
/*                                                                                      */
/*  The contents of this file are subject to the Genesis3D Public License               */
/*  Version 1.01 (the "License"); you may not use this file except in                   */
/*  compliance with the License. You may obtain a copy of the License at                */
/*  http://www.genesis3d.com                                                            */
/*                                                                                      */
/*  Software distributed under the License is distributed on an "AS IS"                 */
/*  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See                */
/*  the License for the specific language governing rights and limitations              */
/*  under the License.                                                                  */
/*                                                                                      */
/*  The Original Code is Genesis3D, released March 25, 1999.                            */
/*  Genesis3D Version 1.1 released November 15, 1999                                 */
/*  Copyright (C) 1999 WildTangent, Inc. All Rights Reserved           */
/*                                                                                      */
/****************************************************************************************/
#ifndef DIRTREE_H
#define DIRTREE_H

#include "common.h"

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
void DirTreeDump(const DirTree *Tree);
#endif

#endif
