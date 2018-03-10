#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <iomanip>
#include <vector>
#include <cmath>


class Expression : public ASTNode{
	public:
		virtual void to_mips_getAddr(std::ostream &dst, Context& ctx) const{
			std::cout << "ERROR : Unassignable" << std::endl;
			exit(1);
		}

		virtual std::string to_mips_getId() const{
			std::cout << "ERROR : Not and Identifier" << std::endl;
			exit(1);
			return "";
		}
		virtual double eval() const{
			return 0;
		}
		virtual Type exprType(Context& ctx) const{
		//	std::cout << "ERROR : exprType() Not implemented" << std::endl;
			return Type(Int);
		}
};

class CastExpression : public Expression{
	protected:
		Type* cast_type;
		Expression* expr;
	public:
		CastExpression(Type* _cast_type, Expression* _expr)
		:cast_type(_cast_type),expr(_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{

			Type sourceType = expr->exprType(ctx);
			Type destType = *cast_type;

			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			expr->to_mips(dst,ctx);

			ctx.convertMemRegType(sourceType,destType,destMemReg,dst);
		}


		virtual Type exprType(Context& ctx) const override{
			return *cast_type;
		}

		virtual void print_struct(std::ostream &dst, int m) const override{
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

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			expr->to_mips_getAddr(dst,ctx);								//addr of expression in destReg	
			std::string tempReg = "v1";
			ctx.memReg_read(destMemReg, destReg, dst);
			dst<<"lw    $"<<tempReg<<",0($"<<destReg<<")"<<std::endl;	//value of expr now in tempReg
			dst<<"addiu $"<<tempReg<<",$"<<tempReg<<",1"<<std::endl;	//increment
			dst<<"sw	$"<<tempReg<<",0($"<<destReg<<")"<<std::endl;	//save incremented value to mem
			dst<<"addiu	$"<<destReg<<",$"<<tempReg<<",-1"<<std::endl;	//place old value into destReg
			ctx.memReg_write(destMemReg, destReg, dst);
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			expr->to_c(dst,indent);
			dst<< "++";
		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			expr->print_struct(dst,m);
			dst << "++" << std::endl;
		}
};

class PostDecrementExpression : public UnaryExpression{
	public:
		PostDecrementExpression(Expression* _expr):UnaryExpression(_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			expr->to_mips_getAddr(dst,ctx);								//addr of expression in destReg	
			std::string tempReg = "v1";
			ctx.memReg_read(destMemReg, destReg, dst);
			dst<<"lw    $"<<tempReg<<",0($"<<destReg<<")"<<std::endl;	//value of expr now in tempReg
			dst<<"addiu $"<<tempReg<<",$"<<tempReg<<",-1"<<std::endl;	//decrement
			dst<<"sw	$"<<tempReg<<",0($"<<destReg<<")"<<std::endl;	//save incremented value to mem
			dst<<"addiu	$"<<destReg<<",$"<<tempReg<<",1"<<std::endl;	//place old value into destReg
			ctx.memReg_write(destMemReg, destReg, dst);
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			expr->to_c(dst,indent);
			dst<< "--";
		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			expr->print_struct(dst,m);
			dst << "--" << std::endl;
		}
};

class ArrayAccessExpression : public UnaryExpression{
	protected:
		Expression* op_expr;
	public:
		ArrayAccessExpression(Expression *_expr,Expression *_op_expr)
		:UnaryExpression(_expr),op_expr(_op_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";		
			to_mips_getAddr(dst,ctx);		//addr of expression operated on

			ctx.memReg_read(destMemReg,destReg,dst);
			ctx.memoryOffsetRead(exprType(ctx),destReg,destReg,0,dst);
			ctx.memReg_write(destMemReg,destReg,dst);			
		}

		virtual void to_mips_getAddr(std::ostream &dst, Context& ctx) const override{
			Type lType = expr->exprType(ctx);

			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			expr->to_mips(dst,ctx);			//addr of id from map if local or mem loc if arguemnt or some sort of pointer

			auto offsetMemReg = ctx.assignNewStorage();
			std::string offsetReg = "v1";	
			op_expr->to_mips(dst,ctx);		//offset calc
			ctx.deAllocStorage();	

			ctx.memReg_read(destMemReg,destReg,dst);
			ctx.memReg_read(offsetMemReg,offsetReg,dst);

			dst << "sll  $"<<offsetReg<<",$"<<offsetReg<<","<<log2(lType.bytes())<< std::endl;	//mult offset by 4 (for int)
			dst << "addu $"<<destReg  <<",$"<<destReg  <<",$"<<offsetReg 		 << std::endl;	//address of element
			ctx.memReg_write(destMemReg,destReg,dst);
		}

