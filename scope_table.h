#include "symbol_info.h"

class scope_table
{
private:
    int bucket_count;
    int unique_id;
    scope_table *parent_scope = NULL;
    vector<list<symbol_info *>> table;

    int hash_function(string name)
    {
        int sum = 0;
        for (int i = 0; i < name.size(); i++)
        {
            sum += (int)name[i];
        }
        return sum % bucket_count;
    }

public:
    scope_table();
    scope_table(int bucket_count);
    scope_table(int bucket_count, int unique_id, scope_table *parent_scope);
    scope_table *get_parent_scope();
    int get_unique_id();
    symbol_info *lookup_in_scope(symbol_info* symbol);
    bool insert_in_scope(symbol_info* symbol);
    bool delete_from_scope(symbol_info* symbol);
    void print_scope_table(ofstream& outlog);
    ~scope_table();

    // you can add more methods if you need
};

scope_table::scope_table()
{
    this->bucket_count = 10;
    this->unique_id = 1;
    this->parent_scope = NULL;
    table.resize(bucket_count);
}

scope_table::scope_table(int bucket_count)
{
    this->bucket_count = bucket_count;
    this->unique_id = 1;
    this->parent_scope = NULL;
    table.resize(bucket_count);
}

scope_table::scope_table(int bucket_count, int unique_id, scope_table *parent_scope)
{
    this->bucket_count = bucket_count;
    this->unique_id = unique_id;
    this->parent_scope = parent_scope;
    table.resize(bucket_count);
}

scope_table *scope_table::get_parent_scope()
{
    return parent_scope;
}

int scope_table::get_unique_id()
{
    return unique_id;
}

symbol_info *scope_table::lookup_in_scope(symbol_info* symbol)
{
    int index = hash_function(symbol->getname());
    for (symbol_info *sym : table[index]) {
        if (sym->getname() == symbol->getname()) {
            return sym;
        }
    }
    return nullptr;
}

bool scope_table::insert_in_scope(symbol_info* symbol)
{
    if (lookup_in_scope(symbol) != nullptr) {
        return false;
    }
    int index = hash_function(symbol->getname());
    table[index].push_back(symbol);
    return true;
}

bool scope_table::delete_from_scope(symbol_info* symbol)
{
    int index = hash_function(symbol->getname());
    for (auto i = table[index].begin(); i != table[index].end(); i++) {
        if ((*i)->getname() == symbol->getname()) {
            table[index].erase(i);
            delete *i;
            return true;
        }
    }
    return false;
}



// complete the methods of scope_table class

void scope_table::print_scope_table(ofstream& outlog)
{   
    outlog << "ScopeTable # " + to_string(unique_id) << endl;
    for (int i = 0; i < bucket_count; i++)
    {
        if (!table[i].empty())
        {
            outlog << i << " --> " << endl;
            for (symbol_info *sym : table[i])
            {
                outlog << "< " << sym->getname() << " : ID >" << endl;
                if (sym->get_symbol_type() == "function")
                {
                    outlog << "Function Definition" << endl;
                    outlog << "Return Type: " << sym->get_return_type() << endl;
                    outlog << "Number of Parameters: " << sym->get_parameters().size() << endl;
                    outlog << "Parameter Details: ";
                    int size = sym->get_parameters().size();
                    for (int j = 0; j < size; j++)
                    {
                        auto param = sym->get_parameters()[j];
                        outlog << param.get_type() <<" "<<param.getname();
                        if (j < size - 1) outlog << ", ";
                    }
                    outlog << endl << endl;
                }
                else if (sym->get_symbol_type() == "array")
                {
                    outlog << "Array" << endl;
                    outlog << "Type: " << sym->get_type() << endl;
                    outlog << "Size: " << sym->get_array_size()<< endl<< endl;
                }
                else
                {
                    outlog << "Variable" << endl;
                    outlog << "Type: " << sym->get_type()<< endl<< endl;
                }
            }
        }
    }
}

scope_table::~scope_table()
{

    for (int i = 0; i < bucket_count; i++)
    {
        for (symbol_info *sym : table[i])
        {
            delete sym;
        }
    }
}