#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <string>
#include <vector>
#include <linux/limits.h>
#include <libgen.h>
#include "binq.h"
#include "bint.h"

int main(int argc, char **argv)
{
    char stanet[32] = {0};
    char stanetcommand[] = "sta=";

    if (argc <  2)
    {
        printf("\nUsage: %s <state file name (.bint/.binq)>\n", argv[0]);
        return -1;
    }


    char cPath[PATH_MAX], fullPath[PATH_MAX];
    strcpy(cPath, argv[1]);
    strcpy(fullPath, argv[1]);

    int PathLen = strlen(fullPath);
    if (PathLen < 6)
    {
        printf("Usage: %s <state file name (.bint/.binq)>", argv[0]);
        return -1;
    }

    if(argc>2)
    {
       if( strncmp(argv[2],stanetcommand,strlen(stanetcommand))==0)
       {
           strcpy(stanet, argv[2]+strlen(stanetcommand));
       }
    }


    if ((strcmp(&fullPath[PathLen - 5], ".bint") != 0)
            && (strcmp(&fullPath[PathLen - 5], ".binq") != 0))
    {
        printf("\nFile extension should be .bint or .binq");
        return -1;
    }

    if (strcmp(&fullPath[PathLen - 5], ".binq") == 0)
    {
        return binq32to64(fullPath, stanet);
    }

    if (strcmp(&fullPath[PathLen - 5], ".bint") == 0)
    {
        return bint32to64(fullPath, stanet);
    }

}

