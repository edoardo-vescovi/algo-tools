#include <stdio.h>
#include "bloom_filter.h"

#define MAX_CHAR_PER_LINE 100

int main()
{
    //load data from file
    FILE *file=fopen("latex_colors.txt","r");
    std::vector<std::string> colors;
    char *s=(char *)malloc(MAX_CHAR_PER_LINE*sizeof(char));
    while(fscanf(file,"%s\n",s)==1)
    {
        colors.push_back(s);
    }
    fclose(file);

    //input data in Bloom filter
    Bloom_filter bloom;
    bloom.initialise(100,20);
    int i;
    for(i=0;i<colors.size();i++)
    {
        bloom.insert(colors[i]);
    }

    // look up and print
    printf("\nColors\t\tPresent?\n");
    for(i=0;i<colors.size();i++)
    {
        printf("%s\t\t",colors[i].c_str());

        if(bloom.lookup(colors[i]))
        {
            printf("yes\n");
        }
        else
        {
            printf("no\n");
        }
    }

    // //choose a color and print
    printf("\nEnter a color: ");
    fscanf(stdin,"%s",s);
    std::vector<std::string> color;
    color.push_back(s);
    if(bloom.lookup(color[0]))
    {
        printf("Present? yes\n\n");
    }
    else
    {
        printf("Present? no\n\n");
    }

    free(s);

    return EXIT_SUCCESS;
}