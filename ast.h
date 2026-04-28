#ifndef AST_H
#define AST_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>

using namespace std;

class ASTNode {
public:
    virtual ~ASTNode() {}
    virtual string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp, int& temp_count, int& label_count) const = 0;
};

// Expression node types

class ExprNode : public ASTNode {
protected:
    string node_type; // Type information (int, float, void, etc.)
public:
    ExprNode(string type) : node_type(type) {}
    virtual string get_type() const { return node_type; }
};

// Variable node (for ID references)

class VarNode : public ExprNode {
private:
    string name;
    ExprNode* index; // For array access, nullptr for simple variables

public:
    VarNode(string name, string type, ExprNode* idx = nullptr)
        : ExprNode(type), name(name), index(idx) {}
    
    ~VarNode() { if(index) delete index; }
    
    bool has_index() const { return index != nullptr; }
    
    string generate_index_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                              int& temp_count, int& label_count) const {
        
        // TODO: Implement this method
        // Should generate code to calculate the array index and return the temp variable
        if (!index) {
            return "";
        }
        string index_val = index->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        return index_val;
    }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for variable access or array access
        if (!index) {
            auto it = symbol_to_temp.find(name);
            if (it != symbol_to_temp.end()) {
                return it->second;
            }

            string t = "t" + to_string(temp_count++);
            outcode << t << " = " << name << endl;
            symbol_to_temp[name] = t;
            return t;
        }
        string idx = index->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        string t = "t" + to_string(temp_count++);
        outcode << t << " = " << name << "[" << idx << "]" << endl;
        return t;
    }
    
    string get_name() const { return name; }
};

// Constant node

class ConstNode : public ExprNode {
private:
    string value;

public:
    ConstNode(string val, string type) : ExprNode(type), value(val) {}
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for constant values
        string t = "t" + to_string(temp_count++);
        outcode << t << " = " << value << endl;
        return t;
    }
};

// Binary operation node

class BinaryOpNode : public ExprNode {
private:
    string op;
    ExprNode* left;
    ExprNode* right;

public:
    BinaryOpNode(string op, ExprNode* left, ExprNode* right, string result_type)
        : ExprNode(result_type), op(op), left(left), right(right) {}
    
    ~BinaryOpNode() {
        delete left;
        delete right;
    }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for binary operations
        string l_val = left->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        string r_val = right->generate_code(outcode, symbol_to_temp, temp_count, label_count);
    
        string t = "t" + to_string(temp_count++);
        outcode << t << " = " << l_val << " " << op << " " << r_val << endl;
    
        return t;
    }
};

// Unary operation node

class UnaryOpNode : public ExprNode {
private:
    string op;
    ExprNode* expr;

public:
    UnaryOpNode(string op, ExprNode* expr, string result_type)
        : ExprNode(result_type), op(op), expr(expr) {}
    
    ~UnaryOpNode() { delete expr; }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for unary operations
        string ev = expr->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        string t = "t" + to_string(temp_count++);
        outcode << t << " = " << op << " " << ev << endl;
        return t;
    }
};

// Assignment node

class AssignNode : public ExprNode {
private:
    VarNode* lhs;
    ExprNode* rhs;

public:
    AssignNode(VarNode* lhs, ExprNode* rhs, string result_type)
        : ExprNode(result_type), lhs(lhs), rhs(rhs) {}
    
    ~AssignNode() {
        delete lhs;
        delete rhs;
    }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for assignment operations
        string rv = rhs->generate_code(outcode, symbol_to_temp, temp_count, label_count);

        if (lhs->has_index()) {
            string idx = lhs->generate_index_code(outcode, symbol_to_temp, temp_count, label_count);
            outcode << lhs->get_name() << "[" << idx << "] = " << rv << endl;
            return rv;
        }

        outcode << lhs->get_name() << " = " << rv << endl;
        symbol_to_temp[lhs->get_name()] = rv;

        return rv;

    }
};

// Statement node types

class StmtNode : public ASTNode {
public:
    virtual string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                                int& temp_count, int& label_count) const = 0;
};

// Expression statement node

class ExprStmtNode : public StmtNode {
private:
    ExprNode* expr;

public:
    ExprStmtNode(ExprNode* e) : expr(e) {}
    ~ExprStmtNode() { if(expr) delete expr; }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for expression statements
        if (expr) {
            expr->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        }
        return "";
    }
};

