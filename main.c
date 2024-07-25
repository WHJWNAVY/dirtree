#include "log.h"
#include "dirtree.h"

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

    DirTree_Dump(root);

    DirTree_Destroy(root);
    return 0;
}