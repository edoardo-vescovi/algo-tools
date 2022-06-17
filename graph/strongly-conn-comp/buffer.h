#define QUEUE_FAILURE -1
//no queue element can be equal to QUEUE_FAILURE
#define STACK_FAILURE -1
//no stack element can be equal to STACK_FAILURE

//circular queue
class Queue{
    public:
        int i_front;    //index of front
        int i_rear;     //index of rear

        int size;       //length of queue
                        //also resolves the ambiguity empty/non-empty queue when i_front+1=i_rear

        int capacity;   //length of array
                        
        int *array;     //array to store queue
                        //
                        //(junk...) |(front queue) (queue...) (rear queue)| (junk...)
                        //
                        //"queue" has length size
                        //"queue"+"junk" have length capacity
                        //(front queue) is at position front
                        //(rear queue) is at position rear

        void initialise(int Capacity);      //allocate memory
        Queue &operator=(const Queue& queue);
        ~Queue();                           //deallocate memory

        bool enqueue(int new_element);  //add element to queue rear
        int dequeue();                  //remove element from queue front

        int front();                    //inspect front
        int rear();                     //inspect rear

        bool isEmpty();                 //check if empty
        bool isFull();                  //check if full
};

class Stack{
    public:
        int size;       //length of stack
                        //so top position is size-1
                        
        int capacity;   //length of array

        int *array;     //array to store stack
                        //
                        //(stack...) (top stack)| (junk...)
                        //
                        //"stack" has length size
                        //"stack"+"junk" have length capacity
                        //(top stack) is at position size-1

        void initialise(int Capacity);  //allocate memory
        Stack &operator=(const Stack& stack);
        ~Stack();                       //deallocate memory

        bool push(int new_element);     //add element to queue rear
        int pop();                      //remove element from queue front

        int top();                      //inspect top

        bool isEmpty();                 //check if empty
        bool isFull();                  //check if full
};