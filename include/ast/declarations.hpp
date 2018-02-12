#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP

#include "expressions.hpp"
#include <iomanip>


class Statement : public ASTNode{			//TEMPORARY FIX
	protected:
		Statement(Statement *_next = NULL):next(_next){} 
	public:
		Statement *next;
};

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

class Declarator  : public ExternalDeclaration{
	private:
		std::string id;
		Expression *init_expr;

	public:
		Declarator *next;


		std::string getId(){
			return id;
		}

		Declarator(std::string _id = "", Expression *_init_expr = NULL)
		:id(_id),init_expr(_init_expr){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst <<  std::setw(m) << "";
			dst << "Declarator [ ";
			dst << "Id ( " << id << " ) ";
			if(init_expr != NULL){
				dst << ", InitExpr ( ";
				init_expr->print_struct(dst,m);
				dst << " )";
			}
			dst << " ]" << std::endl;

			if(next!=NULL){
				next->print_struct(dst,m);
			}
		}
};

class Declaration : public ExternalDeclaration{
	private:
		std::string type;
		Declarator *dec_list;

	public:
		Declaration *next;		//should this be private?

		Declaration(std::string _type, Declarator *_dec_list = NULL, Declaration *_next = NULL)
		:type(_type),dec_list(_dec_list),next(_next){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst <<  std::setw(m) << "";
			dst << "Declaration [" << std::endl;

			dst <<  std::setw(m+2) << "";
			dst << "Type( " << type << " ), " << std::endl;
			if(dec_list != NULL){
				dec_list->print_struct(dst,m+2);
			}
			dst <<  std::setw(m) << "";
			dst << "]" << std::endl;
			if(next != NULL){
				next->print_struct(dst,m);
			}

		}
};

class FunctionDefinition : public ExternalDeclaration{
	private:
		std::string type;
		std::string id;
		Declaration *p_list;
		Statement 	*s_ptr;

	public:
		FunctionDefinition(std::string _type, std::string _id, Declaration *_p_list , Statement *_s_ptr )
		:type(_type), id(_id), p_list(_p_list), s_ptr(_s_ptr){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "FunctionDefinition [ " << std::endl;

			dst <<  std::setw(m+2) << "";
			dst << "Type( " << type << " )"<<  std::endl;

			dst <<  std::setw(m+2) << "";
			dst << "Identifier( " << id << " )" << std::endl ;

			if(p_list != NULL){
				dst <<  std::setw(m+2) << "";
				dst << "ParameterList (" << std::endl;
				p_list->print_struct(dst,m+4);
			}

			dst <<  std::setw(m+2) << "";
			dst << ")" << std::endl;

			s_ptr->print_struct(dst,m+2);
			dst << "]" << std::endl;
		}
};








#endif