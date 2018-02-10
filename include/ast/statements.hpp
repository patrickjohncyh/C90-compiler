#ifndef STATEMENTS_HPP
#define STATEMENTS_HPP

#include "expressions.hpp"
#include <iomanip>


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

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << std::setw(m) << "";
			dst << "ExpressionStatement [ ";
			expr->print_struct(dst,m);
			dst << " ]" << std::endl;

			if(next!=NULL){
				next->print_struct(dst,m);
			}
		}

};


class CompoundStatement : public Statement{

	private:
		Statement *s_list;

	public:
		CompoundStatement( Statement* _s_list = NULL, Statement *_next = NULL ):Statement(_next),s_list(_s_list){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst <<  std::setw(m) << "";
			dst << "CompoundStatement [" << std::endl;
			if(s_list != NULL){
				s_list->print_struct(dst,m+2);
				dst << std::setw(m) << "";
				dst << "]" << std::endl;;
			}
			if(next!=NULL){
				next->print_struct(dst,m);
			}
		}


};

class JumpStatement : public Statement{

	private:
		std::string str;
		Expression* expr;

	public:

		JumpStatement( std::string _str, Expression* _expr = NULL, Statement *_next = NULL):Statement(_next),str(_str),expr(_expr){
			
		}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << std::setw(m) << "";
			dst << "JumpStatement [ " << str;
			dst << " ";
			expr->print_struct(dst,m);
			dst << "; ]" << std::endl;
		}

};




#endif