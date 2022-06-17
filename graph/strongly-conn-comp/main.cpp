#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include "graph.h"

int main()
{
    int choice;

    printf("\nChoose graph to load:\n1. An undirected graph\n2. An directed acyclic graph\n3. The course's directed graph\n");
    scanf("%i",&choice);

    Graph graph;

    switch(choice)
    {
        case 1:
            graph.initialise("graph_undir.txt",false);

            printf("\nEnter starting node (1-%i):\n",graph.num_nodes);
            scanf("%i",&choice);

            graph.exploreBFS_and_distance(choice);
            graph.exploreDFS1(choice);
            graph.exploreDFS2(choice);
            graph.find_conn_components();
            graph.print1();
            
            break;

        case 2:
            graph.initialise("graph_acyclic_dir.txt",true);

            printf("\nEnter starting node (1-%i):\n",graph.num_nodes);
            scanf("%i",&choice);

            graph.exploreBFS(choice);
            graph.exploreDFS1(choice);
            graph.exploreDFS2(choice);
            graph.find_topological_order();
            graph.print2();

            break;

        case 3:
            graph.initialise("SCC.txt",true);

            graph.find_scc();
            graph.print3();

            break;
    }

    return EXIT_SUCCESS;
}