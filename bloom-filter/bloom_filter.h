#include <vector>
#include <string>

//Bloom filter
class Bloom_filter{
    public:
        std::vector<unsigned char> bytes;                   //the array of bytes
                                                            //info is stored in bits
        int num_hash_functions;                             //number of hash functions
                                                            //code ensures doesn't exceed number of bits

        void initialise(int num_bytes, int proposed_num_hash_functions);

        std::vector<int> hash_functions(std::string new_object);
                                                            //generate distinct hash codes deterministically
        void insert(std::string new_object);
        bool lookup(std::string new_object);

        ~Bloom_filter();
};