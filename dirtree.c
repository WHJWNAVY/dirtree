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

#include "dirtree.h"

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
    assert(Tree);
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

    assert(Tree);
    assert(Path);

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

    assert(Tree);
    assert(Path);

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

DirTree *DirTree_AddFile(DirTree *Tree, const char *Path, bool IsDirectory) {
    DirTree *NewEntry = NULL;
    const char *LeftOvers = NULL;

    assert(Tree);
    assert(Path);
    assert(IsDirectory == true || IsDirectory == false);

    assert(strlen(Path) > 0);

    if (PathHasDir(Path)) {
        Tree = DirTree_FindPartial(Tree, Path, &LeftOvers);
        if (!Tree)
            return NULL;

        if (PathHasDir(LeftOvers))
            return NULL;

        Path = LeftOvers;
    }

    NewEntry = malloc(sizeof(*NewEntry));
    if (!NewEntry)
        return NULL;

    memset(NewEntry, 0, sizeof(*NewEntry));
    NewEntry->Name = strdup(Path);
    if (!NewEntry->Name) {
        free(NewEntry->Name);
        free(NewEntry);
        return NULL;
    }

    NewEntry->Siblings = Tree->Children;
    Tree->Children = NewEntry;

    if (IsDirectory == true) {
        NewEntry->Attribute.b_isdir = true;
    }

    return NewEntry;
}

bool DirTree_Remove(DirTree *Tree, DirTree *SubTree) {
    DirTree Siblings = {0};
    DirTree *pSiblings = NULL;
    DirTree *Parent = NULL;
    DirTree *ParanoiaCheck = NULL;

    assert(Tree);
    assert(SubTree);

    Parent = SubTree->Parent;
    assert(Parent);

    ParanoiaCheck = Parent;
    while (ParanoiaCheck && ParanoiaCheck != Tree)
        ParanoiaCheck = ParanoiaCheck->Parent;
    if (!ParanoiaCheck)
        return false;

    Siblings.Siblings = Parent->Children;
    assert(Siblings.Siblings);
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

    assert(!"Shouldn't be a way to get here");
    return false;
}

void DirTree_SetFileAttr(DirTree *Tree, FileAttr *Attributes) {
    assert(Tree);
    assert(Attributes);

    memcpy(&(Tree->Attribute), Attributes, sizeof(FileAttr));
}

void DirTree_GetFileAttr(DirTree *Tree, FileAttr *Attributes) {
    assert(Tree);
    assert(Attributes);

    memcpy(Attributes, &(Tree->Attribute), sizeof(FileAttr));
}

bool DirTree_GetName(DirTree *Tree, char *Buff, int32_t MaxLen) {
    int32_t Length = 0;

    assert(Tree);
    assert(Buff);
    assert(MaxLen > 0);

    Length = strlen(Tree->Name);
    if (Length > MaxLen)
        return false;

    memcpy(Buff, Tree->Name, Length + 1);

    return true;
}

bool DirTree_FileExists(const DirTree *Tree, const char *Path) {
    if (DirTree_FindExact(Tree, Path) == NULL)
        return false;

    return true;
}

DirTree_Finder *DirTree_CreateFinder(DirTree *Tree, const char *Path) {
    DirTree_Finder *Finder = NULL;
    DirTree *SubTree = NULL;
    char Directory[_MAX_PATH] = {0};
    char Name[_MAX_FNAME] = {0};
    char Ext[_MAX_EXT] = {0};

    assert(Tree);
    assert(Path);

    _splitpath(Path, NULL, Directory, Name, Ext);

    SubTree = DirTree_FindExact(Tree, Directory);
    if (!SubTree)
        return NULL;

    Finder = malloc(sizeof(*Finder));
    if (!Finder)
        return Finder;

    Finder->MatchName = strdup(Name);
    if (!Finder->MatchName) {
        free(Finder);
        return NULL;
    }

    // The RTL leaves the '.' on there.  That won't do.
    if (*Ext == '.')
        Finder->MatchExt = strdup(&Ext[1]);
    else
        Finder->MatchExt = strdup(&Ext[0]);

    if (!Finder->MatchExt) {
        free(Finder->MatchName);
        free(Finder);
        return NULL;
    }

    Finder->Current = SubTree->Children;

    return Finder;
}

void DirTree_DestroyFinder(DirTree_Finder *Finder) {
    assert(Finder);
    assert(Finder->MatchName);
    assert(Finder->MatchExt);

    free(Finder->MatchName);
    free(Finder->MatchExt);
    free(Finder);
}

DirTree *DirTree_FinderGetNextFile(DirTree_Finder *Finder) {
    DirTree *Res = NULL;
    char Name[_MAX_FNAME] = {0};
    char Ext[_MAX_EXT] = {0};

    assert(Finder);

    Res = Finder->Current;

    if (!Res)
        return Res;

    do {
        _splitpath(Res->Name, NULL, NULL, Name, Ext);
        if (MatchPattern(Name, Finder->MatchName) == true && MatchPattern(Ext, Finder->MatchExt) == true) {
            break;
        }

        Res = Res->Siblings;

    } while (Res);

    if (Res)
        Finder->Current = Res->Siblings;

    return Res;
}

#ifdef DEBUG

static inline void indent(int i) {
    while (i--)
        printf(" ");
}

static void DirTree_Dump1(const DirTree *Tree, int i) {
    DirTree *Temp;

    indent(i);
    if (Tree->Attribute.b_isdir)
        printf("/%s\n", Tree->Name);
    else
        printf("%-*s  %-8u  %-8u\n", 40 - i, Tree->Name, Tree->Attribute.ul_fid, Tree->Attribute.ul_size);
    Temp = Tree->Children;
    while (Temp) {
        DirTree_Dump1(Temp, i + 2);
        Temp = Temp->Siblings;
    }
}

void DirTree_Dump(const DirTree *Tree) {
    printf("%-*s  %-8s  %-8s\n", 40, "Name", "Fid", "Size");
    printf("------------------------------------------------------------\n");
    DirTree_Dump1(Tree, 0);
}

#endif
