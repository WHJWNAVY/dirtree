# dirtree

> A dir tree data structure. 

## example

```c
#include "log.h"
#include "dirtree.h"

bool dirtreeforeacallback(const dirtree *tree, void *data) {
    printf("%s, %lu, %d\n", tree->name, tree->attribute.ul_fid, tree->attribute.b_isdir);
    return false;
}

int main(int argc, char *argv[]) {
    uint64_t fid = 0;
    fileattr attr = {0};
    char path[MAX_PATH + 1] = {0};
    dirtree *root = dirtree_create();

    LOG_DEBUG("======");
    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "/test1.txt", &attr);

    LOG_DEBUG("======");
    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "/test2.txt", &attr);

    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "/test1", &attr);

    LOG_DEBUG("======");
    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "/test1/t1-test1.txt", &attr);

    LOG_DEBUG("======");
    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "/test1/t1-test2.txt", &attr);

    LOG_DEBUG("======");
    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "/test2", &attr);

    LOG_DEBUG("======");
    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "/test2/t2-test1.txt", &attr);

    LOG_DEBUG("======");
    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "/test2/t2-test2.txt", &attr);

    LOG_DEBUG("======");
    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "/test2/t2-test1", &attr);

    LOG_DEBUG("======");
    dirtree *test1 = dirtree_find(root, "test1");
    dirtree *test2 = dirtree_find(root, "test2");
    dirtree *t2_test1 = dirtree_find(test2, "t2-test1");

    LOG_DEBUG("======");
    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(t2_test1, "t2-t1-test1", &attr);

    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "/test2/t2-test2/t2-t2-test1", &attr);
    LOG_DEBUG("======");

    LOG_DEBUG("======");
    dirtree_dump(root);

    LOG_DEBUG("===readdir /===");
    dirtree_readdir(root, dirtreeforeacallback, NULL);

    LOG_DEBUG("===readdir /test2===");
    dirtree_readdir(test2, dirtreeforeacallback, NULL);

    LOG_DEBUG("===foreach /===");
    dirtree_foreach(root, dirtreeforeacallback, NULL);

    LOG_DEBUG("===foreach /test2===");
    dirtree_foreach(test2, dirtreeforeacallback, NULL);

    LOG_DEBUG("===dump /test2===");
    dirtree_dump(test2);

    LOG_DEBUG("===fileexists[%d]===", dirtree_fileexists(test1, "t1-test3.txt"));
    LOG_DEBUG("===fileexists[%d]===", dirtree_fileexists(test2, "t2-test1.txt"));

    dirtree_getpath(t2_test1, path, sizeof(path) - 1);
    dirtree_remsub(t2_test1);
    LOG_DEBUG("===dump after remove %s===", path);
    dirtree_dump(root);

    dirtree_remove(root, dirtree_find(root, "test"));
    LOG_DEBUG("===dump after remove test===");
    dirtree_dump(root);

    dirtree_remove(root, test1);
    LOG_DEBUG("===dump after remove test1===");
    dirtree_dump(root);

    dirtree_destroy(root);
    return 0;
}
```



## output

