#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP

#include "expressions.hpp"
#include <iomanip>
#include <sstream>

class Statement : public ASTNode{			//TEMPORARY FIX might consider using inline in the future
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
};

class Declarator  : public ExternalDeclaration{
public:
	virtual std::string getId() const{
		return "";
	}
	virtual int getSize() const{
		return 1;
	}
	virtual dType getDtype() const{
		return Basic;
	}
	virtual void to_mips_declare(std::ostream &dst, Context& ctx, Type t) const{}
	virtual void to_mips_declare_init(std::ostream &dst, Context& ctx,Type t, std::vector<Expression*>* init) const{}
	virtual void to_mips_declare_init(std::ostream &dst, Context& ctx,Type t, Expression* init) const{}
};

class ArrayDeclarator : public Declarator{
private:
	std::string id;
	Expression *size_expr; 

public:
	ArrayDeclarator(std::string _id, Expression *_size_expr)
	:id(_id),size_expr(_size_expr){}

	virtual void to_mips_declare(std::ostream &dst, Context& ctx,Type type) const override{ //normal non init array
		int size = getSize();
		type.inc_pLevel();
		Variable var = ctx.assignNewVariable(id,type,Array,size);
		if(size ==-1){
			std::cout << "Erorr : Array Size Missing" << std::endl;
			exit(1);
		}
		if(ctx.getScope() == global){
			dst<<".data"<<std::endl;	
			dst<<".globl "<<id<<std::endl;
			dst<<id<<":"<<std::endl;
			for(int i=0;i<size;i++){
				dst<<"."<<type.storage_type()<<" 0"<<std::endl;
			}	
		}
		else if(ctx.getScope() == local){
			for(int i=0;i<size;i++){
				int offset = var.getAddr() + (i*type.bytes());
				ctx.memoryOffsetWrite(type,"0","fp", offset,dst);
			}
		}
	}
	virtual void to_mips_declare_init(std::ostream &dst, Context& ctx,Type type, std::vector<Expression*>* init) const override{ //normal init array
		int size = init->size();
		type.inc_pLevel();
		ctx.assignNewVariable(id,type,Array,size);
		Variable var = ctx.getVariable(id);
		if(ctx.getScope() == global){
			dst<<".data"<<std::endl;	
			dst<<".globl "<<id<<std::endl;
			dst<<id<<":"<<std::endl;
			for(auto it=init->begin();it!=init->end();it++){
				dst<<"."<<type.storage_type()<< " "<<(*it)->eval()<<std::endl;
			}				
		}
		else if(ctx.getScope() == local){
			for(int i=0;i<size;i++){
				auto tempMemReg = ctx.assignNewStorage();
				std::string tempReg = "v0";
				(*init)[i]->to_mips(dst,ctx);
				ctx.deAllocStorage();
				ctx.memReg_read(tempMemReg,tempReg,dst);
				int offset = var.getAddr()+(i*type.bytes());
				ctx.memoryOffsetWrite(type,tempReg,"fp", offset,dst);
			}
		}	
	}
	virtual void to_mips_declare_init(std::ostream &dst, Context& ctx,Type type, Expression* init) const override{	//some string array 
		//know to be a string literal...	
		int size = 0;

		std::stringstream ss;
		init->to_c(ss,"");
		std::string str = ss.str();
		int array_size = getSize();
		str = str.substr(1,str.size()-2); //remove " "
		int str_size = str.size();


		if(array_size == -1){
			size = str_size+1; //null termiantor...
			str = str + "\0";
		}
		else{
			size = array_size;
			str = str.substr(0,array_size);
		}

		ctx.assignNewVariable(id,type,Array,size);
		Variable var = ctx.getVariable(id);

		if(ctx.getScope() == global){
			dst<<".data"<<std::endl;	
			dst<<".globl "<<id<<std::endl;
			dst<<id<<":"<<std::endl;
			dst<<".ascii "<<str<<std::endl;
		}
		else if(ctx.getScope() == local){
			for(int i=0;i<size;i++){
				auto tempMemReg = ctx.assignNewStorage();
				std::string tempReg = "v0";
				init->to_mips(dst,ctx);
				ctx.deAllocStorage();
				ctx.memReg_read(tempMemReg,tempReg,dst);
				int offset = var.getAddr()+(i*type.bytes());
				ctx.memoryOffsetWrite(type,tempReg,"fp", offset,dst);
			}
		}	
	}
	virtual void to_mips(std::ostream &dst, Context& ctx) const override{
		Variable var = ctx.getVariable(id);
		Type type = var.getType();
		int size = getSize();

		if(size ==-1){
			std::cout << "Erorr : Array Size Missing" << std::endl;
			exit(1);
		}
		
		if(ctx.getScope() == global){
			dst<<".data"<<std::endl;	
			dst<<".globl "<<id<<std::endl;
			dst<<id<<":"<<std::endl;
			for(int i=0;i<size;i++){
				dst<<"."<<type.storage_type()<< " 0"<<std::endl;
			}	
		}
		else if(ctx.getScope() == local){
			for(int i=0;i<size;i++){
				int offset = var.getAddr() + (i*type.bytes());
				ctx.memoryOffsetWrite(type,"0","fp", offset,dst);
			}
		}
	}
	virtual int getSize() const override {
		if(size_expr!=NULL)
			return size_expr->eval();
		return -1;
	}
	virtual std::string getId() const override{
		return id;
	}
	virtual dType getDtype() const override{
		return Array;
	}
};


