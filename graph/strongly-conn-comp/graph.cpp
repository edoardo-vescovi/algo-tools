#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "graph.h"
#include "buffer.h"

int min(int x, int y)
{
    if(x<y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

int max(int x, int y)
{
    if(x>y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

void Graph::initialise(const char *namefile, bool dir)
{
    //set directed
    directed=dir;

    FILE *file=fopen(namefile,"r");

    int tail,head,edge,deg;

    //fill incidence_list and read num_edges
    incidence_list_tail=NULL;
    incidence_list_head=NULL;
    num_edges=0;
    while(fscanf(file,"%i %i ",&tail,&head)==2)
    {
        num_edges++;
        incidence_list_tail=(int *)realloc(incidence_list_tail,num_edges*sizeof(int));
        incidence_list_head=(int *)realloc(incidence_list_head,num_edges*sizeof(int));
        if(directed)
        {
            incidence_list_tail[num_edges-1]=tail;
            incidence_list_head[num_edges-1]=head;
        }
        else
        {
            incidence_list_tail[num_edges-1]=min(tail,head);
            incidence_list_head[num_edges-1]=max(tail,head);
        }

    }
    rewind(file);

    //read num_nodes, assuming nodes are labeled 1,2,... num_nodes
    num_nodes=0;
    for(edge=0;edge<num_edges;edge++)
    {
        if(incidence_list_tail[edge]>num_nodes)
        {
            num_nodes=incidence_list_tail[edge];
        }
        if(incidence_list_head[edge]>num_nodes)
        {
            num_nodes=incidence_list_head[edge];
        }
    }
    fclose(file);

    //read degrees and fill adjacency_list
    degrees=(int *)calloc(num_nodes,sizeof(int));
    adjacency_list=(int **)malloc(num_nodes*sizeof(int *));
    for(tail=0;tail<num_nodes;tail++)
    {
        adjacency_list[tail]=NULL;
    }
    for(edge=0;edge<num_edges;edge++)
    {
        tail=incidence_list_tail[edge];
        head=incidence_list_head[edge];

        degrees[tail-1]++;
        deg=degrees[tail-1];
        adjacency_list[tail-1]=(int *)realloc(adjacency_list[tail-1],deg*sizeof(int));
        adjacency_list[tail-1][deg-1]=head;

        if(!directed)
        {
            degrees[head-1]++;
            deg=degrees[head-1];
            adjacency_list[head-1]=(int *)realloc(adjacency_list[head-1],deg*sizeof(int));
            adjacency_list[head-1][deg-1]=tail;
        }
    }

    if(directed)
    {
        degrees_inv=(int *)calloc(num_nodes,sizeof(int));
        adjacency_list_inv=(int **)malloc(num_nodes*sizeof(int *));
        for(tail=0;tail<num_nodes;tail++)
        {
            adjacency_list_inv[tail]=NULL;
        }
        for(edge=0;edge<num_edges;edge++)
        {
            tail=incidence_list_head[edge];
            head=incidence_list_tail[edge];

            degrees_inv[tail-1]++;
            deg=degrees_inv[tail-1];
            adjacency_list_inv[tail-1]=(int *)realloc(adjacency_list_inv[tail-1],deg*sizeof(int));
            adjacency_list_inv[tail-1][deg-1]=head;
        }
    }

    //allocate variables for (un)directed graph
    explored_nodes=(bool *)malloc(num_nodes*sizeof(bool));
    order_first_exploration_BFS=(int *)malloc(num_nodes*sizeof(int));
    order_first_exploration_DFS1=(int *)malloc(num_nodes*sizeof(int));
    order_first_exploration_DFS2=(int *)malloc(num_nodes*sizeof(int));

    if(directed)
    {
        distances=NULL;
        conn_components=NULL;
        topological_order=(int *)malloc(num_nodes*sizeof(int));
        scc=(int *)malloc(num_nodes*sizeof(int));
    }
    else
    {
        distances=(int *)malloc(num_nodes*sizeof(int));
        conn_components=(int *)malloc(num_nodes*sizeof(int));
        topological_order=NULL;
        scc=NULL;
    }
}

Graph::~Graph()
{
    free(explored_nodes);
    free(order_first_exploration_BFS);
    free(order_first_exploration_DFS1);
    free(order_first_exploration_DFS2);

    int i;
    for(i=0;i<num_nodes;i++)
    {
        free(adjacency_list[i]);
    }
    free(adjacency_list);
    if(directed)
    {
        for(i=0;i<num_nodes;i++)
        {
            free(adjacency_list_inv[i]);
        }
        free(adjacency_list_inv);
    }

    free(incidence_list_tail);
    free(incidence_list_head);
    free(degrees);

    free(distances);
    free(conn_components);
    free(topological_order);
    free(scc);
}

void Graph::exploreBFS(int start_node)
{
    int node,adj_node,edge,exploration_counter;

    //start_node explored, all other nodes unexplored
    for(node=0;node<num_nodes;node++)
    {
        explored_nodes[node]=false;
    }
    explored_nodes[start_node-1]=true;

    for(node=0;node<num_nodes;node++)
    {
        order_first_exploration_BFS[node]=INT_MAX;
    }

    //put start_node in queue
    Queue queue;
    queue.initialise(num_nodes);
    queue.enqueue(start_node);
    
    //queue contains explored nodes
    //...as long as queue exists
    exploration_counter=0;
    while(!(queue.isEmpty()))
    {
        //...take a node from queue
        node=queue.dequeue();
        exploration_counter++;
        order_first_exploration_BFS[node-1]=exploration_counter;
        //...run over adjacent nodes
        for(edge=0;edge<degrees[node-1];edge++)
        {
            adj_node=adjacency_list[node-1][edge];
            //...is it explored?
            //...if yes: do nothing
            //...if no: mark explored and put in queue
            if(!(explored_nodes[adj_node-1]))
            {
                explored_nodes[adj_node-1]=true;
                queue.enqueue(adj_node);
            }
        }

    }
}

void Graph::exploreBFS_and_distance(int start_node)
{
    if(directed)
    {
        return;
    }

    int node,adj_node,edge,exploration_counter;

    //start_node explored, all other nodes unexplored
    for(node=0;node<num_nodes;node++)
    {
        explored_nodes[node]=false;
        distances[node]=INT_MAX;
    }
    explored_nodes[start_node-1]=true;
    distances[start_node-1]=0;

    for(node=0;node<num_nodes;node++)
    {
        order_first_exploration_BFS[node]=INT_MAX;
    }

    //put start_node in queue
    Queue queue;
    queue.initialise(num_nodes);
    queue.enqueue(start_node);
    
    //queue contains explored nodes
    //...as long as queue exists
    exploration_counter=0;
    while(!(queue.isEmpty()))
    {
        //...take a node from queue
        node=queue.dequeue();
        exploration_counter++;
        order_first_exploration_BFS[node-1]=exploration_counter;
        //...run over adjacent nodes
        for(edge=0;edge<degrees[node-1];edge++)
        {
            adj_node=adjacency_list[node-1][edge];
            //...is it explored?
            //...if yes: do nothing
            //...if no: mark explored and put in queue
            if(!(explored_nodes[adj_node-1]))
            {
                explored_nodes[adj_node-1]=true;
                distances[adj_node-1]=distances[node-1]+1;
                queue.enqueue(adj_node);
            }
        }

    }
}

void Graph::find_conn_components()
{
    if(directed)
    {
        return;
    }

    int start_node,node,adj_node,edge;
    num_conn_components=0;
    Queue queue;
    
    //all nodes unexplored
    for(node=0;node<num_nodes;node++)
    {
        explored_nodes[node]=false;
    }

    //...run over all nodes
    for(start_node=1;start_node<=num_nodes;start_node++)
    {
        //is it explored?
        //...if yes, it belongs to an explored connected component, so do nothing
        //...if not, it doesn't, so explore it with BFS
        if(!(explored_nodes[start_node-1]))
        {
            explored_nodes[start_node-1]=true;

            num_conn_components++;
            conn_components[start_node-1]=num_conn_components;

            //put start_node in queue
            queue.initialise(num_nodes);
            queue.enqueue(start_node);

            //queue contains explored nodes
            //...as long as queue exists
            while(!(queue.isEmpty()))
            {
                //...take a node from queue
                node=queue.dequeue();
                //...run over adjacent nodes
                for(edge=0;edge<degrees[node-1];edge++)
                {
                    adj_node=adjacency_list[node-1][edge];
                    //...is it explored?
                    //...if yes: do nothing
                    //...if no: mark explored and put in queue
                    if(!(explored_nodes[adj_node-1]))
                    {
                        explored_nodes[adj_node-1]=true;
                        conn_components[adj_node-1]=num_conn_components;
                        queue.enqueue(adj_node);
                    }
                }

            }
        }
    }
}

void Graph::exploreDFS1(int start_node)
{
    int node,adj_node,edge,exploration_counter;

    //start_node explored, all other nodes unexplored
    for(node=0;node<num_nodes;node++)
    {
        explored_nodes[node]=false;
    }
    explored_nodes[start_node-1]=true;

    for(node=0;node<num_nodes;node++)
    {
        order_first_exploration_DFS1[node]=INT_MAX;
    }
    
    //put start_node in stack
    Stack stack;
    stack.initialise(num_nodes);
    stack.push(start_node);
    
    //stack contains explored nodes
    //...as long as stack exists
    exploration_counter=0;
    while(!(stack.isEmpty()))
    {
        //...take a node from stack
        node=stack.pop();
        exploration_counter++;
        order_first_exploration_DFS1[node-1]=exploration_counter;
        //...run over adjacent nodes
        for(edge=0;edge<degrees[node-1];edge++)
        {
            adj_node=adjacency_list[node-1][edge];
            //...is it explored?
            //...if yes: do nothing
            //...if no: mark explored and put in stack
            if(!(explored_nodes[adj_node-1]))
            {
                explored_nodes[adj_node-1]=true;
                stack.push(adj_node);
            }
        }

    }
}

void Graph::exploreDFS2(int start_node)
{
    int node;

    //start_node explored, all other nodes unexplored
    for(node=0;node<num_nodes;node++)
    {
        explored_nodes[node]=false;
    }
    explored_nodes[start_node-1]=true;

    for(node=0;node<num_nodes;node++)
    {
        order_first_exploration_DFS2[node]=INT_MAX;
    }

    int exploration_counter=1;

    //outer instance to recursive call
    order_first_exploration_DFS2[start_node-1]=exploration_counter;
    DFS(start_node, &exploration_counter);
}

void Graph::DFS(int start_node, int *exploration_counter)
{
    int adj_node,edge;

    //...run over adjacent nodes
    for(edge=0;edge<degrees[start_node-1];edge++)
    {
        adj_node=adjacency_list[start_node-1][edge];
        //...is it explored?
        //...if yes: do nothing
        //...if no: recursive call
        if(!(explored_nodes[adj_node-1]))
        {
            explored_nodes[adj_node-1]=true;

            (*exploration_counter)++;
            order_first_exploration_DFS2[adj_node-1]=(*exploration_counter);
            DFS(adj_node,exploration_counter);
        }
    }
}

void Graph::find_topological_order()
{
    if(!directed)
    {
        return;
    }

    int node,start_node;

    int top_counter=num_nodes;

    //start_node explored, all other nodes unexplored
    for(node=0;node<num_nodes;node++)
    {
        explored_nodes[node]=false;
    }

    //...run over all nodes
    for(start_node=1;start_node<=num_nodes;start_node++)
    {
        //is it explored?
        //...if yes, do nothing
        //...if not, outer instance to recursive call
        if(!(explored_nodes[start_node-1]))
        {
            explored_nodes[start_node-1]=true;

            DFSmod(start_node, &top_counter);
        }
    }

}

void Graph::DFSmod(int start_node, int *top_counter)
{
    int adj_node,edge;

    //...run over adjacent nodes
    for(edge=0;edge<degrees[start_node-1];edge++)
    {
        adj_node=adjacency_list[start_node-1][edge];
        //...is it explored?
        //...if yes: do nothing
        //...if no: recursive call
        if(!(explored_nodes[adj_node-1]))
        {
            explored_nodes[adj_node-1]=true;

            DFSmod(adj_node,top_counter);
        }
    }
    topological_order[start_node-1]=(*top_counter);
    (*top_counter)--;
}

void Graph::find_scc()
{
    if(!directed)
    {
        return;
    }

    int node,start_node,start_node_ft;

    //first DFS loop on inverted graph
    int t=0;
    int *finishing_times=(int *)malloc(num_nodes*sizeof(int));
    int *finishing_times_inv=(int *)malloc(num_nodes*sizeof(int));

    //...start_node explored, all other nodes unexplored
    for(node=0;node<num_nodes;node++)
    {
        explored_nodes[node]=false;
    }

    //...run over all nodes
    for(start_node=num_nodes;start_node>0;start_node--)
    {
        //is it explored?
        //...if yes, do nothing
        //...if not, outer instance to recursive call
        if(!(explored_nodes[start_node-1]))
        {
            explored_nodes[start_node-1]=true;

            DFSmod2(start_node,&t,finishing_times,finishing_times_inv);
        }
    }

    //second DFS loop
    for(node=0;node<num_nodes;node++)
    {
        explored_nodes[node]=false;
    }

    //...run over all nodes
    for(start_node_ft=num_nodes;start_node_ft>0;start_node_ft--)
    {
        //is it explored?
        //...if yes, do nothing
        //...if not, outer instance to recursive call
        start_node=finishing_times_inv[start_node_ft-1];
        if(!(explored_nodes[start_node-1]))
        {
            explored_nodes[start_node-1]=true;

            DFSmod3(start_node,finishing_times[start_node-1],finishing_times,finishing_times_inv);
        }
    }

    free(finishing_times);
    free(finishing_times_inv);
}

void Graph::DFSmod2(int start_node, int *t, int *finishing_times, int *finishing_times_inv)
{
    int adj_node,edge;

    //...run over adjacent nodes
    for(edge=0;edge<degrees_inv[start_node-1];edge++)
    {
        adj_node=adjacency_list_inv[start_node-1][edge];
        //...is it explored?
        //...if yes: do nothing
        //...if no: recursive call
        if(!(explored_nodes[adj_node-1]))
        {   
            explored_nodes[adj_node-1]=true;
            DFSmod2(adj_node,t,finishing_times,finishing_times_inv);
        }
    }
    (*t)++;
    finishing_times[start_node-1]=(*t);
    finishing_times_inv[(*t)-1]=start_node;
}

void Graph::DFSmod3(int start_node, int s, int *finishing_times, int *finishing_times_inv)
{
    int adj_node,edge;

    scc[start_node-1]=s;
    
    //...run over adjacent nodes
    for(edge=0;edge<degrees[start_node-1];edge++)
    {
        adj_node=adjacency_list[start_node-1][edge];
        //...is it explored?
        //...if yes: do nothing
        //...if no: recursive call
        if(!(explored_nodes[adj_node-1]))
        {
            explored_nodes[adj_node-1]=true;
            DFSmod3(adj_node,s,finishing_times,finishing_times_inv);
        }
    }
}


void Graph::print1()
{
    printf("\nNode\tBFS visiting order\tDFS visiting order\tDistance\tConnected-component label\n");

    int node;
    for(node=0;node<num_nodes;node++)
    {
        printf("%i\t",node+1);

        if(order_first_exploration_BFS[node]==INT_MAX)
        {
            printf("inf\t\t\t");
        }
        else
        {
            printf("%i\t\t\t",order_first_exploration_BFS[node]);
        }

        if(order_first_exploration_DFS2[node]==INT_MAX)
        {
            printf("inf\t\t\t");
        }
        else
        {
            printf("%i\t\t\t",order_first_exploration_DFS2[node]);
        }

        if(distances[node]==INT_MAX)
        {
            printf("inf\t\t");
        }
        else
        {
            printf("%i\t\t",distances[node]);
        }

        printf("%i\n",conn_components[node]);
    }
    printf("\n");
}

void Graph::print2()
{
    printf("\nNode\tBFS visiting order\tDFS1 visiting order\tDFS2 visiting order\tTopological order\n");

    int node;
    for(node=0;node<num_nodes;node++)
    {
        printf("%i\t",node+1);

        if(order_first_exploration_BFS[node]==INT_MAX)
        {
            printf("inf\t\t\t");
        }
        else
        {
            printf("%i\t\t\t",order_first_exploration_BFS[node]);
        }

        if(order_first_exploration_DFS1[node]==INT_MAX)
        {
            printf("inf\t\t\t");
        }
        else
        {
            printf("%i\t\t\t",order_first_exploration_DFS1[node]);
        }

        if(order_first_exploration_DFS2[node]==INT_MAX)
        {
            printf("inf\t\t\t");
        }
        else
        {
            printf("%i\t\t\t",order_first_exploration_DFS2[node]);
        }

        printf("%i\n",topological_order[node]);
    }
    printf("\n");
}

void Graph::print3()
{
    int *scc_frequencies=(int*)calloc(num_nodes,sizeof(int));
    
    int i,j;
    for(i=0;i<num_nodes;i++)
    {
        scc_frequencies[scc[i]-1]++;
    }

    int largest_5_frequencies[5]={0,0,0,0,0};
    
    for(i=0;i<5;i++)
    {
        for(j=0;j<num_nodes;j++)
        {
            if(scc_frequencies[j]>largest_5_frequencies[i])
            {
            largest_5_frequencies[i]=scc_frequencies[j];
            }
        }
        for(j=0;j<num_nodes;j++)
        {
            if(scc_frequencies[j]==largest_5_frequencies[i])
            {
                scc_frequencies[j]=-1;
            }
        }
    }    

    printf("\nThe largest strongly-connected components have size: %i, %i, %i, %i, %i.\n\n",largest_5_frequencies[0],largest_5_frequencies[1],largest_5_frequencies[2],largest_5_frequencies[3],largest_5_frequencies[4]);

    free(scc_frequencies);
}