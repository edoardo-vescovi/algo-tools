class Graph{
    private:

        //aux variables for functions
        bool *explored_nodes;               //the i-th entry tells if the i-th node has been visited
        int *order_first_exploration_BFS;   //the i-th entry tells which iteration functions visit the i-th node
        int *order_first_exploration_DFS1;
        int *order_first_exploration_DFS2;

    public:

        //arrays
        int **adjacency_list;       //the i-th entry is the array of head-nodes coming from the i-th tail-node:
                                    //(1st head-node, ... n_j-th head-node)
     
        int *incidence_list_tail;   //the i-th entry is the tail-node of the i-th edge
        int *incidence_list_head;   //the i-th entry is the head-node of the i-th edge

        //array dimensions
        bool directed;              //undirected or directed graph?
                                    //note: if directed=false, heads and tails become simply nodes
                                    //and we set incidence_list_tail[i]<=incidence_list_head[i]

        int num_nodes;              //number of nodes

        int *degrees;               //the i-th entry is n_i

        int num_edges;              //number of edges

        //variables for undirected graph
        int *distances;             //the i-th entry is the distance between a start and i-th node
        int num_conn_components;    //number of connected components  
        int *conn_components;       //the i-th entry tells in which connected component the i-th node belongs

        //variables for directed graph
        int **adjacency_list_inv;   //as above, for transpose graph
        int *degrees_inv;           //as above, for transpose graph
        int *topological_order;     //the i-th entry is the topological-order label of the i-th node
        int *scc;                   //the i-th entry is the strongly-connected-component label of the i-th node

        //functions
        void initialise(const char *namefile, bool dir);            //...but not aux and variables for (un)directed graph
        ~Graph();

        //BFS functions for graph
        void exploreBFS(int start_node);                            //explore a graph from start_node, saving result in explored_nodes

        //BFS functions for undirected graph
        void exploreBFS_and_distance(int start_node);               //as above, also calculate distances from start_node
        void find_conn_components();                                //find which connected component each node belongs in

        //DFS functions for graph
        void exploreDFS1(int start_node);                           //as above
        void exploreDFS2(int start_node);                           //as above
        void DFS(int start_node, int *exploration_counter);

        //DFS functions for directed graph
        void find_topological_order();                              //label nodes according to a topological order
        void DFSmod(int start_node, int *top_counter);
        void find_scc();                                            //find which strongly-connected component each node belongs in
        void DFSmod2(int start_node, int *t, int *finishing_times, int *finishing_times_inv);
        void DFSmod3(int start_node, int s, int *finishing_times, int *finishing_times_inv);

        //print functions for main
        void print1();
        void print2();
        void print3();
};