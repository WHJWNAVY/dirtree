#include "log.h"
#include "dirtree.h"

#define ASSERTN(_x)                                  \
    if (!(_x)) {                                     \
        LOG_DEBUG("assertion failed of [%s]!", #_x); \
        return;                                      \
    }

#define ASSERTV(_x, _v)                              \
    if (!(_x)) {                                     \
        LOG_DEBUG("assertion failed of [%s]!", #_x); \
        return (_v);                                 \
    }

#if 0
static void splitwholename(const char *whole_name, char *fname, char *ext) {
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
static void splitpath(const char *path, char *drive, char *dir, char *fname, char *ext) {
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
        splitwholename(p_whole_name, fname, ext);
        snprintf(dir, p_whole_name - path, "%s", path);
    } else {
        splitwholename(path, fname, ext);
    }
}
#endif

static const char *getnextdir(const char *path, char *buff) {
    while (*path && *path != '/') {
        *buff++ = *path++;
    }
    *buff = '\0';

    if (*path == '/') {
        path++;
    }

    return path;
}

#if 0
static bool matchpattern(const char *source, const char *pattern) {
    assert(source);
    assert(pattern);

    switch (*pattern) {
        case '\0':
            if (*source)
                return false;
            break;

        case '*':
            if (*(pattern + 1) != '\0') {
                pattern++;
                while (*source) {
                    if (matchpattern(source, pattern) == true)
                        return true;
                    source++;
                }
                return false;
            }
            break;

        case '?':
            return matchpattern(source + 1, pattern + 1);

        default:
            if (*source == *pattern)
                return matchpattern(source + 1, pattern + 1);
            else
                return false;
    }

    return true;
}
#endif

static bool pathhasdir(const char *path) {
    return (strchr(path, '/'));
}

dirtree *dirtree_create(void) {
    dirtree *tree = NULL;

    tree = malloc(sizeof(*tree));
    if (!tree) {
        return tree;
    }

    memset(tree, 0, sizeof(*tree));
    tree->name = strdup("/");
    if (!tree->name) {
        free(tree);
        return NULL;
    }

    DIRTREE_SETDIR(tree);

    return tree;
}

void dirtree_destroy(dirtree *tree) {
    ASSERTN(tree);
    assert(tree->name);

    if (tree->children) {
        dirtree_destroy(tree->children);
    }

    if (tree->siblings) {
        dirtree_destroy(tree->siblings);
    }

    free(tree->name);
    free(tree);
}

dirtree *dirtree_find(const dirtree *tree, const char *path) {
    static char buff[MAX_PATH] = {0};
    dirtree *siblings = NULL;

    ASSERTV(tree, NULL);
    ASSERTV(path, NULL);

    if (*path == '/') {
        // return NULL;
        path++;
    }

    if (*path == '\0') {
        return (dirtree *)tree;
    }

    path = getnextdir(path, buff);

    siblings = tree->children;
    while (siblings) {
        if (!strcmp(siblings->name, buff)) {
            if (!*path) {
                return siblings;
            }
            return dirtree_find(siblings, path);
        }
        siblings = siblings->siblings;
    }

    return NULL;
}

dirtree *dirtree_findpartial(const dirtree *tree, const char *path, const char **leftovers) {
    static char buff[MAX_PATH] = {0};
    dirtree *siblings = NULL;

    ASSERTV(tree, NULL);
    ASSERTV(path, NULL);
    ASSERTV(leftovers, NULL);

    if (*path == '/') {
        // return NULL;
        path++;
    }

    *leftovers = path;

    if (*path == '\0') {
        return (dirtree *)tree;
    }

    path = getnextdir(path, buff);
    LOG_DEBUG("path[%s], buff[%s]", path, buff);

    siblings = tree->children;
    while (siblings) {
        if (!strcmp(siblings->name, buff)) {
            *leftovers = path;
            if (!*path) {
                return siblings;
            }
            return dirtree_findpartial(siblings, path, leftovers);
        }
        siblings = siblings->siblings;
    }

    return (dirtree *)tree;
}

dirtree *dirtree_add(dirtree *tree, const char *path, fileattr *attributes) {
    dirtree *newentry = NULL;
    const char *leftovers = NULL;

    ASSERTV(tree, NULL);
    ASSERTV(path, NULL);
    ASSERTV(attributes, NULL);
    ASSERTV((strlen(path) > 0), NULL);

    if (*path == '/') {
        path++;
    }

    if (pathhasdir(path)) {
        tree = dirtree_findpartial(tree, path, &leftovers);
        if (!tree) {
            return NULL;
        }
        LOG_DEBUG("tree [%s], leftovers[%s]", tree->name, leftovers);
        if (pathhasdir(leftovers)) {
            return NULL;
        }

        path = leftovers;
    }
    LOG_DEBUG("path [%s]", path);

    newentry = malloc(sizeof(*newentry));
    if (!newentry) {
        return NULL;
    }

    memset(newentry, 0, sizeof(*newentry));
    newentry->name = strdup(path);
    if (!newentry->name) {
        free(newentry->name);
        free(newentry);
        return NULL;
    }

    newentry->siblings = tree->children;
    tree->children = newentry;
    newentry->parent = tree;

    if (attributes->b_isdir) {
        DIRTREE_SETDIR(newentry);
    }
    memcpy(&(newentry->attribute), attributes, sizeof(fileattr));

    return newentry;
}

dirtree *dirtree_addfile(dirtree *tree, const char *path, bool isdirectory) {
#if 0
    dirtree *newentry = NULL;
    const char *leftovers = NULL;

    ASSERTV(tree, NULL);
    ASSERTV(path, NULL);
    ASSERTV((strlen(path) > 0), NULL);

    if (pathhasdir(path)) {
        tree = dirtree_findpartial(tree, path, &leftovers);
        if (!tree) {
            return NULL;
        }

        if (pathhasdir(leftovers)) {
            return NULL;
        }

        path = leftovers;
    }

    newentry = malloc(sizeof(*newentry));
    if (!newentry) {
        return NULL;
    }

    memset(newentry, 0, sizeof(*newentry));
    newentry->name = strdup(path);
    if (!newentry->name) {
        free(newentry->name);
        free(newentry);
        return NULL;
    }

    newentry->siblings = tree->children;
    tree->children = newentry;
    newentry->parent = tree;

    if (isdirectory == true) {
        DIRTREE_SETDIR(newentry);
    }

    return newentry;
#else
    fileattr attr = {0};
    if (isdirectory) {
        FILEATTR_SETDIR(&attr);
    } else {
        FILEATTR_SETFILE(&attr);
    }
    return dirtree_add(tree, path, &attr);
#endif
}

bool dirtree_remove(dirtree *tree, dirtree *subtree) {
    dirtree siblings = {0};
    dirtree *psiblings = NULL;
    dirtree *parent = NULL;
    dirtree *paranoiacheck = NULL;

    ASSERTV(tree, false);
    ASSERTV(subtree, false);

    parent = subtree->parent;
    ASSERTV(parent, false);

    paranoiacheck = parent;
    while (paranoiacheck && paranoiacheck != tree) {
        paranoiacheck = paranoiacheck->parent;
    }
    if (!paranoiacheck) {
        return false;
    }

    siblings.siblings = parent->children;
    ASSERTV(siblings.siblings, false);
    psiblings = &siblings;
    while (psiblings->siblings) {
        if (psiblings->siblings == subtree) {
            psiblings->siblings = subtree->siblings;
            if (subtree == parent->children) {
                parent->children = subtree->siblings;
            }
            subtree->siblings = NULL;
            dirtree_destroy(subtree);
            return true;
        }
        psiblings = psiblings->siblings;
    }

    ASSERTV(!"shouldn't be a way to get here", false);
    return false;
}

static bool dirtree_remove_subtree_foreachcb(dirtree *subtree, dirtree *tree) {
    dirtree_remove(tree, subtree);
    return false;
}

bool dirtree_remsub(dirtree *tree) {
    dirtree_readdir(tree, dirtree_remove_subtree_foreachcb, tree);
    return true;
}

void dirtree_setattr(dirtree *tree, fileattr *attributes) {
    ASSERTN(tree);
    ASSERTN(attributes);

    memcpy(&(tree->attribute), attributes, sizeof(fileattr));
}

void dirtree_getattr(dirtree *tree, fileattr *attributes) {
    ASSERTN(tree);
    ASSERTN(attributes);

    memcpy(attributes, &(tree->attribute), sizeof(fileattr));
}

bool dirtree_rename(dirtree *tree, const char *path) {
    char *name = NULL;
    ASSERTV(tree, false);
    ASSERTV(path, false);

    name = strrchr(path, '/');
    if (name) {
        name += 1;
    } else {
        name = path;
    }

    if ((name == NULL) || (*name == '\0')) {
        return false;
    }

    if (tree->name) {
        free(tree->name);
    }
    tree->name = strdup(name);

    return true;
}

bool dirtree_getname(dirtree *tree, char *buff, size_t maxlen) {
    size_t length = 0;

    ASSERTV(tree, false);
    ASSERTV(buff, false);
    ASSERTV((maxlen > 0), false);

    maxlen -= 1; // for '\0'
    length = strlen(tree->name);
    length = ((length > maxlen) ? maxlen : length);

    memcpy(buff, tree->name, length);
    *(buff + length) = '\0';

    return true;
}

bool dirtree_getpath(dirtree *tree, char *buff, size_t maxlen) {
    char *pbuf = NULL;
    size_t blen = 0;
    maxlen -= 1; // for '\0'

    dirtree *root = NULL;
    dirtree *node = NULL;
    ASSERTV(tree, false);
    ASSERTV(buff, false);
    ASSERTV((maxlen > 0), false);

    pbuf = buff;
    while ((root != tree) && (maxlen > 0)) {
        node = tree;
        while (node->parent != root) {
            node = node->parent;
        }

        LOG_DEBUG("node [%s]", node->name);

        blen = strlen(node->name);
        strncpy(pbuf, node->name, maxlen);
        pbuf += blen;
        maxlen -= blen;

        if ((DIRTREE_ISDIR(node)) && (*(pbuf - 1) != '/')) {
            *pbuf = '/';
            pbuf += 1;
            maxlen -= 1;
        }

        LOG_DEBUG("path [%s]", buff);

        root = node;
    }

    *pbuf = '\0';

    return true;
}

bool dirtree_fileexists(const dirtree *tree, const char *path) {
    return (dirtree_find(tree, path) != NULL);
}

#if 0
dirtree_finder *dirtree_createfinder(dirtree *tree, const char *path) {
    dirtree_finder *finder = NULL;
    dirtree *subtree = NULL;
    char directory[MAX_PATH] = {0};
    char name[MAX_FNAME] = {0};
    char ext[MAX_EXT] = {0};

    ASSERTV(tree, NULL);
    ASSERTV(path, NULL);

    splitpath(path, NULL, directory, name, ext);
    LOG_DEBUG("===dir[%s], name[%s], ext[%s]===", directory, name, ext);
    subtree = dirtree_find(tree, directory);
    if (!subtree) {
        return NULL;
    }

    finder = malloc(sizeof(*finder));
    if (!finder) {
        return finder;
    }

    finder->matchname = strdup(name);
    if (!finder->matchname) {
        free(finder);
        return NULL;
    }

    // the rtl leaves the '.' on there.  that won't do.
    if (*ext == '.') {
        finder->matchext = strdup(&ext[1]);
    } else {
        finder->matchext = strdup(&ext[0]);
    }

    if (!finder->matchext) {
        free(finder->matchname);
        free(finder);
        return NULL;
    }

    finder->current = subtree->children;

    return finder;
}

void dirtree_destroyfinder(dirtree_finder *finder) {
    ASSERTN(finder);
    ASSERTN(finder->matchname);
    ASSERTN(finder->matchext);

    free(finder->matchname);
    free(finder->matchext);
    free(finder);
}

dirtree *dirtree_findergetnextfile(dirtree_finder *finder) {
    dirtree *res = NULL;
    char name[MAX_FNAME] = {0};
    char ext[MAX_EXT] = {0};

    ASSERTV(finder, NULL);

    res = finder->current;

    if (!res) {
        return res;
    }

    do {
        splitpath(res->name, NULL, NULL, name, ext);
        LOG_DEBUG("===name[%s], ext[%s]===", name, ext);
        if (matchpattern(name, finder->matchname) == true && matchpattern(ext, finder->matchext) == true) {
            break;
        }
        res = res->siblings;
    } while (res);

    if (res) {
        finder->current = res->siblings;
    }

    return res;
}
#endif

void dirtree_readdir(const dirtree *tree, dirtreeforeachcb callback, void *data) {
    dirtree *temp = NULL;
    ASSERTN(tree);
    ASSERTN(callback);

    temp = tree->children;
    while (temp) {
        if (callback(temp, data)) {
            break;
        }
        temp = temp->siblings;
    }
}

static void dirtree_foreachr(const dirtree *tree, dirtreeforeachcb callback, void *data) {
    dirtree *temp = NULL;
    ASSERTN(tree);
    ASSERTN(callback);

    tree = tree->children;
    while (tree) {
        callback(tree, data);
        dirtree_foreachr(tree, callback, data);
        tree = tree->siblings;
    }
}

void dirtree_foreach(const dirtree *tree, dirtreeforeachcb callback, void *data) {
#if 0
    ASSERTN(tree);
    ASSERTN(callback);

    callback(tree, data);

    tree = tree->children;
    while (tree) {
        dirtree_foreach(tree, callback, data);
        tree = tree->siblings;
    }
#else
    ASSERTN(tree);
    ASSERTN(callback);

    dirtree_foreachr(tree, callback, data);
#endif
}

#ifdef DIRTREE_DEBUG
static inline void indentline(int i) {
    while (i--) {
        printf(" ");
    }
}

static void dirtree_dumpr(const dirtree *tree, int i) {
    dirtree *temp = NULL;
    ASSERTN(tree);

    indentline(i);
    // if (DIRTREE_ISDIR(tree)) {
    //     printf("/%-*s  %-8lu\n", 39 - i, tree->name, tree->attribute.ul_fid);
    // } else {
    printf("%-*s  %-8lu  %-8lu\n", 40 - i, tree->name, tree->attribute.ul_fid, tree->attribute.ul_size);
    // }
    temp = tree->children;
    while (temp) {
        dirtree_dumpr(temp, i + 2);
        temp = temp->siblings;
    }
}

void dirtree_dump(const dirtree *tree) {
    ASSERTN(tree);

    printf("%-*s  %-8s  %-8s\n", 40, "name", "fid", "size");
    printf("------------------------------------------------------------\n");
    dirtree_dumpr(tree, 0);
}

#endif
