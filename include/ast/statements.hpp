#ifndef STATEMENTS_HPP
#define STATEMENTS_HPP

#include "expressions.hpp"


class Statement : public ASTNode{

protected:

	Statement(Statement *_next = NULL):next(_next){} 
public:
	Statement *next;
};



class ExprStatement : public Statement{

	private:
		Expression *expr;

	public:
		ExprStatement( Expression* _expr, Statement *_next = NULL):Statement(_next),expr(_expr){}

		virtual void print(std::ostream &dst) const override{
			dst << "ExpressionStatement ( ";
			expr->print(dst);
			dst << " )\n";
			if(next!=NULL){
				next->print(dst);
			}
		}

};


class CompoundStatement : public Statement{

	private:
		Statement *s_list;

	public:
		CompoundStatement( Statement* _s_list = NULL, Statement *_next = NULL ):Statement(_next),s_list(_s_list){}

		virtual void print(std::ostream &dst) const override{
			if(s_list != NULL){
				dst << "CompoundStatement ( \n";
				s_list->print(dst);
				dst << ")\n";
			}
			if(next!=NULL){
				next->print(dst);
			}
		}


};

class JumpStatement : public Statement{

	private:
		std::string str;

	public:

		JumpStatement( std::string _str, Statement *_next = NULL):Statement(_next),str(_str){
			
		}

		virtual void print(std::ostream &dst) const override{
			dst << "JumpStatement ( " << str << "; )";
		}

};




#endif