		virtual Type exprType(Context& ctx) const override{
			Type type=expr->exprType(ctx);
			type.dec_pLevel();
			return type;
		}

		virtual void print_struct(std::ostream &dst, int m) const override{
		}
};

class FunctionCallExpression : public UnaryExpression{
	private:
		std::vector<Expression*>* a_list;

	public:
		FunctionCallExpression(Expression *_expr , std::vector<Expression*>* _a_list = NULL)
		:UnaryExpression(_expr),a_list(_a_list){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{

			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";		//result of function call stored here

			int numArgs=0;
			if(a_list!=NULL) numArgs = a_list->size();

			dst << "addiu $sp,$sp," << -numArgs*4 + ctx.getCurrStorage() << std::endl;		//sp to correct position
			for(int i=0; i<numArgs; i++){
				auto tempMemReg = ctx.assignNewStorage(); 	
				std::string tempReg = "v1";
				(*a_list)[i]->to_mips(dst,ctx); //eval expression
				ctx.deAllocStorage();
				ctx.memReg_read(tempMemReg,tempReg,dst);
				if(i < 4){
					dst<< "move "<<"$a"<<i<<",$"<<tempReg<< std::endl;
				}
				else{
					dst<<"sw $"<<tempReg<<","<<4*i<<"($sp)"<<std::endl;	//NB : arg pos relative to new sp
				}
			}

			std::string id = expr->to_mips_getId();
			dst << "jal "<< id << std::endl; 											//call function, Assumes that it is an Identifier
			dst << "nop "<<std::endl;
			dst << "addiu $sp,$sp," << -ctx.getCurrStorage()  + numArgs*4 << std::endl;	//sp to original position
			dst << "move $"<<destReg<<",$2" << std::endl;
			ctx.memReg_write(destMemReg,destReg,dst);

		}

		virtual Type exprType(Context& ctx) const override{
			return Type(Int); //should be the type of the function declared...
		}

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


/********************** Pre Fix Expressions *********/
class ReferenceExpression : public UnaryExpression{
	public:
		ReferenceExpression(Expression* _expr):UnaryExpression(_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			expr->to_mips_getAddr(dst,ctx);		//addr of expression in destReg	
		}

		virtual Type exprType(Context& ctx) const override{
			Type type = expr->exprType(ctx);
			type.inc_pLevel();
			return type;
		}

		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst<< "&";
			expr->to_c(dst,indent);

		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "&" << std::endl;
			expr->print_struct(dst,m);
		}
};

class DereferenceExpression : public UnaryExpression{
	public:
		DereferenceExpression(Expression* _expr):UnaryExpression(_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{

			Type type = expr->exprType(ctx);
			type.dec_pLevel();
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			expr->to_mips(dst,ctx);
			ctx.memReg_read(destMemReg, destReg, dst);
			ctx.memoryOffsetRead(type,destReg,destReg,0,dst);
			ctx.memReg_write(destMemReg, destReg, dst);
		}

		virtual Type exprType(Context& ctx) const override{
			Type type = expr->exprType(ctx);
			type.dec_pLevel();
			return type;
		}

		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst<< "*";
			expr->to_c(dst,indent);

		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "*" << std::endl;
			expr->print_struct(dst,m);
		}
};

class PreIncrementExpression : public UnaryExpression{
	public:
		PreIncrementExpression(Expression* _expr):UnaryExpression(_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			expr->to_mips_getAddr(dst,ctx);		//addr of expression in destReg	
			std::string tempReg = "v1";
			ctx.memReg_read(destMemReg, destReg, dst);
			dst<<"lw    $"<<tempReg<<",0($"<<destReg<<")"<<std::endl;	//value of expr now in tempReg
			dst<<"addiu $"<<tempReg<<",$"<<tempReg<<",1"<<std::endl;	//increment
			dst<<"sw	$"<<tempReg<<",0($"<<destReg<<")"<<std::endl;	//save incremented value to mem
			dst<<"move	$"<<destReg<<",$"<<tempReg<<std::endl;			//place incremeneted value into destReg
			ctx.memReg_write(destMemReg, destReg, dst);
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst<< "++";
			expr->to_c(dst,indent);

		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "++" << std::endl;
			expr->print_struct(dst,m);

		}
};

class PreDecrementExpression : public UnaryExpression{
	public:
		PreDecrementExpression(Expression* _expr):UnaryExpression(_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			expr->to_mips_getAddr(dst,ctx);		//addr of expression in destReg	
			std::string tempReg = "v1";
			ctx.memReg_read(destMemReg, destReg, dst);
			dst<<"lw    $"<<tempReg<<",0($"<<destReg<<")"<<std::endl;	//value of expr now in tempReg
			dst<<"addiu $"<<tempReg<<",$"<<tempReg<<",-1"<<std::endl;	//increment
			dst<<"sw	$"<<tempReg<<",0($"<<destReg<<")"<<std::endl;	//save incremented value to mem
			dst<<"move	$"<<destReg<<",$"<<tempReg<<std::endl;			//place incremeneted value into destReg
			ctx.memReg_write(destMemReg, destReg, dst);
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst<< "--";
			expr->to_c(dst,indent);
		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "--" << std::endl;
			expr->print_struct(dst,m);
		}
};

class PrePositiveExpression : public UnaryExpression{
	public:
		PrePositiveExpression(Expression* _expr):UnaryExpression(_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			ctx.getCurrStorage();
			expr->to_mips(dst,ctx);	
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst<< "+";
			expr->to_c(dst,indent);
		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "+" << std::endl;
			expr->print_struct(dst,m);
		}
};

class PreNegativeExpression : public UnaryExpression{
	public:
		PreNegativeExpression(Expression* _expr):UnaryExpression(_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{

			Type rType = expr->exprType(ctx);
			Type type = ctx.arithmeticConversion(rType,rType);

			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			expr->to_mips(dst,ctx);		

			ctx.convertMemRegType(rType,type,destMemReg,dst);

			ctx.memReg_read(destMemReg, destReg, dst);
			if(type.isIntegral()){
				dst<<"subu  $"<<destReg<<",$0,$"<<destReg<<std::endl;		//negate
			}
			else{
				std::string left_f  = "f0";
				std::string zero_f  = "f2";
				ctx.moveToFloatReg(destReg  ,left_f , dst);
				ctx.moveToFloatReg("0"		,zero_f , dst);
				dst<<"sub.s   $"<<left_f<<",$"<<zero_f<<",$"<<left_f<<std::endl;
				ctx.moveFromFloatReg(destReg,left_f,dst);
			}
			ctx.memReg_write(destMemReg, destReg, dst);
		}
		virtual Type exprType(Context& ctx) const override{
			Type rType 	  = expr->exprType(ctx);
			Type thisType = ctx.arithmeticConversion(rType,rType);
			return thisType;
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst<< "-";
			expr->to_c(dst,indent);
		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "-" << std::endl;
			expr->print_struct(dst,m);
		}
};

class PreNotExpression : public UnaryExpression{
	public:
		PreNotExpression(Expression* _expr):UnaryExpression(_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			expr->to_mips(dst,ctx);	
			ctx.memReg_read(destMemReg, destReg, dst);
			dst << "sltiu $"<<destReg<<",$0,1"<<std::endl;
			ctx.memReg_write(destMemReg, destReg, dst);
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst<< "!";
			expr->to_c(dst,indent);
		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "!" << std::endl;
			expr->print_struct(dst,m);
		}
};


/********************** Binary Expressions ************************/
class BinaryExpression : public Expression{
	protected:
		Expression* left;
		Expression* right;

