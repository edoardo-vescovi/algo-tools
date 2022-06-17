#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <tuple>
#include <math.h>

#define triplet std::tuple<const int, const double, const double>
#define INF_DISTANCE 1000000.

template <typename T>
T min(T x, T y)
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

double distanceSquared(double x1, double y1, double x2, double y2)
{
    return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
}

class Graph{
    public:

        //arrays
        std::list<triplet> node_list;        //the i-th entry is the triplet (node number, x, y)

        //array dimensions
        int num_nodes;                      //number of nodes

        //functions
        Graph(const char *namefile);
        Graph(const Graph& old);
        Graph& operator=(const Graph& old);
        double heuristicTSP();               //minimum cost of a traveling salesman tour
        ~Graph();
};

Graph::Graph(const char *namefile)
{
    FILE *file=fopen(namefile,"r");

    //read num_nodes
    fscanf(file,"%i\n",&num_nodes);

    int node;
    double x, y;

    for(int i=0;i<num_nodes;++i)
    {
        fscanf(file,"%i %lf %lf\n",&node,&x,&y);
        node_list.push_back(std::make_tuple(node,x,y));
    }

    fclose(file);
}

Graph::Graph(const Graph& old)
{
    num_nodes=old.num_nodes;

    for(const auto& node : old.node_list)
        node_list.push_back(node);
}

Graph& Graph::operator=(const Graph& old)
{    
    if(this!=&old)
    { 
        num_nodes=old.num_nodes;

        for(const auto& node : old.node_list)
            node_list.push_back(node);
    }

    return *this;
}

Graph::~Graph()
{
}

double Graph::heuristicTSP()
{
    double res = 0;
    auto first_node_it = node_list.begin();
    decltype(first_node_it) second_node_min_it;
    const auto node1 = node_list.front();
    
    while(node_list.size() > 1)
    {
        double distanceSquared_min = INF_DISTANCE*INF_DISTANCE;
        
        // loop over the nodes, save for the first in node_list
        for(auto second_node_it = node_list.begin(); second_node_it!=node_list.end(); second_node_it=std::next(second_node_it))
        {
            if(first_node_it == second_node_it)
                continue;

            // find the node closest to the first node
            double distanceSquared = ::distanceSquared(std::get<1>(*first_node_it)-1, std::get<2>(*first_node_it)-1, std::get<1>(*second_node_it)-1, std::get<2>(*second_node_it)-1);
            if(distanceSquared < distanceSquared_min)
            {
                distanceSquared_min = distanceSquared;
                second_node_min_it = second_node_it;
            }
        }
        // move to that node and delete the first node
        res += sqrt(distanceSquared_min);
        node_list.erase(first_node_it);
        first_node_it = second_node_min_it;
    }

    // hop back to the only node left
    res += sqrt(::distanceSquared(std::get<1>(node_list.front())-1, std::get<2>(node_list.front())-1, std::get<1>(node1)-1, std::get<2>(node1)-1));
    return res;
}

int main()
{
    Graph graph("nn.txt");

    printf("\nThe minimum cost of a traveling salesman tour is:");    
    printf("\n%i\n", (int)graph.heuristicTSP());

    return EXIT_SUCCESS;
}