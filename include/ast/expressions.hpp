#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <iomanip>
#include <vector>


class Expression : public ASTNode{
	public:
		virtual void print_struct(std::ostream &dst, int m) const =0;
		virtual void to_mips_getAddr(std::ostream &dst, Context ctx) const{
			dst << "ERROR Unassignable" << std::endl;
		}
};


/********************** Unary Expressions ************************/

class UnaryExpression : public Expression{
	protected:
		Expression* expr;
	public:
		UnaryExpression(Expression* _expr):expr(_expr){}
};

/********************** Post Fix Expressions *********/


class PostIncrementExpression : public UnaryExpression{
	public:
		PostIncrementExpression(Expression* _expr):UnaryExpression(_expr){}

		virtual void to_c(std::ostream &dst,std::string indent) const override{
			expr->to_c(dst,indent);
			dst<< "++";
		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			expr->print_struct(dst,m);
			dst << "++" << std::endl;
		}
};


class FunctionCallExpression : public UnaryExpression{
	private:
		std::vector<Expression*>* a_list;

	public:

		FunctionCallExpression(Expression *_expr , std::vector<Expression*>* _a_list = NULL)
		:UnaryExpression(_expr),a_list(_a_list){}


		virtual void to_c(std::ostream &dst,std::string indent) const override{
			expr->to_c(dst,indent);
			dst << "(";
			if(a_list != NULL){
				for(auto it=a_list->begin();it!=a_list->end();it++){
					(*it)->to_c(dst,"");
					if(next(it,1)!=a_list->end()){
						dst << ",";
					}
				}
			}
			dst << ")";
		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "FunctionCallExpression [ Identifier ( ";  expr->print_struct(dst,m); dst << " ) ";
			if(a_list->size() !=0){
				for(auto it = a_list->begin(); it != a_list->end(); it++){
					(*it)->print_struct(dst,m);
				}
			}
			dst << "]" << std::endl;
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			expr->to_python(dst,indent,tc);
			dst << "(";
			if(a_list != NULL){
				for(auto it=a_list->begin();it!=a_list->end();it++){
					(*it)->to_python(dst,"",tc);
					if(next(it,1)!=a_list->end()){
						dst << ",";
					}
				}
			}
			dst << ")";
		}
};


// TO IMPLEMENT OTHER POSTFIX EXPRESSIONS


/********************** Pre Fix Expressions *********/






/********************** Binary Expressions ************************/
class BinaryExpression : public Expression{
	protected:
		Expression* left;
		Expression* right;

	public:
		BinaryExpression(Expression* _left, Expression* _right):left(_left),right(_right){}

		virtual const char *getOpcode() const =0;

		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst << "(";
			left->to_c(dst,indent);
			dst << getOpcode();
			right->to_c(dst,"");
			dst <<")";
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			dst << "(";
			left->to_python(dst,indent,tc);
			dst << getOpcode();
			right->to_python(dst,"",tc);
			dst <<")";
		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst<<"( ";
	        left->print_struct(dst,m);
	        dst<<" ";
	        dst<<getOpcode();
	        dst<<" ";
	        right->print_struct(dst,m);
	        dst<<" )";
		}
};


/********* Arithmetic Binary Expressions *********/

class MultExpression : public BinaryExpression{
	public:
		MultExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "*";
		}
};

class DivExpression : public BinaryExpression{
	public:
		DivExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "/";
		}
};

class AddExpression : public BinaryExpression{
	public:
		AddExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			auto destReg = ctx.getCurrStorage(); 	//write to dest Reg
			left->to_mips(dst,ctx);
			auto tempReg = ctx.assignNewStorage(); 
			right->to_mips(dst,ctx);
			std::string destReg_r;
			std::string tempReg_r;

			dst << "##### ADD ####" << std::endl;

			//read dest, read temp
			// write store to dest

			if(destReg.second == "reg"){
				destReg_r = destReg.first;
			}
			else{ //in memory, load it into some reg
				destReg_r = "v0";
				dst <<"lw $v0,"<<destReg.first<<"($fp)"<<std::endl;
			}

