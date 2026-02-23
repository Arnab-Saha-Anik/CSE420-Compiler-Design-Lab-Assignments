%{

#include"symbol_info.h"

#define YYSTYPE symbol_info*

int yyparse(void);
int yylex(void);

extern FILE *yyin;


ofstream outlog;

int lines;

// declare any other variables or functions needed here

%}

%token IF FOR DO INT FLOAT VOID SWITCH DEFAULT GOTO ELSE WHILE BREAK CHAR DOUBLE RETURN CASE CONTINUE PRINTF ADDOP MULOP INCOP RELOP ASSIGNOP LOGICOP NOT LPAREN RPAREN LCURL RCURL LTHIRD RTHIRD COMMA COLON SEMICOLON ID CONST_INT CONST_FLOAT

%%

start : program
	{
		outlog<<"At line no: "<<lines<<" start : program "<<endl<<endl;
	}
	;

program : program unit
	{
		outlog<<"At line no: "<<lines<<" program : program unit "<<endl<<endl;
		outlog<<$1->getname()+"\n"+$2->getname()<<endl<<endl;
		
		$$ = new symbol_info($1->getname()+"\n"+$2->getname(),"program");
	}
	| unit
	{
		outlog<<"At line no: "<<lines<<" program : unit "<<endl<<endl;
		outlog<<$1->getname()<<endl<<endl;
		
		$$ = new symbol_info($1->getname(),"program");
	}
	;

unit : var_declaration
	{
		outlog<<"At line no: "<<lines<<" unit : var_declaration "<<endl<<endl;
		outlog<<$1->getname()<<endl<<endl;
		$$ = new symbol_info($1->getname(),"unit");			
	}
	| func_definition
	{
		outlog<<"At line no: "<<lines<<" unit : func_definition "<<endl<<endl;
		outlog<<$1->getname()<<endl<<endl;
		$$ = new symbol_info($1->getname(),"unit");
	}
	;

func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement
		{	
			outlog<<"At line no: "<<lines<<" func_definition : type_specifier ID LPAREN paremeter_list RPAREN compound_statement "<<endl<<endl;
			outlog<<$1->getname()<<" "<<$2->getname()<<"("<<$4->getname()<<")\n"<<$5->getname()<<endl<<endl;
			
			$$ = new symbol_info($1->getname()<<" "<<$2->getname()<<"("<<$4->getname()<<")\n"<<$5->getname(),"func_def");
		}
		| type_specifier ID LPAREN RPAREN compound_statement
		{
			
			outlog<<"At line no: "<<lines<<" func_definition : type_specifier ID LPAREN RPAREN compound_statement "<<endl<<endl;
			outlog<<$1->getname()<<" "<<$2->getname()<<"()\n"<<$5->getname()<<endl<<endl;
			
			$$ = new symbol_info($1->getname()+" "+$2->getname()+"()\n"+$5->getname(),"func_def");	
		}
 		;

parameter_list : parameter_list COMMA type_specifier ID
		{
			outlog<<"At line no: "<<lines<<" parameter_list : parameter_list COMMA type_specifier ID "<<endl<<endl;
			outlog<<$1->getname()<<", "<<$3->getname()<<" "<<$4->getname()<<endl<<endl;
			
			$$ = new symbol_info($1->getname()<<", "<<$3->getname()<<" "<<$4->getname(),"parameter_list");			
		}
		| parameter_list COMMA type_specifier
		{
			outlog<<"At line no: "<<lines<<" parameter_list : parameter_list COMMA type_specifier "<<endl<<endl;
			outlog<<$1->getname()<<", "<<$3->getname()<<endl<<endl;
			
			$$ = new symbol_info($1->getname()<<", "<<$3->getname(),"parameter_list");
		}
		| type_specifier ID
		{
			outlog<<"At line no: "<<lines<<" parameter_list : type_specifier ID "<<endl<<endl;
			outlog<<$1->getname()<<" "<<$2->getname()<<endl<<endl;
			
			$$ = new symbol_info($1->getname()<<" "<<$2->getname(),"parameter_list");
		}
		| type_specifier
		{
			outlog<<"At line no: "<<lines<<" parameter_list : type_specifier "<<endl<<endl;
			outlog<<$1->getname()<<endl<<endl;
			
			$$ = new symbol_info($1->getname(),"parameter_list");
		}
		;

