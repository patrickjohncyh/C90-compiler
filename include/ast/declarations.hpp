#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP

#include "statements.hpp"
#include "expressions.hpp"

class TranslationUnit : public ASTNode{
	protected:
		astNodePtr left;
		astNodePtr right;

	private:

	public:
		TranslationUnit(astNodePtr _left, astNodePtr _right):left(_left),right(_right){}

		virtual void print(std::ostream &dst) const override{
				left->print(dst);
				dst << "\n";
				right->print(dst);

		}
};

class ExternalDeclaration : public ASTNode{
	virtual void print(std::ostream &dst) const =0;
};



class FunctionDefinition : public ExternalDeclaration{
	private:
		std::string type;
		std::string id;
		Statement *s_ptr;

	public:
		FunctionDefinition(std::string _type, std::string _id, Statement *_s_ptr ): type(_type), id(_id), s_ptr(_s_ptr){}

		virtual void print(std::ostream &dst) const override{
			dst << "Type( " << type << " ) " << "Identifier( " << id << " )\n";
			s_ptr->print(dst);
		}
};

class Declaration : public ExternalDeclaration{
	private:
		std::string type;
		std::string id;
		Expression *init_expr;

	public:
		Declaration(std::string _type, std::string _id = "", Expression *_init_expr = NULL): type(_type),id(_id),init_expr(_init_expr){}

		virtual void print(std::ostream &dst) const override{
			dst << "Declaration( Type( " << type << " ), " << "Identifier ( " << id << " ), " << " Init ( ";
			if(init_expr != NULL){
				init_expr->print(dst);
				dst << " )";
			}
			dst << " )" ;
		}
};
















#endif