	public:
		BinaryExpression(Expression* _left, Expression* _right):left(_left),right(_right){}

		virtual const char *getOpcode() const =0;

		virtual void to_mips_getOperation(std::ostream &dst, Context& ctx,std::string left,std::string right, Type type) const{}; 

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{

			Type lType = left->exprType(ctx);
			Type rType = right->exprType(ctx);
			Type type  = ctx.arithmeticConversion(lType,rType);

			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			left->to_mips(dst,ctx);

			auto tempMemReg = ctx.assignNewStorage(); 
			std::string tempReg = "v1";
			right->to_mips(dst,ctx);
			ctx.deAllocStorage();

			ctx.convertMemRegType(lType,type,destMemReg,dst);
			ctx.convertMemRegType(rType,type,tempMemReg,dst);
			
			ctx.memReg_read(destMemReg,destReg,dst);	
			ctx.memReg_read(tempMemReg,tempReg,dst);	

			to_mips_getOperation(dst,ctx,destReg,tempReg,type);
			
			ctx.memReg_write(destMemReg, destReg,dst);	
		}

		virtual Type exprType(Context& ctx) const override{
			Type leftType  = left->exprType(ctx);
			Type rightType = right->exprType(ctx);
			Type thisType = ctx.arithmeticConversion(leftType,rightType);
			return thisType;
		}

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