compound_statement : LCURL statements RCURL
			{
				outlog<<"At line no: "<<lines<<" compound_statement : LCURL statements RCURL "<<endl<<endl;
				outlog<<"{"<<$2->getname()<<"}"<<endl<<endl;
			
				$$ = new symbol_info("{"<<$2->getname()<<"}","compound_statement");
			}
			| LCURL RCURL
				outlog<<"At line no: "<<lines<<" compound_statement : LCURL RCURL "<<endl<<endl;
				outlog<<"{}"<<endl<<endl;
			
				$$ = new symbol_info("{}","compound_statement");

var_declaration	: type_specifier declaration_list SEMICOLON
			{
				outlog<<"At line no: "<<lines<<" var_declaration : type_specifier declaration_list SEMICOLON "<<endl<<endl;
				outlog<<$1->getname()<<" "<<$2->getname()<<";"<<endl<<endl;
			
				$$ = new symbol_info($1->getname()<<" "<<$2->getname()<<";","var_declaration");			
			}
			;

type_specifier : INT
			{
				outlog<<"At line no: "<<lines<<" type_specifier : INT "<<endl<<endl;
				outlog<<"int"<<endl<<endl;
			
				$$ = new symbol_info("int","type_specifier");	
			}
			| FLOAT
			{
				outlog<<"At line no: "<<lines<<" type_specifier : FLOAT "<<endl<<endl;
				outlog<<"float"<<endl<<endl;
			
				$$ = new symbol_info("float","type_specifier");				
			}
			| VOID
			{
				outlog<<"At line no: "<<lines<<" type_specifier : VOID "<<endl<<endl;
				outlog<<"void"<<endl<<endl;
			
				$$ = new symbol_info("void","type_specifier");				
			}
			;

declaration_list : declaration_list COMMA ID
			{
				outlog<<"At line no: "<<lines<<" declaration_list : declaration_list COMMA ID "<<endl<<endl;
				outlog<<$1->getname()<<", "<<$3->getname()<<endl<<endl;
			
				$$ = new symbol_info($1->getname()<<", "<<$3->getname(),"declaration_list");
			}
			| declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
			{
				outlog<<"At line no: "<<lines<<" declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD "<<endl<<endl;
				outlog<<$1->getname()<<", "<<$3->getname()<<"["<<$5->getname()<<"]"<<endl<<endl;
			
				$$ = new symbol_info($1->getname()<<", "<<$3->getname()<<"["<<$5->getname()<<"]","declaration_list");			
			}
			| ID
			{
				outlog<<"At line no: "<<lines<<" declaration_list : ID "<<endl<<endl;
				outlog<<$1->getname()<<endl<<endl;
			
				$$ = new symbol_info($1->getname(),"declaration_list");			
			}
			| ID LTHIRD CONST_INT RTHIRD
			{
				outlog<<"At line no: "<<lines<<" declaration_list : ID LTHIRD CONST_INT RTHIRD "<<endl<<endl;
				outlog<<$1->getname()<<"["<<$3->getname()<<"]"<<endl<<endl;
			
				$$ = new symbol_info($1->getname(),"declaration_list");				
			}
			;

statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement
	  {
	    	outlog<<"At line no: "<<lines<<" statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement "<<endl<<endl;
			outlog<<"for("<<$3->getname()<<$4->getname()<<$5->getname()<<")\n"<<$7->getname()<<endl<<endl;
			
			$$ = new symbol_info("for("+$3->getname()+$4->getname()+$5->getname()+")\n"+$7->getname(),"stmnt");
	  }

%%

int main(int argc, char *argv[])
{
	if(argc != 2) 
	{
        cout<<"No file is given"<<endl;
	}
	yyin = fopen(argv[1], "r");
	outlog.open("my_log.txt", ios::trunc);
	
	if(yyin == NULL)
	{
		cout<<"Couldn't open file"<<endl;
		return 0;
	}
    
	yyparse();
	
	printf("Total lines: %d", lines)
	
	outlog.close();
	
	fclose(yyin);
	
	return 0;
}