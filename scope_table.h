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
    scope_table(int bucket_count, int unique_id, scope_table *parent_scope);
    scope_table(int n);
    scope_table *get_parent_scope();
    int get_unique_id();
    symbol_info *lookup_in_scope(symbol_info* symbol);
    bool insert_in_scope(symbol_info* symbol);
    bool delete_from_scope(symbol_info* symbol);
    void print_scope_table(ofstream& outlog);  // Changed back to ofstream&
    ~scope_table();

    // you can add more methods if you need
};

// complete the methods of scope_table class
scope_table::scope_table()
{
    this->bucket_count = 10;
    this->unique_id = 0;
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
scope_table::scope_table(int n)
{
    this->bucket_count = n;
    this->unique_id = 0;
    this->parent_scope = NULL;
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
    return NULL;
}
bool scope_table::insert_in_scope(symbol_info* symbol)
{
    if (lookup_in_scope(symbol) != NULL) {
        return false;
    }
    int index = hash_function(symbol->getname());
    table[index].push_front(symbol);
    return true;
}
bool scope_table::delete_from_scope(symbol_info* symbol)
{
    int index = hash_function(symbol->getname());
    for (auto i = table[index].begin(); i != table[index].end(); ++i) {
        if ((*i)->getname() == symbol->getname()) {
            delete *i;
            table[index].erase(i);
            return true;
        }
    }
    return false;
}
void scope_table::print_scope_table(ofstream& outlog)
{
    outlog << "ScopeTable # " + to_string(unique_id) << endl;

    // Iterate through the current scope table and print the symbols and all relevant information
    for (int i = 0; i < bucket_count; i++)
    {
        if (!table[i].empty())
        {
            outlog << i << "--> ";
            for (symbol_info *sym : table[i])
            {
                outlog << "<" << sym->getname() << ",";
                if (sym->get_symbol_type() == "function")
                {
                    outlog << sym->get_return_type() << ",";
                    vector<symbol_info*> params = sym->get_parameters();
                    if (!params.empty())
                    {
                        outlog << "(";
                        for (size_t j = 0; j < params.size(); ++j)
                        {
                            outlog << params[j]->get_type();
                            if (j < params.size() - 1) outlog << ",";
                        }
                        outlog << ")";
                    } else {
                        outlog << "()";
                    }
                } else if (sym->get_symbol_type() == "array") {
                    outlog << sym->get_type() << "[" << sym->get_array_size() << "]";
                } else {
                    outlog << sym->get_type();
                }
                outlog << "> ";
            }
            outlog << endl;
        }
    }
}
scope_table::~scope_table()
{
    for (int i = 0; i < bucket_count; i++) {
        for (symbol_info *sym : table[i]) {
            delete sym;
        }
    }
}