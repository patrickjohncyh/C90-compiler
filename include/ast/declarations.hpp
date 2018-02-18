#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP

#include "expressions.hpp"
#include <iomanip>


class Statement : public ASTNode{			//TEMPORARY FIX might consider using inline in the future
	public:
		virtual void to_python(std::ostream &dst, std::string indent) const override{
			std::cerr<<"ASTNode::translate is not implemented by type "<<typeid(this).name()<<"\n";
		}
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

		virtual void to_python(std::ostream &dst, std::string indent) const override{
			left->to_python(dst,indent);
			dst << std::endl;
			right->to_python(dst,indent);
		}


};

class ExternalDeclaration : public ASTNode{
	virtual void print_struct(std::ostream &dst, int m) const =0;
	virtual void to_python(std::ostream &dst, std::string indent) const=0;
};

class Declarator  : public ExternalDeclaration{
	private:
		std::string id;
		Expression *init_expr;

	public:
		std::string getId(){
			return id;
		}

		Declarator(std::string _id = "", Expression *_init_expr = NULL)
		:id(_id),init_expr(_init_expr){}

		virtual void to_python(std::ostream &dst, std::string indent) const override{
			dst << indent << id << " =";
			if(init_expr!=NULL) init_expr->to_python(dst," ");
			else dst << " 0";
		}

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
		}
};

class Declaration : public ExternalDeclaration{
	private:
		std::string type;
		std::vector<Declarator*>* dec_list;

	public:
		Declaration(std::string _type, std::vector<Declarator*>* _dec_list = NULL)
		:type(_type),dec_list(_dec_list){}

		virtual void to_python(std::ostream &dst, std::string indent) const override{
			if(dec_list != NULL){
				for(auto it=dec_list->begin();it!=dec_list->end();it++){
					(*it)->to_python(dst,indent);
					if(next(it,1) != dec_list->end()) dst << std::endl;
				}
			}
		}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst <<  std::setw(m) << "";
			dst << "Declaration [" << std::endl;

			dst <<  std::setw(m+2) << "";
			dst << "Type( " << type << " ), " << std::endl;
			if(dec_list != NULL){
				//dec_list->print_struct(dst,m+2);
			}
			dst <<  std::setw(m) << "";
			dst << "]" << std::endl;
		}
};

class FunctionDefinition : public ExternalDeclaration{
	private:
		std::string type;
		std::string id;
		std::vector<Declaration*>*	p_list;
		Statement*	s_ptr;

	public:
		FunctionDefinition(std::string _type, std::string _id,std::vector<Declaration*>* _p_list , Statement *_s_ptr )
		:type(_type), id(_id), p_list(_p_list), s_ptr(_s_ptr){}

		virtual void to_python(std::ostream &dst, std::string indent) const override{
			dst << indent << "def " << id << "(";
			if(p_list != NULL){
				for(auto it=p_list->begin();it!=p_list->end();it++){
					(*it)->to_python(dst,"");
					if(next(it,1) != p_list->end()) dst << ",";
				}
			}
			dst << ")" << ":" << std::endl;
			if(s_ptr != NULL){
				s_ptr->to_python(dst,indent+"  ");	
			}
		}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "FunctionDefinition [ " << std::endl;

			dst <<  std::setw(m+2) << "";
			dst << "Type( " << type << " )"<<  std::endl;

			dst <<  std::setw(m+2) << "";
			dst << "Identifier( " << id << " )" << std::endl ;

			if(p_list != NULL){
				dst <<  std::setw(m+2) << "";
				dst << "ParameterList (" << std::endl;
		//		p_list->print_struct(dst,m+4);
			}

			dst <<  std::setw(m+2) << "";
			dst << ")" << std::endl;

			s_ptr->print_struct(dst,m+2);
			dst << "]" << std::endl;
		}
};


#endif