		virtual void to_mips_getOperation(std::ostream &dst, Context& ctx,std::string left,std::string right, Type type) const override{
			if(type.isIntegral()){
				if(type.isSigned()){
					dst <<"mult $"<<left<<",$"<<right<<std::endl;
					dst <<"mflo $"<<left<<std::endl;
				}
				else{
					dst <<"multu $"<<left<<",$"<<right<<std::endl; //
					dst <<"mflo $"<<left<<std::endl;
				}
			}
			else{
				std::string left_f  = "f0";
				std::string right_f = "f2";
				ctx.moveToFloatReg(left ,left_f , dst);
				ctx.moveToFloatReg(right,right_f, dst);
				dst <<"mul.s $"<<left_f<<",$"<<left_f<<",$"<<right_f<<std::endl;
				ctx.moveFromFloatReg(left,left_f,dst);
			}
		}

		virtual const char *getOpcode() const override{
			return "*";
		}
};

class DivExpression : public BinaryExpression{
	public:
		DivExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual void to_mips_getOperation(std::ostream &dst, Context& ctx,std::string left,std::string right, Type type) const override{
			if(type.isIntegral()){
				if(type.isSigned()){
					dst <<"div $0,$"<<left<<",$"<<right<<std::endl;
				} 			
				else{
					dst <<"divu $0,$"<<left<<",$"<<right<<std::endl;
				}
				dst <<"mfhi $"<<left<<std::endl;
				dst <<"mflo $"<<left<<std::endl;
			}
			else{
				std::string left_f  = "f0";
				std::string right_f = "f2";
				ctx.moveToFloatReg(left ,left_f , dst);
				ctx.moveToFloatReg(right,right_f, dst);
				dst <<"div.s $"<<left_f<<",$"<<left_f<<",$"<<right_f<<std::endl;
				ctx.moveFromFloatReg(left,left_f,dst);
			}
		};
		virtual const char *getOpcode() const override{
			return "/";
		}
};

class ModuloExpression : public BinaryExpression{
	public:
		ModuloExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual void to_mips_getOperation(std::ostream &dst, Context& ctx,std::string left,std::string right, Type type) const override{
			if(type.isIntegral())
				if(type.isSigned()){
					dst <<"div  $"<<left<<",$"<<right<<std::endl;
					dst <<"mfhi $"<<left<<std::endl;	
				}
				else{
					dst <<"divu $"<<left<<",$"<<right<<std::endl;
					dst <<"mfhi $"<<left<<std::endl;
				}
			else{
				std::cout << "Error : Cannot perform % on non-integeral types"<<std::endl;
				exit(1);
			}
		};
		virtual const char *getOpcode() const override{
			return "%";
		}
};

class AddExpression : public BinaryExpression{
	public:
		AddExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual void to_mips_getOperation(std::ostream &dst, Context& ctx,std::string left,std::string right,Type type) const override{
			if(type.isIntegral() || type.isPointer())
				dst <<"addu $"<<left<<",$"<<left<<",$"<<right<<std::endl;
			else{
				std::string left_f  = "f0";
				std::string right_f = "f2";
				ctx.moveToFloatReg(left ,left_f , dst);
				ctx.moveToFloatReg(right,right_f, dst);
				dst<<"add.s $"<<left_f<<",$"<<left_f<<",$"<<right_f<<std::endl;
				ctx.moveFromFloatReg(left,left_f,dst);
			}
		}
		virtual double eval() const override{
			return left->eval() + right->eval();	
		}
		virtual const char *getOpcode() const override{
			return "+";
		}
};

class SubExpression : public BinaryExpression{
	public:
		SubExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual void to_mips_getOperation(std::ostream &dst, Context& ctx,std::string left,std::string right,Type type) const override{
			if(type.isIntegral())
				dst <<"subu $"<<left<<",$"<<left<<",$"<<right<<std::endl;
			else{
				std::string left_f  = "f0";
				std::string right_f = "f2";
				ctx.moveToFloatReg(left ,left_f , dst);
				ctx.moveToFloatReg(right,right_f, dst);
				dst<<"sub.s $"<<left_f<<",$"<<left_f  <<",$"<<right_f<<std::endl;
				dst<<"mfc1  $"<<left  <<",$"<<left_f  <<std::endl;
				ctx.moveFromFloatReg(left,left_f,dst);
			}
		};
		virtual const char *getOpcode() const override{
			return "-";
		}
};

/********* Relational Binary Expressions *********/

class LessThanExpression : public BinaryExpression{
	public:
		LessThanExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual void to_mips_getOperation(std::ostream &dst, Context& ctx,std::string left,std::string right, Type type) const override{
			dst <<"slt $"<<left<<",$"<<left<<",$"<<right<<std::endl;
		};

