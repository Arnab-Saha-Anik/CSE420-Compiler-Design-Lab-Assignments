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
        for (char c: name)
        {
            sum += (int)c;
        }
        return sum % bucket_count;
    }


public:
    scope_table();
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
    int index = hash_function(symbol->get_name());
    for (symbol_info *sym : table[index]) {
        if (sym->get_name() == symbol->get_name()) {
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
    int index = hash_function(symbol->get_name());
    table[index].push_front(symbol);
    return true;
}
bool scope_table::delete_from_scope(symbol_info* symbol)
{
    int index = hash_function(symbol->get_name());
    for (auto i = table[index].begin(); i != table[index].end(); ++i) {
        if ((*i)->get_name() == symbol->get_name()) {
            delete *i;
            table[index].erase(i);
            return true;
        }
    }
    return false;
}
void scope_table::print_scope_table(ofstream& outlog)
{
    outlog << "ScopeTable # "+ to_string(unique_id) << endl;

    //iterate through the current scope table and print the symbols and all relevant information
    for (int i = 0; i < bucket_count; i++) {
        if (!table[i].empty()) {
            outlog << "Bucket " << i << ": ";
            for (symbol_info *sym : table[i])
            {
                outlog << "< " << sym->get_name() << " : " << sym->get_type() << " > ";
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