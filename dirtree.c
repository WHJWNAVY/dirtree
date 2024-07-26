/****************************************************************************************/
/*  DIRTREE.C                                                                           */
/*                                                                                      */
/*  Author: Eli Boling                                                                  */
/*  Description: Directory tree implementation                                          */
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
/*Genesis3D Version 1.1 released November 15, 1999                            */
/*  Copyright (C) 1999 WildTangent, Inc. All Rights Reserved           */
/*                                                                                      */
/****************************************************************************************/
#include "log.h"
#include "dirtree.h"

#define Assert(_x)                                   \
    if (!(_x)) {                                     \
        LOG_ERROR("Assertion failed of [%s]!", #_x); \
        return;                                      \
    }

#define AssertV(_x, _v)                              \
    if (!(_x)) {                                     \
        LOG_ERROR("Assertion failed of [%s]!", #_x); \
        return (_v);                                 \
    }

DirTree *DirTree_Create(void) {
    DirTree *Tree = NULL;

    Tree = malloc(sizeof(*Tree));
    if (!Tree)
        return Tree;

    memset(Tree, 0, sizeof(*Tree));
    Tree->Name = strdup("");
    if (!Tree->Name) {
        free(Tree);
        return NULL;
    }

    Tree->Attribute.b_isdir = true;

    return Tree;
}

void DirTree_Destroy(DirTree *Tree) {
    Assert(Tree);
    assert(Tree->Name);

    if (Tree->Children)
        DirTree_Destroy(Tree->Children);

    if (Tree->Siblings)
        DirTree_Destroy(Tree->Siblings);

    free(Tree->Name);
    free(Tree);
}

DirTree *DirTree_FindExact(const DirTree *Tree, const char *Path) {
    static char Buff[_MAX_PATH] = {0};
    DirTree *Siblings = NULL;

    AssertV(Tree, NULL);
    AssertV(Path, NULL);

    if (*Path == '/')
        return NULL;

    if (*Path == '\0')
        return (DirTree *)Tree;

    Path = GetNextDir(Path, Buff);

    Siblings = Tree->Children;
    while (Siblings) {
        if (!strcmp(Siblings->Name, Buff)) {
            if (!*Path)
                return Siblings;
            return DirTree_FindExact(Siblings, Path);
        }
        Siblings = Siblings->Siblings;
    }

    return NULL;
}

DirTree *DirTree_FindPartial(const DirTree *Tree, const char *Path, const char **LeftOvers) {
    static char Buff[_MAX_PATH] = {0};
    DirTree *Siblings = NULL;

    AssertV(Tree, NULL);
    AssertV(Path, NULL);

    if (*Path == '/')
        return NULL;

    *LeftOvers = Path;

    if (*Path == '\0')
        return (DirTree *)Tree;

    Path = GetNextDir(Path, Buff);

    Siblings = Tree->Children;
    while (Siblings) {
        if (!strcmp(Siblings->Name, Buff)) {
            *LeftOvers = Path;
            if (!*Path)
                return Siblings;
            return DirTree_FindPartial(Siblings, Path, LeftOvers);
        }
        Siblings = Siblings->Siblings;
    }

    return (DirTree *)Tree;
}

DirTree *DirTree_Add(DirTree *Tree, const char *Path, FileAttr *Attributes) {
    DirTree *NewEntry = NULL;
    const char *LeftOvers = NULL;

    AssertV(Tree, NULL);
    AssertV(Path, NULL);
    AssertV(Attributes, NULL);
    AssertV((strlen(Path) > 0), NULL);

    if (PathHasDir(Path)) {
        Tree = DirTree_FindPartial(Tree, Path, &LeftOvers);
        if (!Tree)
            return NULL;

        if (PathHasDir(LeftOvers))
            return NULL;

        Path = LeftOvers;
    }

    NewEntry = malloc(sizeof(*NewEntry));
    if (!NewEntry) {
        return NULL;
    }

    memset(NewEntry, 0, sizeof(*NewEntry));
    NewEntry->Name = strdup(Path);
    if (!NewEntry->Name) {
        free(NewEntry->Name);
        free(NewEntry);
        return NULL;
    }

    NewEntry->Siblings = Tree->Children;
    Tree->Children = NewEntry;
    NewEntry->Parent = Tree;

    memcpy(&(NewEntry->Attribute), Attributes, sizeof(FileAttr));

    return NewEntry;
}

DirTree *DirTree_AddFile(DirTree *Tree, const char *Path, bool IsDirectory) {
#if 0
    DirTree *NewEntry = NULL;
    const char *LeftOvers = NULL;

    AssertV(Tree, NULL);
    AssertV(Path, NULL);
    AssertV((strlen(Path) > 0), NULL);

    if (PathHasDir(Path)) {
        Tree = DirTree_FindPartial(Tree, Path, &LeftOvers);
        if (!Tree)
            return NULL;

        if (PathHasDir(LeftOvers))
            return NULL;

        Path = LeftOvers;
    }

    NewEntry = malloc(sizeof(*NewEntry));
    if (!NewEntry) {
        return NULL;
    }

    memset(NewEntry, 0, sizeof(*NewEntry));
    NewEntry->Name = strdup(Path);
    if (!NewEntry->Name) {
        free(NewEntry->Name);
        free(NewEntry);
        return NULL;
    }

    NewEntry->Siblings = Tree->Children;
    Tree->Children = NewEntry;
    NewEntry->Parent = Tree;

    if (IsDirectory == true) {
        NewEntry->Attribute.b_isdir = true;
    }

    return NewEntry;
#else
    FileAttr attr = {0};
    attr.b_isdir = IsDirectory;
    return DirTree_Add(Tree, Path, &attr);
#endif
}

bool DirTree_Remove(DirTree *Tree, DirTree *SubTree) {
    DirTree Siblings = {0};
    DirTree *pSiblings = NULL;
    DirTree *Parent = NULL;
    DirTree *ParanoiaCheck = NULL;

    AssertV(Tree, false);
    AssertV(SubTree, false);

    Parent = SubTree->Parent;
    AssertV(Parent, false);

    ParanoiaCheck = Parent;
    while (ParanoiaCheck && ParanoiaCheck != Tree)
        ParanoiaCheck = ParanoiaCheck->Parent;
    if (!ParanoiaCheck)
        return false;

    Siblings.Siblings = Parent->Children;
    AssertV(Siblings.Siblings, false);
    pSiblings = &Siblings;
    while (pSiblings->Siblings) {
        if (pSiblings->Siblings == SubTree) {
            pSiblings->Siblings = SubTree->Siblings;
            if (SubTree == Parent->Children)
                Parent->Children = SubTree->Siblings;
            SubTree->Siblings = NULL;
            DirTree_Destroy(SubTree);
            return true;
        }
        pSiblings = pSiblings->Siblings;
    }

    AssertV(!"Shouldn't be a way to get here", false);
    return false;
}

void DirTree_SetFileAttr(DirTree *Tree, FileAttr *Attributes) {
    Assert(Tree);
    Assert(Attributes);

    memcpy(&(Tree->Attribute), Attributes, sizeof(FileAttr));
}

void DirTree_GetFileAttr(DirTree *Tree, FileAttr *Attributes) {
    Assert(Tree);
    Assert(Attributes);

    memcpy(Attributes, &(Tree->Attribute), sizeof(FileAttr));
}

bool DirTree_GetName(DirTree *Tree, char *Buff, int32_t MaxLen) {
    int32_t Length = 0;

    AssertV(Tree, false);
    AssertV(Buff, false);
    AssertV((MaxLen > 0), false);

    Length = strlen(Tree->Name);
    if (Length > MaxLen)
        return false;

    memcpy(Buff, Tree->Name, Length + 1);

    return true;
}

bool DirTree_FileExists(const DirTree *Tree, const char *Path) {
    return (DirTree_FindExact(Tree, Path) != NULL);
}

#if 0
DirTree_Finder *DirTree_CreateFinder(DirTree *Tree, const char *Path) {
    DirTree_Finder *Finder = NULL;
    DirTree *SubTree = NULL;
    char Directory[_MAX_PATH] = {0};
    char Name[_MAX_FNAME] = {0};
    char Ext[_MAX_EXT] = {0};

    AssertV(Tree,NULL);
    AssertV(Path,NULL);

    Splitpath(Path, NULL, Directory, Name, Ext);
    LOG_DEBUG("===dir[%s], name[%s], ext[%s]===", Directory, Name, Ext);
    SubTree = DirTree_FindExact(Tree, Directory);
    if (!SubTree) {
        return NULL;
    }

    Finder = malloc(sizeof(*Finder));
    if (!Finder)
        return Finder;

    Finder->MatchName = strdup(Name);
    if (!Finder->MatchName) {
        free(Finder);
        return NULL;
    }

    // The RTL leaves the '.' on there.  That won't do.
    if (*Ext == '.') {
        Finder->MatchExt = strdup(&Ext[1]);
    } else {
        Finder->MatchExt = strdup(&Ext[0]);
    }

    if (!Finder->MatchExt) {
        free(Finder->MatchName);
        free(Finder);
        return NULL;
    }

    Finder->Current = SubTree->Children;

    return Finder;
}

void DirTree_DestroyFinder(DirTree_Finder *Finder) {
    Assert(Finder);
    Assert(Finder->MatchName);
    Assert(Finder->MatchExt);

    free(Finder->MatchName);
    free(Finder->MatchExt);
    free(Finder);
}

DirTree *DirTree_FinderGetNextFile(DirTree_Finder *Finder) {
    DirTree *Res = NULL;
    char Name[_MAX_FNAME] = {0};
    char Ext[_MAX_EXT] = {0};

    AssertV(Finder,NULL);

    Res = Finder->Current;

    if (!Res) {
        return Res;
    }

    do {
        Splitpath(Res->Name, NULL, NULL, Name, Ext);
        LOG_DEBUG("===name[%s], ext[%s]===", Name, Ext);
        if (MatchPattern(Name, Finder->MatchName) == true && MatchPattern(Ext, Finder->MatchExt) == true) {
            break;
        }
        Res = Res->Siblings;
    } while (Res);

    if (Res) {
        Finder->Current = Res->Siblings;
    }

    return Res;
}
#endif

void DirTree_ReadDir(const DirTree *Tree, DirTreeForeachCb CallBack, void *Data) {
    DirTree *Temp = NULL;
    Assert(Tree);
    Assert(CallBack);

    Temp = Tree->Children;
    while (Temp) {
        if (CallBack(Temp, Data)) {
            break;
        }
        Temp = Temp->Siblings;
    }
}

static void DirTree_ForeachR(const DirTree *Tree, DirTreeForeachCb CallBack, void *Data) {
    DirTree *Temp = NULL;
    Assert(Tree);
    Assert(CallBack);

    Tree = Tree->Children;
    while (Tree) {
        CallBack(Tree, Data);
        DirTree_ForeachR(Tree, CallBack, Data);
        Tree = Tree->Siblings;
    }
}

void DirTree_Foreach(const DirTree *Tree, DirTreeForeachCb CallBack, void *Data) {
#if 0
    Assert(Tree);
    Assert(CallBack);

    CallBack(Tree, Data);

    Tree = Tree->Children;
    while (Tree) {
        DirTree_Foreach(Tree, CallBack, Data);
        Tree = Tree->Siblings;
    }
#else
    Assert(Tree);
    Assert(CallBack);

    DirTree_ForeachR(Tree, CallBack, Data);
#endif
}

#ifdef DEBUG
static inline void indent(int i) {
    while (i--) {
        printf(" ");
    }
}

static void DirTreeDump1(const DirTree *Tree, int i) {
    DirTree *Temp = NULL;
    Assert(Tree);

    indent(i);
    if (Tree->Attribute.b_isdir) {
        printf("/%-*s  %-8lu\n", 39 - i, Tree->Name, Tree->Attribute.ul_fid);
    } else {
        printf("%-*s  %-8lu  %-8lu\n", 40 - i, Tree->Name, Tree->Attribute.ul_fid, Tree->Attribute.ul_size);
    }
    Temp = Tree->Children;
    while (Temp) {
        DirTreeDump1(Temp, i + 2);
        Temp = Temp->Siblings;
    }
}

void DirTreeDump(const DirTree *Tree) {
    Assert(Tree);

    printf("%-*s  %-8s  %-8s\n", 40, "Name", "Fid", "Size");
    printf("------------------------------------------------------------\n");
    DirTreeDump1(Tree, 0);
}

#endif