```
(main:15) ======
(dirtree_add:255) path [test1.txt]
(main:21) ======
(dirtree_add:255) path [test2.txt]
(dirtree_add:255) path [test1]
(main:32) ======
(dirtree_findpartial:213) path[t1-test1.txt], buff[test1]
(dirtree_findpartial:213) path[], buff[t1-test1.txt]
(dirtree_add:248) tree [test1], leftovers[t1-test1.txt]
(dirtree_add:255) path [t1-test1.txt]
(main:38) ======
(dirtree_findpartial:213) path[t1-test2.txt], buff[test1]
(dirtree_findpartial:213) path[], buff[t1-test2.txt]
(dirtree_add:248) tree [test1], leftovers[t1-test2.txt]
(dirtree_add:255) path [t1-test2.txt]
(main:44) ======
(dirtree_add:255) path [test2]
(main:50) ======
(dirtree_findpartial:213) path[t2-test1.txt], buff[test2]
(dirtree_findpartial:213) path[], buff[t2-test1.txt]
(dirtree_add:248) tree [test2], leftovers[t2-test1.txt]
(dirtree_add:255) path [t2-test1.txt]
(main:56) ======
(dirtree_findpartial:213) path[t2-test2.txt], buff[test2]
(dirtree_findpartial:213) path[], buff[t2-test2.txt]
(dirtree_add:248) tree [test2], leftovers[t2-test2.txt]
(dirtree_add:255) path [t2-test2.txt]
(main:62) ======
(dirtree_findpartial:213) path[t2-test1], buff[test2]
(dirtree_findpartial:213) path[], buff[t2-test1]
(dirtree_add:248) tree [test2], leftovers[t2-test1]
(dirtree_add:255) path [t2-test1]
(main:68) ======
(main:73) ======
(dirtree_add:255) path [t2-t1-test1]
(dirtree_findpartial:213) path[t2-test2/t2-t2-test1], buff[test2]
(dirtree_findpartial:213) path[t2-t2-test1], buff[t2-test2]
(dirtree_add:248) tree [test2], leftovers[t2-test2/t2-t2-test1]
(main:83) ======
(main:85) ======
name                                      fid       size
------------------------------------------------------------
/                                         0         0
  test2                                   5         600
    t2-test1                              8         900
      t2-t1-test1                         9         1000
    t2-test2.txt                          7         800
    t2-test1.txt                          6         700
  test1                                   2         300
    t1-test2.txt                          4         500
    t1-test1.txt                          3         400
  test2.txt                               1         200
  test1.txt                               0         100
(main:88) ===readdir /===
test2, 5, 1
test1, 2, 1
test2.txt, 1, 0
test1.txt, 0, 0
(main:91) ===readdir /test2===
t2-test1, 8, 1
t2-test2.txt, 7, 0
t2-test1.txt, 6, 0
(main:94) ===foreach /===
test2, 5, 1
t2-test1, 8, 1
t2-t1-test1, 9, 1
t2-test2.txt, 7, 0
t2-test1.txt, 6, 0
test1, 2, 1
t1-test2.txt, 4, 0
t1-test1.txt, 3, 0
test2.txt, 1, 0
test1.txt, 0, 0
(main:97) ===foreach /test2===
t2-test1, 8, 1
t2-t1-test1, 9, 1
t2-test2.txt, 7, 0
t2-test1.txt, 6, 0
(main:100) ===dump /test2===
name                                      fid       size
------------------------------------------------------------
test2                                     5         600
  t2-test1                                8         900
    t2-t1-test1                           9         1000
  t2-test2.txt                            7         800
  t2-test1.txt                            6         700
(main:103) ===fileexists[0]===
(main:104) ===fileexists[1]===
(dirtree_getpath:436) node [/]
(dirtree_getpath:449) path [/]
(dirtree_getpath:436) node [test2]
(dirtree_getpath:449) path [/test2/]
(dirtree_getpath:436) node [t2-test1]
(dirtree_getpath:449) path [/test2/t2-test1/]
(main:108) ===dump after remove /test2/t2-test1/===
name                                      fid       size
------------------------------------------------------------
/                                         0         0
  test2                                   5         600
    t2-test1                              8         900
    t2-test2.txt                          7         800
    t2-test1.txt                          6         700
  test1                                   2         300
    t1-test2.txt                          4         500
    t1-test1.txt                          3         400
  test2.txt                               1         200
  test1.txt                               0         100
(dirtree_remove:344) assertion failed of [subtree]!
(main:112) ===dump after remove test===
name                                      fid       size
------------------------------------------------------------
/                                         0         0
  test2                                   5         600
    t2-test1                              8         900
    t2-test2.txt                          7         800
    t2-test1.txt                          6         700
  test1                                   2         300
    t1-test2.txt                          4         500
    t1-test1.txt                          3         400
  test2.txt                               1         200
  test1.txt                               0         100
(main:116) ===dump after remove test1===
name                                      fid       size
------------------------------------------------------------
/                                         0         0
  test2                                   5         600
    t2-test1                              8         900
    t2-test2.txt                          7         800
    t2-test1.txt                          6         700
  test2.txt                               1         200
  test1.txt                               0         100
```

