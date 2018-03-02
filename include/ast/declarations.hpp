#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP

#include "expressions.hpp"
#include <iomanip>
#include <sstream>

class Statement : public ASTNode{			//TEMPORARY FIX might consider using inline in the future
	public:
		virtual void to_mips_switch(std::ostream &dst, Context& ctx) const{
			//do nothing by default
		}
};

class TranslationUnit : public ASTNode{
	protected:
		astNodePtr left;
		astNodePtr right;

	public:
		TranslationUnit(astNodePtr _left, astNodePtr _right):left(_left),right(_right){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			left->to_mips(dst,ctx);
			right->to_mips(dst,ctx);
		}

		virtual void print_struct(std::ostream &dst, int m) const override{
			left->print_struct(dst,m);
			dst << std::endl;
			right->print_struct(dst,m);
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			left->to_c(dst,indent);
			dst << ";";
			dst << std::endl;
			right->to_c(dst,indent);
			dst << ";";
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			left->to_python(dst,indent,tc);
			right->to_python(dst,indent,tc);
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
		std::string getId(){
			return id;
		}

		Declarator(std::string _id = "", Expression *_init_expr = NULL)
		:id(_id),init_expr(_init_expr){}


		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			if(ctx.getScope() == global){
				if(init_expr!=NULL){
				/*	std::stringstream ss;
					init_expr->to_c(ss,"");
					dst<<"    "<<".data"<<std::endl;	
					dst<<"    "<<".globl "<<id<<std::endl;
					dst<<id<<":"<<std::endl;
					dst<<"    "<<".word "<<ss.str()<<std::endl;
					ctx.set_binding(id,".globl");*/
				}
				else{
					/*dst<<"    "<<".comm "<<id<<",4,4"<<std::endl;	
					ctx.set_binding(id,".comm");*/
				}
			}
			else{
				ctx.assignNewVariable(id);
				dst<<"sw $0,"<<ctx.getVariable(id).first<<"($fp)"<<std::endl;
				if(init_expr!=NULL){
					auto tempReg = ctx.assignNewStorage();
					std::string tempReg_r = "v1";
					init_expr->to_mips(dst,ctx);
					dst<<"lw $v1,"<<tempReg<<"($fp)"<<std::endl;
					dst<<"sw $"<<tempReg_r<<","<<ctx.getVariable(id).first<<"($fp)"<<std::endl;
					ctx.deAllocStorage();
				}
			}
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst << indent << id;
			if(init_expr!=NULL){
				dst << " =";
				init_expr->to_c(dst,"");
			}
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			dst << indent << id << "=";
			if(init_expr!=NULL) init_expr->to_python(dst,"",tc);
			else dst << "0";
			dst << std::endl;
			tc.global_var.push_back(id);
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


		std::string getParam(){
			return (*dec_list)[0]->getId();
		}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			if(dec_list != NULL){
				if(ctx.getScope() == local){
					for(auto it=dec_list->begin();it!=dec_list->end();it++){
						(*it)->to_mips(dst,ctx);
					}
				}
				
			}
		}

		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst << indent << type << " ";
			if(dec_list != NULL){
				for(auto it=dec_list->begin();it!=dec_list->end();it++){
					(*it)->to_c(dst,"");
					if(next(it,1) != dec_list->end()) dst << ",";
				}
			}
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			if(dec_list != NULL){
				for(auto it=dec_list->begin();it!=dec_list->end();it++){
					(*it)->to_python(dst,indent,tc);
					//if(next(it,1) != dec_list->end()) dst << std::endl;
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

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			ctx.scopeLocal();
			//std::string epilouge_label = ctx.generateLabel("epilouge");
			
			dst<<"# Start Prologue #"<<std::endl;
			dst<<".text"<<std::endl;	
			dst<<".globl "<<id<<std::endl;
			dst<<id<<":"<<std::endl;
			dst<<"sw $31,-4($sp)"<<std::endl; //return address
			dst<<"sw $fp,-8($sp)"<<std::endl; // old fp
			dst<<"addiu $sp,$sp,-8"<<std::endl;
			dst<<"move $fp,$sp"<<std::endl;
			dst<<"# End Prologue #"<<std::endl;

			
			if(p_list!=NULL){
				for(unsigned int i=0;i<p_list->size();i++){
					if(i<4)
						dst<<"    "<<"sw $a"<<i<<","<<(i*4+8)<<"($fp)"<<std::endl;	
					ctx.assignNewArgument( (*p_list)[i]->getParam() , i*4+8 );
				}
			}

			if(s_ptr!=NULL){
				s_ptr->to_mips(dst,ctx);
			}


			//dst<<epilouge_label<<":"<<std::endl;
			/*dst<<"# Start Epilouge #"<<std::endl;
			dst<<"addiu $sp,$sp,8"<<std::endl;	//asume one var for now just for testing
			dst<<"lw $31,-4($sp)"<<std::endl; //restore return address
			dst<<"lw $fp,-8($sp)"<<std::endl; // restor old fp
			dst<<"j $31"<<std::endl;
			dst<<"nop"<<std::endl;
			dst<<std::endl;
			dst<<"# End Epilouge #"<<std::endl;*/
		}


		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst << type << " " << id << "(";
			if(p_list != NULL){
				for(auto it=p_list->begin();it!=p_list->end();it++){
					(*it)->to_c(dst,"");
					if(next(it,1) != p_list->end()) dst << ", ";
				}
			}
			dst <<")";
			if(s_ptr != NULL){
				s_ptr->to_c(dst,indent);
			}
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			dst << indent << "def " << id << "(";
			if(p_list != NULL){
				for(auto it=p_list->begin();it!=p_list->end();it++){
					dst << (*it)->getParam();
					if(next(it,1) != p_list->end()) dst << ",";
				}
			}
			dst << ")" << ":" << std::endl;
			for(auto it=tc.global_var.begin(); it!=tc.global_var.end();it++){
				dst << "  global " << *it << std::endl;
			}
			if(s_ptr != NULL){
				s_ptr->to_python(dst,indent+"  ",tc);	
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
			}

			dst <<  std::setw(m+2) << "";
			dst << ")" << std::endl;

			s_ptr->print_struct(dst,m+2);
			dst << "]" << std::endl;
		}
};

#endif