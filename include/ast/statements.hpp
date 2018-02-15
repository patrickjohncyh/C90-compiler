#ifndef STATEMENTS_HPP
#define STATEMENTS_HPP

#include "expressions.hpp"
#include "declarations.hpp"
#include <iomanip>

class ExprStatement : public Statement{
	private:
		Expression *expr;

	public:
		ExprStatement( Expression* _expr ):expr(_expr){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << std::setw(m) << "";
			dst << "ExpressionStatement [ ";
			expr->print_struct(dst,m);
			dst << " ]" << std::endl;
		}
};


class CompoundStatement : public Statement{
	private:
		std::vector<Statement*>* s_list;
		std::vector<Declaration*>* d_list;

	public:
		CompoundStatement(std::vector<Declaration*>* _d_list = NULL ,std::vector<Statement*>* _s_list = NULL)
		:s_list(_s_list),d_list(_d_list){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst <<  std::setw(m) << "";
			dst << "CompoundStatement [" << std::endl;
			if(d_list != NULL){			//print declaration list
				for(auto it=d_list->begin();it!=d_list->end();it++){
					(*it)->print_struct(dst,m+2);
				}
			}
			if(s_list !=NULL){			//print statement list
				for(auto it=s_list->begin();it!=s_list->end();it++){
					(*it)->print_struct(dst,m+2);
				}
			}
			dst << std::setw(m) << "";
			dst << "]" << std::endl;
		}
};

class ConditionIfStatement : public Statement{
	private:
		Expression* cond_expr;
		Statement* s_true;
		
	public:
		ConditionIfStatement(Expression* _cond_expr, Statement* _s_true)
		:cond_expr(_cond_expr),s_true(_s_true){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << std::setw(m) << "";
			dst << "IfStatement [ " ;
			cond_expr->print_struct(dst,m+2);
			dst << std::endl;
			s_true->print_struct(dst,m+2);
			dst << std::setw(m) << "";
			dst << " ]" << std::endl;
		}

};


class ConditionIfElseStatement : public Statement{
	private:
		Expression* cond_expr;
		Statement* s_true;
		Statement* s_false;
	public:
		ConditionIfElseStatement(Expression* _cond_expr, Statement* _s_true, Statement* _s_false)
		:cond_expr(_cond_expr),s_true(_s_true),s_false(_s_false){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << std::setw(m) << "";
			dst << "IfElseStatement [ " ;
			cond_expr->print_struct(dst,m+2);
			dst << std::endl;

			dst << std::setw(m+2) << "";
			dst << "ConditionTrue [ " << std::endl;
			s_true->print_struct(dst,m+4);
			dst << std::setw(m+2) << "";
			dst << "]" << std::endl;

			dst << std::setw(m+2) << "";
			dst << "ConditionFalse [ " << std::endl;
			s_false->print_struct(dst,m+4);
			dst << std::setw(m+2) << "";
			dst << "]" << std::endl;

			dst << std::setw(m) << "";
			dst << "]" << std::endl;
		}


};

class JumpStatement : public Statement{
	private:
		Expression* expr;

	public:
		JumpStatement( Expression* _expr = NULL)
		:expr(_expr){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << std::setw(m) << "";
			dst << "JumpStatement [ ";
			if(expr!=NULL){
				expr->print_struct(dst,m);
			}
			dst << "; ]" << std::endl;
		}
};




#endif