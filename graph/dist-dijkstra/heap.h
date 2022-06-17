#include <limits.h>

#define HEAP_FAILURE -INT_MAX
//no heap object nor key can be equal to HEAP_FAILURE

//min heap
class Heap{
    private:
        int parent(int i_index);        //array[...] is parent of array[i_index]
        int left_child(int i_index);    //array[...] is left child of array[i_index]
        int right_child(int i_index);   //array[...] is right child of array[i_index]

    public:
        int size;       //length of heap
                        
        int capacity;   //length of array

        int *objects;   //array to store objects
                        //
                        //(heap...) (bottom heap)| (junk...)
                        //
                        //"heap" has length size
                        //"heap"+"junk" have length capacity
        int *keys;      //array to store keys
                        //as above

        void initialise(int Capacity);              //allocate memory
        Heap& operator=(const Heap& heap);
        ~Heap();                                    //deallocate memory

        bool insert(int new_object, int new_key);   //add element to heap bottom
        int extractMin();                           //remove element from heap top
        bool deleteObject(int new_object);          //remove element from heap

        int minObject();                            //inspect top element
        int minKey();                               //inspect top key

        bool doesItContain(int new_object);         //check if new_object appears in heap
        int key(int new_object);                    //return key of new_object

        bool isEmpty();                             //check if empty
        bool isFull();                              //check if full
};