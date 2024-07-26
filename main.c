#include "log.h"
#include "dirtree.h"

bool DirTreeForeaCallBack(const DirTree *Tree, void *Data) {
    printf("%s, %lu, %d\n", Tree->Name, Tree->Attribute.ul_fid, Tree->Attribute.b_isdir);
    return false;
}

int main(int argc, char *argv[]) {
    uint64_t fid = 0;
    FileAttr attr = {0};
    DirTree *root = DirTree_Create();

    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    DirTree_Add(root, "test1.txt", &attr);

    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    DirTree_Add(root, "test2.txt", &attr);

    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    DirTree_Add(root, "test1", &attr);

    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    DirTree_Add(root, "test1/t1-test1.txt", &attr);

    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    DirTree_Add(root, "test1/t1-test2.txt", &attr);

    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    DirTree_Add(root, "test2", &attr);

    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    DirTree_Add(root, "test2/t2-test1.txt", &attr);

    attr.b_isdir = false;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    DirTree_Add(root, "test2/t2-test2.txt", &attr);

    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    DirTree_Add(root, "test2/t2-test1", &attr);

    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    DirTree_Add(root, "test2/t2-test1/t2-t1-test1", &attr);

    attr.b_isdir = true;
    attr.ul_fid = fid;
    attr.ul_size = (++fid) * 100;
    DirTree_Add(root, "test2/t2-test2/t2-t2-test1", &attr);

    DirTree *test1 = DirTree_FindExact(root, "test1");
    DirTree *test2 = DirTree_FindExact(root, "test2");
    DirTree *t2_test1 = DirTree_FindExact(root, "t2-test1");

    LOG_DEBUG("======");
    DirTree_Dump(root);

    LOG_DEBUG("===ReadDir /===");
    DirTree_ReadDir(root, DirTreeForeaCallBack, NULL);

    LOG_DEBUG("===ReadDir /test2===");
    DirTree_ReadDir(test2, DirTreeForeaCallBack, NULL);

    LOG_DEBUG("===Foreach /===");
    DirTree_Foreach(root, DirTreeForeaCallBack, NULL);

    LOG_DEBUG("===Foreach /test2===");
    DirTree_Foreach(test2, DirTreeForeaCallBack, NULL);

    LOG_DEBUG("===Dump /test2===");
    DirTree_Dump(test2);

    LOG_DEBUG("===FileExists[%d]===", DirTree_FileExists(test1, "t1-test3.txt"));
    LOG_DEBUG("===FileExists[%d]===", DirTree_FileExists(test2, "t2-test1.txt"));

    DirTree_Remove(root, t2_test1);
    LOG_DEBUG("===Dump after remove test2/t2-test1===");
    DirTree_Dump(root);

    DirTree_Remove(root, DirTree_FindExact(root, "test"));
    LOG_DEBUG("===Dump after remove test===");
    DirTree_Dump(root);

    DirTree_Remove(root, test1);
    LOG_DEBUG("===Dump after remove test1===");
    DirTree_Dump(root);

    DirTree_Destroy(root);
    return 0;
}