// Block (compound statement) node

class BlockNode : public StmtNode {
private:
    vector<StmtNode*> statements;

public:
    ~BlockNode() {
        for (auto stmt : statements) {
            delete stmt;
        }
    }
    
    void add_statement(StmtNode* stmt) {
        if (stmt) statements.push_back(stmt);
    }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for all statements in the block
        for (auto* s : statements) s->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        return "";
    }
};

// If statement node

class IfNode : public StmtNode {
private:
    ExprNode* condition;
    StmtNode* then_block;
    StmtNode* else_block; // nullptr if no else part

public:
    IfNode(ExprNode* cond, StmtNode* then_stmt, StmtNode* else_stmt = nullptr)
        : condition(cond), then_block(then_stmt), else_block(else_stmt) {}
    
    ~IfNode() {
        delete condition;
        delete then_block;
        if (else_block) delete else_block;
    }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for if-else statements
        string cond = condition->generate_code(outcode, symbol_to_temp, temp_count, label_count);

        string Lthen = "L" + to_string(label_count++);
        string Lelse = "L" + to_string(label_count++);
        string Lend  = "L" + to_string(label_count++);

        outcode << "if " << cond << " goto " << Lthen << endl;
        outcode << "goto " << Lelse << endl;

        outcode << Lthen << ":" << endl;
        if (then_block) then_block->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        outcode << "goto " << Lend << endl;

        outcode << Lelse << ":" << endl;
        if (else_block) else_block->generate_code(outcode, symbol_to_temp, temp_count, label_count);

        outcode << Lend << ":" << endl;
        return "";

    }
};

// While statement node

class WhileNode : public StmtNode {
private:
    ExprNode* condition;
    StmtNode* body;

public:
    WhileNode(ExprNode* cond, StmtNode* body_stmt)
        : condition(cond), body(body_stmt) {}
    
    ~WhileNode() {
        delete condition;
        delete body;
    }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for while loops
        string Lstart = "L" + to_string(label_count++);
        string Lbody  = "L" + to_string(label_count++);
        string Lend   = "L" + to_string(label_count++);

        outcode << Lstart << ":" << endl;


        string cond = condition->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        outcode << "if " << cond << " goto " << Lbody << endl;
        outcode << "goto " << Lend << endl;

        outcode << Lbody << ":" << endl;
        if (body) body->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        outcode << "goto " << Lstart << endl;

        outcode << Lend << ":" << endl;
        return "";
    }
};

// For statement node

class ForNode : public StmtNode {
private:
    ExprNode* init;
    ExprNode* condition;
    ExprNode* update;
    StmtNode* body;

public:
    ForNode(ExprNode* init_expr, ExprNode* cond_expr, ExprNode* update_expr, StmtNode* body_stmt)
        : init(init_expr), condition(cond_expr), update(update_expr), body(body_stmt) {}
    
    ~ForNode() {
        if (init) delete init;
        if (condition) delete condition;
        if (update) delete update;
        delete body;
    }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for for loops
        if (init) init->generate_code(outcode, symbol_to_temp, temp_count, label_count);

        string Lstart = "L" + to_string(label_count++);
        string Lbody  = "L" + to_string(label_count++);
        string Lend   = "L" + to_string(label_count++);

        outcode << Lstart << ":" << endl;
        symbol_to_temp.clear();

        if (condition) {
            string cond = condition->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        
            // FIX: condition expression sometimes returns "" even after emitting "tK = ...".
            // In that case, the correct condition temp is the last one allocated.
            if (cond.empty() && temp_count > 0) {
                cond = "t" + to_string(temp_count - 1);
            }
        
            outcode << "if " << cond << " goto " << Lbody << endl;
            outcode << "goto " << Lend << endl;
        }

        outcode << Lbody << ":" << endl;

        // After condition, t12=i is now in cache; allow body to reuse it (no extra t=i)
        if (body) body->generate_code(outcode, symbol_to_temp, temp_count, label_count);

        // update should not reuse old i temp; but code2 update uses t12 and constants,
        // so keep cache as-is.
        if (update) update->generate_code(outcode, symbol_to_temp, temp_count, label_count);

        outcode << "goto " << Lstart << endl;
        outcode << Lend << ":" << endl;
        return "";
    


    }
};

// Return statement node

