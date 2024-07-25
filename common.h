/****************************************************************************************/
/*  DIRTREE-COMMON.H                                                                    */
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
#ifndef DIRTREE_COMMON_H
#define DIRTREE_COMMON_H

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

void Splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);
const char *GetNextDir(const char *Path, char *Buff);
bool MatchPattern(const char *Source, const char *Pattern);
bool PathHasDir(const char *Path);

#endif