		virtual const char *getOpcode() const override{
			return "<";
		}
};

class MoreThanExpression : public BinaryExpression{
	public:
		MoreThanExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual void to_mips_getOperation(std::ostream &dst, Context& ctx,std::string left,std::string right, Type type) const override{
			if(type.isIntegral()){
				if(type.isSigned()){
					dst <<"slt $"<<left<<",$"<<right<<",$"<<left<<std::endl;
				}
				else{
					dst <<"sltu $"<<left<<",$"<<right<<",$"<<left<<std::endl;
				}
			}
			else{
				//
			}
			
		}

		virtual const char *getOpcode() const override{
			return ">";
		}
};

class LessThanEqExpression : public BinaryExpression{
	public:
		LessThanEqExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}
		
		virtual void to_mips_getOperation(std::ostream &dst, Context& ctx,std::string left,std::string right, Type type) const override{
			dst <<"slt  $"<<left<<",$"<<right<<",$"<<left<<std::endl;
			dst <<"xori	$"<<left<<",$"<<left<<","<<0x1<<std::endl;
		};

		virtual const char *getOpcode() const override{
			return "<=";
		}
};

class MoreThanEqExpression : public BinaryExpression{
	public:
		MoreThanEqExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual void to_mips_getOperation(std::ostream &dst, Context& ctx,std::string left,std::string right, Type type) const override{
			dst <<"slt  $"<<left<<",$"<<left<<",$"<<right<<std::endl;
			dst <<"xori	$"<<left<<",$"<<left<<","<<0x1<<std::endl;
		};

		virtual const char *getOpcode() const override{
			return ">=";
		}
};

class EqualityExpression : public BinaryExpression{
	public:
		EqualityExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual void to_mips_getOperation(std::ostream &dst, Context& ctx,std::string left,std::string right, Type type) const override{
			dst <<"xor   $"<<left<<",$"<<left<<",$"<<right<<std::endl;
			dst <<"sltiu $"<<left<<",$"<<left<<",1"<<std::endl;
		};
		virtual const char *getOpcode() const override{
			return "==";
		}
};

class NotEqualityExpression : public BinaryExpression{
	public:
		NotEqualityExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual void to_mips_getOperation(std::ostream &dst, Context& ctx,std::string left,std::string right, Type type)const override{
			dst <<"xor  $"<<left<<",$"<<left<<",$"<<right<<std::endl;
			dst <<"sltu $"<<left<<",$0,$"<<left<<std::endl;
		};

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
			dst << "# ----- DirectAssignment ----- #" << std::endl;

			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";	//addr
			lvalue->to_mips_getAddr(dst,ctx);			
			auto tempMemReg = ctx.assignNewStorage();
			std::string tempReg = "v1";	//val bit pattr
			expr->to_mips(dst,ctx);
			ctx.deAllocStorage();

			Type lType = lvalue->exprType(ctx);
			Type rType = expr->exprType(ctx);

			ctx.convertMemRegType(rType,lType,tempMemReg,dst); //from rType to lType;

			ctx.memReg_read(destMemReg, destReg, dst);
			ctx.memReg_read(tempMemReg, tempReg, dst);
			ctx.memoryOffsetWrite(lType,tempReg,destReg,0,dst);
			dst<<"move $"<<destReg<<",$"<<tempReg<<std::endl;
			ctx.memReg_write(destMemReg, destReg, dst);
		}

		virtual void to_c(std::ostream &dst,std::string indent) const override{
			lvalue->to_c(dst,indent);
			dst << "=";
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