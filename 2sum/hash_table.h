#define TARGET 10000

//a single node
class Node{
    public:
        long long int key;
        long long int object;
        Node *parent;
        Node *child;
};

//hash table with chaining
class Hash_table{
    public:
        long long int capacity;                             //number of allocable items
        Node **buckets;                                     //allocable items, as array of DDL

        void initialise(long long int proposed_capacity);

        long long int hash_function(long long int new_key);
        void insert(long long int new_object, long long int new_key);
        bool lookup(long long int new_key);

        void print();
        ~Hash_table();
};