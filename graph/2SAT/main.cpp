#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <list>
#include <random>
#include <set>
#include <tuple>
#include <vector>
#include <math.h>

#define pair std::tuple<const int, const int>
#define quartet std::tuple<const bool, const size_t, const bool, const size_t>

class System{
    private:

        int    first(const pair& clause)     {return std::get<0>(clause);}
        int    second(const pair& clause)    {return std::get<1>(clause);}

        bool   first(const quartet& clause)  {return std::get<0>(clause);}
        size_t second(const quartet& clause) {return std::get<1>(clause);}
        bool   third(const quartet& clause)  {return std::get<2>(clause);}
        size_t fourth(const quartet& clause) {return std::get<3>(clause);}

        void evaluate_clause_values()
        {
            clause_values.resize(clauses.size());
            for(size_t i=0; i<clauses.size(); ++i)
            {
                const quartet clause = clauses[i];
                const bool val_x = first(clause) ? var_values[second(clause)] : !var_values[second(clause)];
                const bool val_y = third(clause) ? var_values[fourth(clause)] : !var_values[fourth(clause)];
                clause_values[i] = val_x || val_y;
            }
        }
        
    public:

        //arrays
        std::vector<int> vars;              //sorted variables
        std::vector<bool> var_values;       //their values
        std::vector<quartet> clauses;       //clauses: each one is (sign1, index var1, sign2, index var1)
        std::vector<bool> clause_values;    //their values

        //variables
        int num_vars;                       //original number variables
        
        //functions
        System(const char *namefile);
        bool twoSAT();                      //determine 2-SAT problem is satisfiable
};

System::System(const char *namefile)
{
    //temp allocations to sort and mod out duplicates
    std::set<int> vars_set;
    std::set<pair> clauses_set;

    FILE *file=fopen(namefile,"r");

    //read num_clauses
    int num_clauses;
    fscanf(file,"%i\n",&num_clauses);

    //read vars and clauses
    for(int i=0;i<num_clauses;++i)
    {
        int x, y;
        fscanf(file,"%i %i\n",&x,&y);
        vars_set.insert(abs(x));
        vars_set.insert(abs(y));
        if(abs(x)<abs(y))
            clauses_set.insert(pair(x,y));
        else
            clauses_set.insert(pair(y,x));
    }
    num_vars = vars_set.size();
    //temp allocation with fast deletion
    std::list<int> vars_list(vars_set.begin(), vars_set.end());
    vars_set.empty();
    //temp allocation with fast deletion
    std::list<pair> clauses_list(clauses_set.begin(), clauses_set.end());
    clauses_set.empty();
    
    fclose(file);

    printf("\nBefore processing: %i variables and %i clauses\n",num_vars,num_clauses);

    //drop variables with trivially-satisfiable clauses
    size_t size;
    do
    {   
        size = vars_list.size();

        //loop over variables
        auto x_it = vars_list.begin();
        while(x_it != vars_list.end())
        {
            printf("Logger: %lu clauses\n",clauses_list.size());

            const auto x = *x_it;

            //x appears in non-negated or negated form in clauses?
            bool pos_var = false;
            bool neg_var = false;

            //loop over clauses
            auto clause_it = clauses_list.begin();
            while(clause_it != clauses_list.end() && !(pos_var && neg_var))
            {
                const auto clause = *clause_it;
                if(x == abs(first(clause)))
                    first(clause)>0 ? pos_var = true : neg_var = true;
                if(x == abs(second(clause)))
                    second(clause)>0 ? pos_var = true : neg_var = true;
                clause_it = std::next(clause_it);
            }

            //x not found
            if(!pos_var && !neg_var)
            {
                x_it = vars_list.erase(x_it);
                continue;
            }

            //x is found in either non-negated or negated forms
            if((!pos_var && neg_var) || (pos_var && !neg_var))
            {
                x_it = vars_list.erase(x_it);

                //loop over clauses and drop those with x
                clause_it = clauses_list.begin();
                while(clause_it != clauses_list.end() || pos_var && neg_var)
                {
                    const auto clause = *clause_it;
                    if(x == abs(first(clause)) || x == abs(second(clause)))
                        clause_it = clauses_list.erase(clause_it);
                    else
                        clause_it = std::next(clause_it);
                }
                continue;
            }

            //x is found in both non-negated and negated forms
            x_it = std::next(x_it);
        }
    } while(size != vars_list.size());

    //sorted non-trivial variables
    vars = std::vector<int>(vars_list.begin(), vars_list.end());
    std::sort(vars.begin(),vars.end());

    //non-trivial clauses
    clauses.reserve(clauses_list.size());
    for(const auto& clause : clauses_list)
    {
        const int x = first(clause);
        const int y = second(clause);
        const size_t index_x = std::lower_bound(vars.begin(), vars.end(), abs(x)) - vars.begin();
        const size_t index_y = std::lower_bound(vars.begin(), vars.end(), abs(y)) - vars.begin();
        clauses.push_back(quartet(first(clause)>0,index_x,second(clause)>0,index_y));
    }

    printf("After processing:  %lu variables and %lu clauses",vars.size(),clauses.size());
}

bool System::twoSAT()
{
    for(unsigned long long i=0; i<log(num_vars)/log(2); ++i)
    {
        //pick random assignment 
        std::srand((unsigned long)i*i*i);
        auto bernoulli = [](){return std::rand()%2==0;};
        var_values.resize(vars.size());
        std::generate(var_values.begin(), var_values.end(), bernoulli);
        //update clause values
        evaluate_clause_values();

        for(unsigned long long j=0; j<((unsigned long long) 2*num_vars*num_vars); ++j)
        {
            //system is sastified?
            if(std::find(clause_values.begin(),clause_values.end(),false) == clause_values.end())
                return true;
            else
            {
                //find false clauses
                std::vector<size_t> indices_false_clauses;
                indices_false_clauses.reserve(clauses.size());
                for(size_t k=0; k<clauses.size(); ++k)
                    if(!clause_values[k])
                        indices_false_clauses.push_back(k);
                //shuffle them
                std::random_shuffle(indices_false_clauses.begin(), indices_false_clauses.end());
                //pick a false clause
                const size_t k = indices_false_clauses[0];
                //try to fix it
                if(std::rand()%2==0)
                    var_values[second(clauses[k])] = !var_values[second(clauses[k])];
                else
                    var_values[fourth(clauses[k])] = !var_values[fourth(clauses[k])];
                //update clause values
                evaluate_clause_values();
            }
        }
    }

    //system is probably unsatisfiable
    return false;
}

int main()
{
    const char* filenames[] = {"2sat1.txt"};
    for(int i=0; i<1; ++i)
    {
        System system(filenames[i]);

        printf("\nThe 2-SAT problem %s is satisfied?", filenames[i]);
        printf("\n%s\n", system.twoSAT() ? "Yes" : "No");
    }

    return EXIT_SUCCESS;
}