//node
class NodeUF
{
    public:
        int object;
        int index_parent;               //index position of parent node in array
                                        //the parent of an ancestor nodes is itself
                                        //the ancestor's index serves as the group's name
        int rank;                       //max distance to leaf
};

//union-find
class Union_find{
    public:
        int capacity;                   //capacity of union-find

        int size;                       //length of union-find

        int num_groups;                 //number of distinct elements in nodes

        NodeUF *nodes;                  //array to store nodes
                                        //elements are implicitly labeled as 0,1,... size-1

        Union_find() {}
        void initialise(int Capacity);                                      //allocate memory
        Union_find(const Union_find& uf);
        Union_find& operator=(const Union_find& uf);
        ~Union_find();                                                      //deallocate memory

        bool push(int new_object);                                          //insert new object at back end of array
                                                                            //make it parent of itself

        int find(int index);                                                //find ancestor
        int *find_traversed_nodes(int index, int *num_traversed_nodes);     //collect the traversed nodes up to the ancestor
        void Union(int index1, int index2);                                 //merge two groups

        bool isEmpty();                                                     //check if empty
        bool isFull();                                                      //check if full
};