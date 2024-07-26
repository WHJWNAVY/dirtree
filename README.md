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
    dirtree *root = dirtree_create();

    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "test1.txt", &attr);

    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "test2.txt", &attr);

    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "test1", &attr);

    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "test1/t1-test1.txt", &attr);

    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "test1/t1-test2.txt", &attr);

    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "test2", &attr);

    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "test2/t2-test1.txt", &attr);

    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "test2/t2-test2.txt", &attr);

    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "test2/t2-test1", &attr);

    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "test2/t2-test1/t2-t1-test1", &attr);

    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    dirtree_add(root, "test2/t2-test2/t2-t2-test1", &attr);

    dirtree *test1 = dirtree_find(root, "test1");
    dirtree *test2 = dirtree_find(root, "test2");
    dirtree *t2_test1 = dirtree_find(root, "t2-test1");

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

    dirtree_remove(root, t2_test1);
    LOG_DEBUG("===dump after remove test2/t2-test1===");
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
(main:73) ======
name                                      fid       size
------------------------------------------------------------
/                                         0
  /test2                                  5
    /t2-test1                             8
      /t2-t1-test1                        9
    t2-test2.txt                          7         800
    t2-test1.txt                          6         700
  /test1                                  2
    t1-test2.txt                          4         500
    t1-test1.txt                          3         400
  test2.txt                               1         200
  test1.txt                               0         100
(main:76) ===readdir /===
test2, 5, 1
test1, 2, 1
test2.txt, 1, 0
test1.txt, 0, 0
(main:79) ===readdir /test2===
t2-test1, 8, 1
t2-test2.txt, 7, 0
t2-test1.txt, 6, 0
(main:82) ===foreach /===
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
(main:85) ===foreach /test2===
t2-test1, 8, 1
t2-t1-test1, 9, 1
t2-test2.txt, 7, 0
t2-test1.txt, 6, 0
(main:88) ===dump /test2===
name                                      fid       size
------------------------------------------------------------
/test2                                    5
  /t2-test1                               8
    /t2-t1-test1                          9
  t2-test2.txt                            7         800
  t2-test1.txt                            6         700
(main:91) ===fileexists[0]===
(main:92) ===fileexists[1]===
(dirtree_remove:337) assertion failed of [subtree]!
(main:95) ===dump after remove test2/t2-test1===
name                                      fid       size
------------------------------------------------------------
/                                         0
  /test2                                  5
    /t2-test1                             8
      /t2-t1-test1                        9
    t2-test2.txt                          7         800
    t2-test1.txt                          6         700
  /test1                                  2
    t1-test2.txt                          4         500
    t1-test1.txt                          3         400
  test2.txt                               1         200
  test1.txt                               0         100
(dirtree_remove:337) assertion failed of [subtree]!
(main:99) ===dump after remove test===
name                                      fid       size
------------------------------------------------------------
/                                         0
  /test2                                  5
    /t2-test1                             8
      /t2-t1-test1                        9
    t2-test2.txt                          7         800
    t2-test1.txt                          6         700
  /test1                                  2
    t1-test2.txt                          4         500
    t1-test1.txt                          3         400
  test2.txt                               1         200
  test1.txt                               0         100
(main:103) ===dump after remove test1===
name                                      fid       size
------------------------------------------------------------
/                                         0
  /test2                                  5
    /t2-test1                             8
      /t2-t1-test1                        9
    t2-test2.txt                          7         800
    t2-test1.txt                          6         700
  test2.txt                               1         200
  test1.txt                               0         100
```

