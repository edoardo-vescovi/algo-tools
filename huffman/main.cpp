#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <queue>

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
    if(x<y)
    {
        return y;
    }
    else
    {
        return x;
    }
}

//a node in a binary tree
class Symbol
{
    public:
        std::vector<int> id;                //(meta)symbol id
                                            //size: 1 for symbol, >1 for metasymbol
                                            //vector is maintained sorted by symbols' individual weights
        unsigned long long int weight;      //total weight
        int depth_min;                      //number of edges from (meta)symbol to shallowest leaf
        int depth_max;                      //number of edges from (meta)symbol to deepest leaf

        Symbol *parent;
        Symbol *left_child;
        Symbol *right_child;
};

bool less_than(Symbol *sym1, Symbol *sym2)
{
    return sym1->weight<sym2->weight;
}

void assign(Symbol *new_sym, Symbol *old_sym)
{
    (new_sym->id)=(old_sym->id);
    (new_sym->weight)=(old_sym->weight);
    (new_sym->depth_min)=(old_sym->depth_min);
    (new_sym->depth_max)=(old_sym->depth_max);
    
    (new_sym->parent)=(old_sym->parent);
    (new_sym->left_child)=(old_sym->left_child);
    (new_sym->right_child)=(old_sym->right_child);
}

int main()
{
    //input data
    int size;
    std::vector<Symbol *> data;

    //read file
    FILE *file=fopen("huffman.txt","r");
    fscanf(file,"%i\n",&size);

    Symbol *tmp_sym;
    int i;
    for(i=0;i<size;i++)
    {
        tmp_sym=new Symbol;

        tmp_sym->id.push_back(i);
        fscanf(file,"%llu\n",&(tmp_sym->weight));
        tmp_sym->depth_min=0;
        tmp_sym->depth_max=0;

        tmp_sym->parent=NULL;
        tmp_sym->left_child=NULL;
        tmp_sym->right_child=NULL;

        data.push_back(tmp_sym);
    }
    fclose(file);

    //sort input data
    sort(data.begin(),data.end(),&less_than);

    //initialise 2 queues and root(s) of binary tree
    std::queue<Symbol *> queue_symbols;
    std::queue<Symbol *> queue_metasymbols;
    std::vector<Symbol *> BT;
    for(i=0;i<size;i++)
    {
        queue_symbols.push(data[i]);
        BT.push_back(data[i]);
    }

    //erase input data
    data.clear();

    //Huffman algorithm
    Symbol *lowest=new Symbol;
    Symbol *next_lowest=new Symbol;
    bool is_lowest_sym;             //...is lowest1 a symbol or a metasymbol?
    bool is_next_lowest_sym;        //...is lowest2 a symbol or a metasymbol?
    bool done;                      //...flag
    
    //...as long as there are 2 (meta)symbols to merge into new metasymbols
    int ii=-1;;
    while(queue_symbols.size()+queue_metasymbols.size()>=2)
    {ii++;
        //...take 2 lowest-weight (meta)symbols
        done=false;
        //22
        if(queue_symbols.size()>=2 && queue_metasymbols.size()>=2 && !done)
        {
            if((queue_symbols.front()->weight)<(queue_metasymbols.front()->weight))
            {
                assign(lowest,queue_symbols.front());
                queue_symbols.pop();
                is_lowest_sym=true;
            }
            else
            {
                assign(lowest,queue_metasymbols.front());
                queue_metasymbols.pop();
                is_lowest_sym=false;
            }
            if((queue_symbols.front()->weight)<(queue_metasymbols.front()->weight))
            {
                assign(next_lowest,queue_symbols.front());
                queue_symbols.pop();
                is_next_lowest_sym=true;
            }
            else
            {
                assign(next_lowest,queue_metasymbols.front());
                queue_metasymbols.pop();
                is_next_lowest_sym=false;
            }
            done=true;
        }
        //...12
        if(queue_symbols.size()==1 && queue_metasymbols.size()>=2 && !done)
        {
            if((queue_symbols.front()->weight)<(queue_metasymbols.front()->weight))
            {
                assign(lowest,queue_symbols.front());
                queue_symbols.pop();
                is_lowest_sym=true;
                assign(next_lowest,queue_metasymbols.front());
                queue_metasymbols.pop();
                is_next_lowest_sym=false;
            }
            else
            {
                assign(lowest,queue_metasymbols.front());
                queue_metasymbols.pop();
                is_lowest_sym=false;
                if((queue_symbols.front()->weight)<(queue_metasymbols.front()->weight))
                {
                    assign(next_lowest,queue_symbols.front());
                    queue_symbols.pop();
                    is_next_lowest_sym=true;
                }
                else
                {
                    assign(next_lowest,queue_metasymbols.front());
                    queue_metasymbols.pop();
                    is_next_lowest_sym=false;
                }
            }
            done=true;
        }
        //...21
        if(queue_metasymbols.size()==1 && queue_symbols.size()>=2 && !done)
        {
            if((queue_metasymbols.front()->weight)<(queue_symbols.front()->weight))
            {
                assign(lowest,queue_metasymbols.front());
                queue_metasymbols.pop();
                is_lowest_sym=false;
                assign(next_lowest,queue_symbols.front());
                queue_symbols.pop();
                is_next_lowest_sym=true;
            }
            else
            {
                assign(lowest,queue_symbols.front());
                queue_symbols.pop();
                is_lowest_sym=true;
                if((queue_metasymbols.front()->weight)<(queue_symbols.front()->weight))
                {
                    assign(next_lowest,queue_metasymbols.front());
                    queue_metasymbols.pop();
                    is_next_lowest_sym=false;
                }
                else
                {
                    assign(next_lowest,queue_symbols.front());
                    queue_symbols.pop();
                    is_next_lowest_sym=true;
                }
            }
            done=true;
        }
        //...11
        if(queue_symbols.size()==1 && queue_metasymbols.size()==1 && !done)
        {
            if((queue_symbols.front()->weight)<(queue_metasymbols.front()->weight))
            {
                assign(lowest,queue_symbols.front());
                queue_symbols.pop();
                is_lowest_sym=true;
                assign(next_lowest,queue_metasymbols.front());
                queue_metasymbols.pop();
                is_next_lowest_sym=false;
            }
            else
            {
                assign(lowest,queue_metasymbols.front());
                queue_metasymbols.pop();
                is_lowest_sym=false;
                assign(next_lowest,queue_symbols.front());
                queue_symbols.pop();
                is_next_lowest_sym=true;
            }
            done=true;
        }
        //...20
        if(queue_symbols.size()>=2 && queue_metasymbols.size()==0 && !done)
        {
            assign(lowest,queue_symbols.front());
            queue_symbols.pop();
            is_lowest_sym=true;
            assign(next_lowest,queue_symbols.front());
            queue_symbols.pop();
            is_next_lowest_sym=true;
            done=true;
        }
        //...02
        if(queue_metasymbols.size()>=2 && queue_symbols.size()==0 && !done)
        {
            assign(lowest,queue_metasymbols.front());
            queue_metasymbols.pop();
            is_lowest_sym=false;
            assign(next_lowest,queue_metasymbols.front());
            queue_metasymbols.pop();
            is_next_lowest_sym=false;
            done=true;
        }

        //...extract lowest's and next_lowest's subtrees from BT
        for(i=0;i<BT.size();i++)
        {
            if(lowest->id==BT[i]->id)
            {
                BT.erase(BT.begin()+i);
                break;
            }
        }
        for(i=0;i<BT.size();i++)
        {
            if(next_lowest->id==BT[i]->id)
            {
                BT.erase(BT.begin()+i);
                break;
            }
        }
        //...merge into a subtree
        tmp_sym=new Symbol;
        for(i=0;i<lowest->id.size();i++)
        {
            tmp_sym->id.push_back(lowest->id[i]);
        }
        for(i=0;i<next_lowest->id.size();i++)
        {
            tmp_sym->id.push_back(next_lowest->id[i]);
        }
        tmp_sym->weight=(lowest->weight)+(next_lowest->weight);
        if(is_lowest_sym || is_next_lowest_sym)
        {
            tmp_sym->depth_min=1;
        }
        else
        {
            tmp_sym->depth_min=1+min(lowest->depth_min,next_lowest->depth_min);;
        }
        tmp_sym->depth_max=1+max(lowest->depth_max,next_lowest->depth_max);
        tmp_sym->parent=NULL;
        tmp_sym->left_child=lowest;
        tmp_sym->right_child=next_lowest;
        lowest->parent=tmp_sym;
        next_lowest->parent=tmp_sym;
        //...put new metasymbol in BT and queue
        BT.push_back(tmp_sym);
        queue_metasymbols.push(tmp_sym);
    }

    //...there is 1 metasymbol: take and place it BT
    assign(lowest,queue_metasymbols.front());
    queue_metasymbols.pop();
    BT.push_back(lowest);

    //printf
    printf("\nThe maximum length of a codeword in the Huffman code is %i",BT[0]->depth_max);
    printf("\nThe minimum length of a codeword in the Huffman code is %i\n\n",BT[0]->depth_min);

    return EXIT_SUCCESS;
}