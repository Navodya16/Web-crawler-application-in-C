#include "pagedir.h"

bool pagedir_init(const char *pageDirectory)
{
    int retval = _mkdir(pageDirectory);

    if (retval == -1)
    {
        printf("make directory failed\n");
        if (errno == EEXIST)
            printf("directory already exixts\n");
        else if (errno == ENOENT)
            printf("path not found\n");
        return 1;
    }
    else
    {
        printf("make directory succeeded\n");
    }
    return 0;
}

void pagedir_save(const webpage_t *page, const char *pageDirectory, const int documentID)
{
    int length = snprintf(NULL, 0, "%s%s%d%s", pageDirectory, "\\\\", documentID, ".txt");
    char *result = (char *)malloc(length + 1);
    sprintf(result, "%s%s%d%s", pageDirectory, "\\\\", documentID, ".txt");
    printf("%s = %s\n", result, page->url);

    FILE *fp;
    fp = fopen(result, "w");
    fprintf(fp, "%s\n\n", page->url);
    fprintf(fp, "%d\n\n", page->depth);
    fprintf(fp, "%s\n\n", page->html);
    // fprintf(fp, "this was written using fprintf\n");
    fclose(fp);

    free(result);
}

