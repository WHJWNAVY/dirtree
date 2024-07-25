/****************************************************************************************/
/*  DIRTREE-COMMON.C                                                                    */
/*                                                                                      */
/*  Author: Samuel Seay                                                                 */
/*  Description: Common functions between Dirtree files                                 */
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
/*  Genesis3D Version 1.1 released November 15, 1999                                    */
/*  Copyright (C) 1999 WildTangent, Inc. All Rights Reserved                            */
/*                                                                                      */
/****************************************************************************************/

#include <assert.h>
#include <string.h>

#include "common.h"

static void _split_whole_name(const char *whole_name, char *fname, char *ext) {
    char *p_ext = NULL;

    if (whole_name == NULL) {
        return;
    }

    p_ext = rindex(whole_name, '.');
    if (NULL != p_ext) {
        if (ext != NULL) {
            strcpy(ext, p_ext);
        }
        if (fname != NULL) {
            snprintf(fname, p_ext - whole_name + 1, "%s", whole_name);
        }
    } else {
        if (fname != NULL) {
            strcpy(fname, whole_name);
        }
    }
}
void Splitpath(const char *path, char *drive, char *dir, char *fname, char *ext) {
    char *p_whole_name = NULL;

    if (dir != NULL) {
        dir[0] = '\0';
    }
    if (fname != NULL) {
        fname[0] = '\0';
    }
    if (ext != NULL) {
        ext[0] = '\0';
    }

    if (NULL == path) {
        return;
    }

    if ('/' == path[strlen(path)]) {
        strcpy(dir, path);
        return;
    }

    p_whole_name = rindex(path, '/');
    if (NULL != p_whole_name) {
        p_whole_name++;
        _split_whole_name(p_whole_name, fname, ext);

        snprintf(dir, p_whole_name - path, "%s", path);
    } else {
        _split_whole_name(path, fname, ext);
    }
}

const char *GetNextDir(const char *Path, char *Buff) {
    while (*Path && *Path != '/') {
        *Buff++ = *Path++;
    }
    *Buff = '\0';

    if (*Path == '/') {
        Path++;
    }

    return Path;
}

bool MatchPattern(const char *Source, const char *Pattern) {
    assert(Source);
    assert(Pattern);

    switch (*Pattern) {
        case '\0':
            if (*Source)
                return false;
            break;

        case '*':
            if (*(Pattern + 1) != '\0') {
                Pattern++;
                while (*Source) {
                    if (MatchPattern(Source, Pattern) == true)
                        return true;
                    Source++;
                }
                return false;
            }
            break;

        case '?':
            return MatchPattern(Source + 1, Pattern + 1);

        default:
            if (*Source == *Pattern)
                return MatchPattern(Source + 1, Pattern + 1);
            else
                return false;
    }

    return true;
}

bool PathHasDir(const char *Path) {
    if (strchr(Path, '/'))
        return true;

    return false;
}