class IdentifierDeclarator  : public Declarator{
private:
	std::string id;

public:
	IdentifierDeclarator(std::string _id):id(_id){}


	virtual void to_mips_declare(std::ostream &dst, Context& ctx,Type type) const override{ //normal identifier, un-init
		Variable var = ctx.assignNewVariable(id,type,Basic,1);
		if(ctx.getScope() == global){
			dst<<".data"<<std::endl;	
			dst<<".globl "<<id<<std::endl;
			dst<<id<<":"<<std::endl;
			dst<<"."<<type.storage_type()<< " 0"<<std::endl;
		}
		else if(ctx.getScope() == local){
			ctx.memoryOffsetWrite(type,"0","fp",var.getAddr(),dst);	
		}
	}
	virtual void to_mips_declare_init(std::ostream &dst, Context& ctx,Type type, Expression* init_exp) const override{ //init identifier
		Variable var = ctx.assignNewVariable(id,type,Basic,1);
		if(ctx.getScope() == global){
			int init_val = init_exp->eval(); //global only allows constant init
			dst<<".data"<<std::endl;	
			dst<<".globl "<<id<<std::endl;
			dst<<id<<":"<<std::endl;
			dst<<"."<<type.storage_type()<< " "<<init_val<<std::endl;
		}
		else if(ctx.getScope() == local){
			auto tempMemReg = ctx.assignNewStorage();
			std::string tempReg = "v1";
			init_exp->to_mips(dst,ctx);
			ctx.deAllocStorage();
			ctx.convertMemRegType(init_exp->exprType(ctx),type,tempMemReg,dst);
			ctx.memReg_read(tempMemReg,tempReg,dst);
			ctx.memoryOffsetWrite(type,tempReg,"fp",var.getAddr(),dst);
		}
	}

	virtual void to_mips(std::ostream &dst, Context& ctx) const override{
		Variable var = ctx.getVariable(id);
		Type type = var.getType();

		if(ctx.getScope() == global){
			dst<<".data"<<std::endl;	
			dst<<".globl "<<id<<std::endl;
			dst<<id<<":"<<std::endl;
			dst<<"."<<type.storage_type()<< " 0"<<std::endl;
		}
		else if(ctx.getScope() == local){
			ctx.memoryOffsetWrite(type,"0","fp",var.getAddr(),dst);	
		}
	}
	virtual std::string getId() const override{
		return id;
	}

	virtual void to_c(std::ostream &dst,std::string indent) const override{
		dst << indent << id;
	}
	virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
		dst << indent << id << "=";
		dst << "0";;
		dst << std::endl;
		tc.global_var.push_back(id);
	}
};



class InitIdentifierDeclarator  : public Declarator{
private:
		Declarator* dec;
		Expression* init_expr;

public:
	InitIdentifierDeclarator(Declarator* _dec, Expression* _init_expr)
	:dec(_dec),init_expr(_init_expr){}

	virtual void to_mips_declare(std::ostream &dst, Context& ctx,Type t) const override{
		dec->to_mips_declare_init(dst,ctx,t,init_expr);
	}

	virtual std::string getId() const{
		return dec->getId();
	}
	virtual dType getDtype() const override{
		return dec->getDtype();
	}

	virtual void to_c(std::ostream &dst,std::string indent) const override{
		dst << indent << getId();
		dst << " =";
		init_expr->to_c(dst,"");
	}
	virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
		std::string id = dec->getId();
		dst << indent << id << "=";
		init_expr->to_python(dst,"",tc);
		dst << std::endl;
		tc.global_var.push_back(id);
	}
};

class InitArrayDeclarator : public Declarator{
private:
	Declarator* dec;
	std::vector<Expression*>* init_list;

public:
	InitArrayDeclarator(Declarator* _dec, std::vector<Expression*>* _init_list)
	:dec(_dec),init_list(_init_list){}


	virtual void to_mips_declare(std::ostream &dst, Context& ctx,Type t) const override{
		dec->to_mips_declare_init(dst,ctx,t,init_list);
	}

	virtual void to_mips(std::ostream &dst, Context& ctx) const override{
		std::string id = getId();
		Variable var = ctx.getVariable(id);
		Type type = var.getType();
		int size = getSize();

		if(ctx.getScope() == global){
			dst<<".data"<<std::endl;	
			dst<<".globl "<<id<<std::endl;
			dst<<id<<":"<<std::endl;
			for(auto it=init_list->begin();it!=init_list->end();it++){
				dst<<"."<<type.storage_type()<< " "<<(*it)->eval()<<std::endl;
			}				
		}
		else if(ctx.getScope() == local){
			for(int i=0;i<size;i++){
				auto tempMemReg = ctx.assignNewStorage();
				std::string tempReg = "v0";
				(*init_list)[i]->to_mips(dst,ctx);
				ctx.deAllocStorage();
				ctx.memReg_read(tempMemReg,tempReg,dst);

				int offset = var.getAddr()+(i*type.bytes());
				ctx.memoryOffsetWrite(type,tempReg,"fp", offset,dst);
			}
		}	
	}