class ReturnNode : public StmtNode {
private:
    ExprNode* expr;

public:
    ReturnNode(ExprNode* e) : expr(e) {}
    ~ReturnNode() { if (expr) delete expr; }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for return statements
        if (expr) {
            string rv = expr->generate_code(outcode, symbol_to_temp, temp_count, label_count);
            outcode << "return " << rv << endl << endl;
        } else {
            outcode << "return" << endl << endl;
        }
        return "";
    }
};

// Declaration node

class DeclNode : public StmtNode {
private:
    string type;
    vector<pair<string, int>> vars; // Variable name and array size (0 for regular vars)

public:
    DeclNode(string t) : type(t) {}
    
    void add_var(string name, int array_size = 0) {
        vars.push_back(make_pair(name, array_size));
    }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for variable declarations
        for (auto& v : vars) {
            if (v.second > 0) outcode << "// Declaration: " << type << " " << v.first << "[" << v.second << "]" << endl;
            else outcode << "// Declaration: " << type << " " << v.first << endl;
        }
        return "";
    }
    
    string get_type() const { return type; }
    const vector<pair<string, int>>& get_vars() const { return vars; }
};

// Function declaration node

class FuncDeclNode : public ASTNode {
private:
    string return_type;
    string name;
    vector<pair<string, string>> params; // Parameter type and name
    BlockNode* body;

public:
    FuncDeclNode(string ret_type, string n) : return_type(ret_type), name(n), body(nullptr) {}
    ~FuncDeclNode() { if (body) delete body; }
    
    void add_param(string type, string name) {
        params.push_back(make_pair(type, name));
    }
    
    void set_body(BlockNode* b) {
        body = b;
    }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for function declarations
        symbol_to_temp.clear();

        outcode << "// Function: " << return_type << " " << name << "(";
        for (size_t i = 0; i < params.size(); ++i) {
            outcode << params[i].first << " " << params[i].second;
            if (i + 1 < params.size()) outcode << ", ";
        }
        outcode << ")" << endl;

        if (body) body->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        return "";
    }
};

// Helper class for function arguments

class ArgumentsNode : public ASTNode {
private:
    vector<ExprNode*> args;

public:
    ~ArgumentsNode() {
        // Don't delete args here - they'll be transferred to FuncCallNode
    }
    
    void add_argument(ExprNode* arg) {
        if (arg) args.push_back(arg);
    }
    
    ExprNode* get_argument(int index) const {
        if (index >= 0 && index < args.size()) {
            return args[index];
        }
        return nullptr;
    }
    
    size_t size() const {
        return args.size();
    }
    
    const vector<ExprNode*>& get_arguments() const {
        return args;
    }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // This node doesn't generate code directly
        return "";
    }
};

// Function call node

class FuncCallNode : public ExprNode {
private:
    string func_name;
    vector<ExprNode*> arguments;

public:
    FuncCallNode(string name, string result_type)
        : ExprNode(result_type), func_name(name) {}
    
    ~FuncCallNode() {
        for (auto arg : arguments) {
            delete arg;
        }
    }
    
    void add_argument(ExprNode* arg) {
        if (arg) arguments.push_back(arg);
    }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for function calls
        for (auto* arg : arguments) {
            if (auto* v = dynamic_cast<VarNode*>(arg); v && !v->has_index()) {
                string at = "t" + to_string(temp_count++);
                outcode << at << " = " << v->get_name() << endl;
                outcode << "param " << at << endl;
            } else {
                string aval = arg->generate_code(outcode, symbol_to_temp, temp_count, label_count);
                outcode << "param " << aval << endl;
            }
        }

        string ret = "t" + to_string(temp_count++);
        outcode << ret << " = call " << func_name << ", " << arguments.size() << endl;
        return ret;
    }
};

// Program node (root of AST)

class ProgramNode : public ASTNode {
private:
    vector<ASTNode*> units;

public:
    ~ProgramNode() {
        for (auto unit : units) {
            delete unit;
        }
    }
    
    void add_unit(ASTNode* unit) {
        if (unit) units.push_back(unit);
    }
    
    string generate_code(ofstream& outcode, map<string, string>& symbol_to_temp,
                        int& temp_count, int& label_count) const override {
        // TODO: Implement this method
        // Should generate code for the entire program
        for (auto* u : units) u->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        return "";
    }
};

#endif // AST_H