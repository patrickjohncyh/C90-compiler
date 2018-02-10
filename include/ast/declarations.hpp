#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP

#include "statements.hpp"
#include "expressions.hpp"
#include <iomanip>

class TranslationUnit : public ASTNode{
	protected:
		astNodePtr left;
		astNodePtr right;

	private:

	public:
		TranslationUnit(astNodePtr _left, astNodePtr _right):left(_left),right(_right){}

		virtual void print_struct(std::ostream &dst, int m) const override{
				left->print_struct(dst,m);
				dst << "\n";
				right->print_struct(dst,m);
		}
};

class ExternalDeclaration : public ASTNode{
	virtual void print_struct(std::ostream &dst, int m) const =0;
};



class FunctionDefinition : public ExternalDeclaration{
	private:
		std::string type;
		std::string id;
		Statement *s_ptr;

	public:
		FunctionDefinition(std::string _type, std::string _id, Statement *_s_ptr ): type(_type), id(_id), s_ptr(_s_ptr){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "FunctionDefinition [ ";
			dst << "Type( " << type << " ) " << "Identifier( " << id << " )" << std::endl ;
			s_ptr->print_struct(dst,m+2);
			dst << "]" << std::endl;
		}
};


class Declaration : public ExternalDeclaration{
	private:
		std::string type;
		std::string id;
		Expression *init_expr;


	public:

		Declaration *next;

		Declaration(std::string _type, std::string _id = "", Expression *_init_expr = NULL, Declaration *_next = NULL)
		:type(_type),id(_id),init_expr(_init_expr),next(_next){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "Declaration [ Type( " << type << " ), " << "Identifier ( " << id << " ), " << " InitExpr ( ";
			if(init_expr != NULL){
				init_expr->print_struct(dst,m);
				dst << " )";
			}
			dst << " ]" ;
		}
};








#endif