	virtual std::string getId() const override{
		return dec->getId();
	}

	virtual int getSize() const override{
		int size = dec->getSize();
		if(size != -1){	
			if ((unsigned)size != init_list->size()){
				std::cout << "Error : Array initializer size does not match Array size" <<std::endl;
				exit(1);
			}
		}
		else{
			size = init_list->size();
		}
		return size;
	}
	virtual dType getDtype() const{
		return Array;
	}
};

class Declaration : public ExternalDeclaration{
private:
	Type type;
	std::vector<Declarator*>* dec_list;

public:
	Declaration(Type _type, std::vector<Declarator*>* _dec_list = NULL)
	:type(_type),dec_list(_dec_list){}

	virtual void to_mips(std::ostream &dst, Context& ctx) const override{
		if(dec_list != NULL){
			for(auto it=dec_list->begin();it!=dec_list->end();it++){
				(*it)->to_mips_declare(dst,ctx,type);
			}
		}
	}

	std::string getParam(){
		return (*dec_list)[0]->getId();
	}
	dType getParam_dtype(){
		return (*dec_list)[0]->getDtype();
	}
	Type getParam_type(){
		return type;
	}

	virtual void to_c(std::ostream &dst,std::string indent) const override{
	/*	dst << indent << type << " ";
		if(dec_list != NULL){
			for(auto it=dec_list->begin();it!=dec_list->end();it++){
				(*it)->to_c(dst,"");
				if(next(it,1) != dec_list->end()) dst << ",";
			}
		}*/
	}

	virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
		if(dec_list != NULL){
			for(auto it=dec_list->begin();it!=dec_list->end();it++){
				(*it)->to_python(dst,indent,tc);
				//if(next(it,1) != dec_list->end()) dst << std::endl;
			}
		}
	}

};


class FunctionDefinition : public ExternalDeclaration{
private:
	Type* type;
	std::string id;
	std::vector<Declaration*>*	p_list;
	Statement*	s_ptr;

public:
	FunctionDefinition(Type* _type, std::string _id,std::vector<Declaration*>* _p_list , Statement *_s_ptr )
	:type(_type), id(_id), p_list(_p_list), s_ptr(_s_ptr){}

	virtual void to_mips(std::ostream &dst, Context& ctx) const override{
		ctx.scopeLocal();
		ctx.return_label = ctx.generateLabel("RETURN");
			
		dst<<"# Start Prologue #"<<std::endl;
		dst<<".text"<<std::endl;	
		dst<<".globl "<<id<<std::endl;
		dst<<id<<":"<<std::endl;

		dst<<".set noreorder"<<std::endl;
		dst<<".set nomacro"<<std::endl;

		dst<<"sw $31,-4($sp)"<<std::endl; // store return address
		dst<<"sw $fp,-8($sp)"<<std::endl; // store old fp
		dst<<"addiu $sp,$sp,-8"<<std::endl;
		dst<<"move $fp,$sp"<<std::endl;
		dst<<"# End Prologue #"<<std::endl;
		
		if(p_list!=NULL){
			for(unsigned int i=0;i<p_list->size();i++){
				if(i<4) dst<<"sw $a"<<i<<","<<(i*4+8)<<"($fp)"<<std::endl;	
				ctx.assignNewArgument( (*p_list)[i]->getParam(),(*p_list)[i]->getParam_type() ,Basic,i*4+8);
			}
		}

		if(s_ptr!=NULL){
			s_ptr->to_mips(dst,ctx);
		}

		dst<<ctx.return_label<<":"<<std::endl;
		dst<<"# Start Epilouge #"<<std::endl;
		dst<<"addiu $sp,$sp,8"<<std::endl;
		dst<<"lw $31,-4($sp)"<<std::endl; //restore return address
		dst<<"lw $fp,-8($sp)"<<std::endl; //restor old fp
		dst<<"j $31"<<std::endl;
		dst<<"nop"<<std::endl;
		dst<<std::endl;
		dst<<"# End Epilouge #"<<std::endl;

		dst<<".set macro"<<std::endl;
		dst<<".set reorder"<<std::endl;


		ctx.scopeGlobal();
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

	virtual void to_c(std::ostream &dst,std::string indent) const override{
	/*	dst << type << " " << id << "(";
		if(p_list != NULL){
			for(auto it=p_list->begin();it!=p_list->end();it++){
				(*it)->to_c(dst,"");
				if(next(it,1) != p_list->end()) dst << ", ";
			}
		}
		dst <<")";
		if(s_ptr != NULL){
			s_ptr->to_c(dst,indent);
		}*/
	}
	
};

#endif