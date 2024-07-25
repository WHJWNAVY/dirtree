#include "log.h"
#include "dirtree.h"

void DirTreeForeaCallBack(const DirTree *Tree) {
    printf("%s, %lu, %d\n", Tree->Name, Tree->Attribute.ul_fid, Tree->Attribute.b_isdir);
}

int main(int argc, char *argv[]) {
    FileAttr attr = {0};
    DirTree *root = DirTree_Create();
    DirTree_AddFile(root, "test1.txt", false);
    DirTree_AddFile(root, "test2.txt", false);

    DirTree_AddFile(root, "test1", true);
    DirTree_AddFile(root, "test1/test1.txt", false);
    DirTree_AddFile(root, "test1/test2.txt", false);

    DirTree_AddFile(root, "test2", true);
    DirTree_AddFile(root, "test2/test1.txt", false);
    DirTree_AddFile(root, "test2/test2.txt", false);

    DirTree_AddFile(root, "test2/test1", true);
    DirTree_AddFile(root, "test2/test1/test1", true);
    DirTree_AddFile(root, "test2/test2/test1", true);

    attr.ul_fid = 1;
    attr.ul_size = 100;
    DirTree_SetFileAttr(DirTree_FindExact(root, "test1/test1.txt"), &attr);

    attr.ul_fid = 2;
    attr.ul_size = 200;
    DirTree_SetFileAttr(DirTree_FindExact(root, "test2/test2.txt"), &attr);

    attr.ul_fid = 3;
    attr.ul_size = 300;
    DirTree_SetFileAttr(DirTree_FindExact(root, "test2/test1.txt"), &attr);

    attr.ul_fid = 4;
    attr.ul_size = 400;
    DirTree_SetFileAttr(DirTree_FindExact(root, "test2/test2.txt"), &attr);

    LOG_DEBUG("======");
    DirTreeDump(root);

    LOG_DEBUG("===Foreach===");
    DirTreeForeach(root, DirTreeForeaCallBack);


    LOG_DEBUG("===Dump /test2===");
    DirTreeDump(DirTree_FindExact(root, "test2"));

    LOG_DEBUG("===FileExists[%d]===", DirTree_FileExists(DirTree_FindExact(root, "test1"), "test3.txt"));
    LOG_DEBUG("===FileExists[%d]===", DirTree_FileExists(DirTree_FindExact(root, "test2"), "test1.txt"));

    DirTree_Remove(root, DirTree_FindExact(root, "test2/test1"));
    LOG_DEBUG("===Dump after remove test2/test1===");
    DirTreeDump(root);

    DirTree_Remove(root, DirTree_FindExact(root, "test"));
    LOG_DEBUG("===Dump after remove test===");
    DirTreeDump(root);

    DirTree_Remove(root, DirTree_FindExact(root, "test1"));
    LOG_DEBUG("===Dump after remove test1===");
    DirTreeDump(root);

    DirTree_Remove(root, DirTree_FindExact(root, ""));
    LOG_DEBUG("===Dump after remove /===");
    DirTreeDump(root);

    DirTree_Destroy(root);
    return 0;
}