			if(tempReg.second == "reg"){
				tempReg_r = tempReg.first;
			}
			else{ //in memory, load it into some reg
				tempReg_r = "v1";
				dst <<"lw $v1,"<<tempReg.first<<"($fp)"<<std::endl;
			}



			//perform add
			dst <<"addu $"<<destReg_r<<",$"<<destReg_r<<",$"<<tempReg_r<<std::endl;

			//if dest regReg is mem, then save to stack

			if(destReg.second == "s"){
				dst << "sw $v0,"<<destReg.first<<"($fp)"<<std::endl;
			}

			ctx.deAllocStorage();
		}

		virtual const char *getOpcode() const override{
			return "+";
		}
};

class SubExpression : public BinaryExpression{
	public:
		SubExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			/*std::string destReg = ctx.get_dest_reg();
			left->to_mips(dst,ctx);
			std::string tempReg = ctx.alloc_free_reg();
			right->to_mips(dst,ctx);
			dst << "subu $"<<destReg<<",$"<<destReg<<",$"<<tempReg<<std::endl;*/
		}

		virtual const char *getOpcode() const override{
			return "-";
		}
};

/********* Relational Binary Expressions *********/

class LessThanExpression : public BinaryExpression{
	public:
		LessThanExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "<";
		}
};

class MoreThanExpression : public BinaryExpression{
	public:
		MoreThanExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return ">";
		}
};

class LessThanEqExpression : public BinaryExpression{
	public:
		LessThanEqExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "<=";
		}
};

class MoreThanEqExpression : public BinaryExpression{
	public:
		MoreThanEqExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return ">=";
		}
};

class EqualityExpression : public BinaryExpression{
	public:
		EqualityExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "==";
		}
};

class NotEqualityExpression : public BinaryExpression{
	public:
		NotEqualityExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "!=";
		}
};


/********* Bitwise Binary Expressions *********/




/********* Logical Binary Expressions *********/




/********************** Assignment Expressions ************************/

class AssignmentExpression : public Expression{
	protected:
		Expression* lvalue;
		Expression* expr;
			
	public:
		AssignmentExpression(Expression* _lvalue, Expression* _expr)
		:lvalue(_lvalue),expr(_expr){}
};



class DirectAssignmentExpression : public AssignmentExpression{

	public:
		DirectAssignmentExpression(Expression* _lvalue, Expression* _expr)
		: AssignmentExpression(_lvalue,_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			dst << "##### DirectAssignment #####" << std::endl;

			auto destReg = ctx.getCurrStorage(); 	//write to dest Reg
			lvalue->to_mips_getAddr(dst,ctx);		//addr store in destReg
			auto tempReg = ctx.assignNewStorage();
			expr->to_mips(dst,ctx);

			//lw r_exp, addr
			//need to get val from destReg
			//need to get val from tempReg


			std::string destReg_r = destReg.first;
			std::string tempReg_r = tempReg.first;

			if(destReg.second == "s"){
				destReg_r = "v0";
				dst<<"lw $v0,"<<destReg.first<<"($fp)"<<std::endl;
			}

			if(tempReg.second == "s"){
				tempReg_r = "v1";
				dst<<"lw $v1,"<<tempReg.first<<"($fp)"<<std::endl;
			}

			dst<<"sw $"<<tempReg_r<<",0($"<<destReg_r<<")"<<std::endl;

			ctx.deAllocStorage();

		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			lvalue->to_c(dst,indent);
			dst << " =";
			expr->to_c(dst," ");
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			lvalue->to_python(dst,indent,tc);
			dst << "=";
			expr->to_python(dst,"",tc);
		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "DirectAssignemntExpression" << std::endl;
		}
};


// TO IMPLMENET OTHER ASSIGNMENT I.E += ,-=, *= ETC



/********************** Ternary Expressions ************************/

// TO IMPLMENET CONDITIONAL EXPRESSION


#endif