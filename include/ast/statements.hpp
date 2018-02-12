#ifndef STATEMENTS_HPP
#define STATEMENTS_HPP

#include "expressions.hpp"
#include "declarations.hpp"
#include <iomanip>


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
		Declaration *d_list;

	public:
		CompoundStatement( Declaration *_d_list = NULL, Statement* _s_list = NULL, Statement *_next = NULL )
		:Statement(_next),s_list(_s_list),d_list(_d_list){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst <<  std::setw(m) << "";
			dst << "CompoundStatement [" << std::endl;
			if(d_list != NULL){
				d_list->print_struct(dst,m+2);
			}
			if(s_list != NULL){
				s_list->print_struct(dst,m+2);
			}
			dst << std::setw(m) << "";
			dst << "]" << std::endl;
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
		JumpStatement( std::string _str, Expression* _expr = NULL, Statement *_next = NULL)
		:Statement(_next),str(_str),expr(_expr){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << std::setw(m) << "";
			dst << "JumpStatement [ " << str;
			dst << " ";
			if(expr!=NULL){
				expr->print_struct(dst,m);
			}
			dst << "; ]" << std::endl;
		}
};




#endif