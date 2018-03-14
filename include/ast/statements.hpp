#ifndef STATEMENTS_HPP
#define STATEMENTS_HPP

#include "expressions.hpp"
#include "declarations.hpp"
#include <iomanip>
#include <sstream>

class ExprStatement : public Statement{
	private:
		Expression *expr;

	public:
		ExprStatement( Expression* _expr = NULL):expr(_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			if(expr!=NULL){
				ctx.assignNewStorage();
				expr->to_mips(dst,ctx);
				ctx.deAllocStorage();
			}	
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			if(expr!=NULL){
				expr->to_c(dst,indent);
				dst << ";";
			}
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			if(expr!=NULL){
				expr->to_python(dst,indent,tc);
				dst << std::endl;
			}
		}
};


class CompoundStatement : public Statement{
	private:
		std::vector<Statement*>* s_list;
		std::vector<Declaration*>* d_list;

	public:
		CompoundStatement(std::vector<Declaration*>* _d_list = NULL ,std::vector<Statement*>* _s_list = NULL)
		:s_list(_s_list),d_list(_d_list){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			ctx.open_scope();

			if(d_list != NULL){
				for(auto it=d_list->begin();it!=d_list->end();it++){
					(*it)->to_mips(dst,ctx);
				}
			}
			if(s_list !=NULL){
				for(auto it=s_list->begin();it!=s_list->end();it++){
					(*it)->to_mips(dst,ctx);
				}
			}

			ctx.close_scope();
		}

		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst<<indent<<"{"<<std::endl;
			if(d_list != NULL){
				for(auto it=d_list->begin();it!=d_list->end();it++){
					(*it)->to_c(dst,indent+"    ");
					dst << ";" << std::endl;
				}
			}
			if(s_list !=NULL){			//print statement list
				for(auto it=s_list->begin();it!=s_list->end();it++){
					(*it)->to_c(dst,indent+"    ");
					dst << std::endl;
				}
			}
			dst<<indent<<"}"<<std::endl;
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			if(d_list != NULL){
				for(auto it=d_list->begin();it!=d_list->end();it++){
					(*it)->to_python(dst,indent,tc);
				}
			}
			if(s_list !=NULL){			//print statement list
				for(auto it=s_list->begin();it!=s_list->end();it++){
					(*it)->to_python(dst,indent,tc);
				}
			//	dst << std::endl;
			}
		}
	
};

class ConditionIfStatement : public Statement{
	private:
		Expression* cond_expr;
		Statement* s_true;
		
	public:
		ConditionIfStatement(Expression* _cond_expr, Statement* _s_true)
		:cond_expr(_cond_expr),s_true(_s_true){}


		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			auto condMemReg = ctx.assignNewStorage();
			cond_expr->to_mips(dst,ctx);
			ctx.deAllocStorage();

			std::string bottom_label = ctx.generateLabel("if_bottom");

			std::string condReg = "v0";
			ctx.memReg_read(condMemReg, condReg,dst);	

			dst << "beq $0,$"<<condReg<<","<<bottom_label<<std::endl;
			dst << "nop" << std::endl;
			
			s_true->to_mips(dst,ctx);
			dst << bottom_label << ":" << std::endl;
		}


		virtual void to_c(std::ostream &dst, std::string indent) const override{
			dst << indent << "if (";
			cond_expr->to_c(dst,"");
			dst << ")" << std::endl;
			s_true->to_c(dst,indent);
			dst << std::endl;
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			dst << indent << "if(";
			cond_expr->to_python(dst,"",tc);
			dst << "):" << std::endl;
			s_true->to_python(dst,indent+"  ",tc);
			dst << std::endl;
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

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			auto condMemReg = ctx.assignNewStorage();
			cond_expr->to_mips(dst,ctx);
			ctx.deAllocStorage();

			std::string if_bottom_label = ctx.generateLabel("if_bottom");
			std::string else_bottom_label = ctx.generateLabel("else_bottom");

			std::string condReg = "v0";
			ctx.memReg_read(condMemReg,condReg,dst);

			dst << "beq $0,$"<<condReg<<","<<if_bottom_label<<std::endl;
			dst << "nop" << std::endl;

			s_true->to_mips(dst,ctx);

			dst << "b "<<else_bottom_label<<std::endl;
			dst << "nop" << std::endl;
			dst << if_bottom_label << ":" << std::endl;

			s_false->to_mips(dst,ctx);

			dst << else_bottom_label << ":" << std::endl;
		}

		virtual void to_c(std::ostream &dst, std::string indent) const override{
			dst << indent << "if(";
			cond_expr->to_c(dst,"");
			dst << ")" << std::endl;
			s_true->to_c(dst,indent);
			dst << std::endl;
			dst << indent << "else" << std::endl;
			s_false->to_c(dst,indent);
			dst << std::endl;
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			dst << indent << "if(";
			cond_expr->to_python(dst,"",tc);
			dst << "):" << std::endl;
			s_true->to_python(dst,indent+"  ",tc);
			dst << indent << "else:" << std::endl;
			s_false->to_python(dst,indent+"  ",tc);
		}
		
};

class WhileStatement : public Statement{
	private:
		Expression* cond_expr;
		Statement* s_true;
		
	public:
		WhileStatement(Expression* _cond_expr, Statement* _s_true)
		:cond_expr(_cond_expr),s_true(_s_true){}


		virtual void to_mips(std::ostream &dst, Context& ctx) const override{

			std::string whileStartLabel = ctx.generateLabel("$WHILE_START");
			std::string whileEndLabel = ctx.generateLabel("$WHILE_END");

			ctx.break_label.push(whileEndLabel);


			dst<<whileStartLabel<<":"<<std::endl;

			auto condMemReg = ctx.assignNewStorage();
			cond_expr->to_mips(dst,ctx);
			ctx.deAllocStorage();

			std::string condReg = "v0";
			ctx.memReg_read(condMemReg,condReg,dst);

			dst << "beq $0,$"<<condReg<<","<<whileEndLabel<<std::endl;

			s_true->to_mips(dst,ctx);

			dst << "b "<<whileStartLabel<<std::endl;
			dst << "nop" << std::endl;
			dst<<whileEndLabel<<":"<<std::endl;

			ctx.break_label.pop();
		}

		virtual void to_c(std::ostream &dst, std::string indent) const override{
			dst << indent << "while (";
			cond_expr->to_c(dst,"");
			dst << ")" << std::endl;
			s_true->to_c(dst,indent);
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			dst << indent << "while (";
			cond_expr->to_python(dst,"",tc);
			dst << "):" << std::endl;
			s_true->to_python(dst,indent+"  ",tc);
			dst << std::endl;
		}
		

};

class ForStatement : public Statement{
	private:
		Statement* init_stat;
		Statement* cond_stat;
		Expression* update_expr;
		Statement*	s_true;
		
	public:
		ForStatement(Statement* _init_stat, Statement* _cond_stat, Expression* _update_expr, Statement* _s_true)
		:init_stat(_init_stat),cond_stat(_cond_stat),update_expr(_update_expr),s_true(_s_true){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{

			std::string forStartLabel = ctx.generateLabel("$FOR_START");
			std::string forEndLabel = ctx.generateLabel("$FOR_END");

			ctx.break_label.push(forEndLabel);
			
			init_stat->to_mips(dst,ctx);

			dst<<forStartLabel<<":"<<std::endl;

			auto condMemReg = ctx.assignNewStorage();
			std::string condReg = "v0";
			ctx.deAllocStorage();
			cond_stat->to_mips(dst,ctx);

			ctx.memReg_read(condMemReg,condReg,dst);
			dst<<"beq $0,$"<<condReg<<","<<forEndLabel<<std::endl;
			s_true->to_mips(dst,ctx);

			if(update_expr!=NULL){
				ctx.assignNewStorage();
				std::string updateReg = "v0";
				update_expr->to_mips(dst,ctx);
				ctx.deAllocStorage();
			}

			dst << "b "<<forStartLabel<<std::endl;
			dst << "nop" << std::endl;
			dst<<forEndLabel<<":"<<std::endl;

			ctx.break_label.pop();
		}


		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst << indent << "for(";
			init_stat->to_c(dst,"");
			dst << ";";
			cond_stat->to_c(dst,"");
			dst << ";";
			update_expr->to_c(dst,"");
			dst << ")" << std::endl;
			s_true->to_c(dst,indent);
		} 
		
};


class JumpStatement : public Statement{
	private:
		Expression* expr;

	public:
		JumpStatement( Expression* _expr = NULL)
		:expr(_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			if(expr != NULL){
				dst << "# ----- Return General  -----#" << std::endl;
				Type type = expr->exprType(ctx);
				auto destMemReg = ctx.assignNewStorage();
				expr->to_mips(dst,ctx);
				ctx.deAllocStorage();
				ctx.convertMemRegType(type,ctx.returnType, destMemReg, dst);

				if(ctx.returnType.isIntegral() || ctx.returnType.isPointer() ){
					dst << "# ----- Return Integral -----#" << std::endl;
					std::string destReg = "v0";
					ctx.memReg_read(destMemReg,destReg,dst);
					dst <<"move $2,$"<<destReg<<std::endl;
				}
				else{
					dst << "# ----- Return Float-----#" << std::endl;
					std::string destReg = "f0";
					ctx.memReg_read_f(destMemReg,destReg,dst);
				}
				
			/*
				if(expr->exprType(ctx).isIntegral() || expr->exprType(ctx).isPointer()){
					auto destMemReg = ctx.assignNewStorage();
					expr->to_mips(dst,ctx);
			
					std::string destReg = "v0";
					ctx.memReg_read(destMemReg,destReg,dst);
					dst <<"move $2,$"<<destReg<<std::endl;
				}
				else{	//floating
					auto destMemReg = ctx.assignNewStorage();
					expr->to_mips(dst,ctx);
					ctx.deAllocStorage();
					std::string destReg = "f0";
					ctx.memReg_read_f(destMemReg,destReg,dst);
					dst<<".set	macro"<<std::endl;
					dst<<"trunc.w.s $"<<destReg<<",$"<<destReg<<",$v0"<<std::endl;
					dst<<".set	nomacro"<<std::endl;
					dst<<"mfc1	$2,$"<<destReg<<std::endl;
				}*/
			}
			dst <<"b " << ctx.return_label << std::endl; 
			dst <<"nop"<< std::endl; 
		}

		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst << indent << "return";
			if(expr!=NULL) expr->to_c(dst," ");
			dst << ";";
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			dst << indent << "return";
			if(expr != NULL) expr->to_python(dst," ",tc);
			dst << std::endl;
		}

};


class JumpBreakStatement : public Statement{
	private:
		Expression* expr;

	public:
		JumpBreakStatement(){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{

			dst << "##### Break #####" << std::endl;

			if(!ctx.break_label.empty()){
				std::string breakLabel = ctx.break_label.top();
				dst << "b " << breakLabel << std::endl;
				dst << "nop" << std::endl;
			}
			else{
				dst << "Error : Using Break Outside Loop" << std::endl;
				exit(1);
			}
		}


};


class ConditionSwitchStatement : public Statement{
	private:
		Expression* expr;
		Statement* s_ptr;
	
	public:
	
	ConditionSwitchStatement( Expression* _expr, Statement* _s_ptr)
	:expr(_expr),s_ptr(_s_ptr){}


	virtual void to_mips(std::ostream &dst, Context& ctx) const override{

		dst << "##### Switch #####" << std::endl;
		std::stringstream ss; //temporarily store in ss;
		std::string switchStartLabel = ctx.generateLabel("$SWTICH_START");
		std::string switchEndLabel = ctx.generateLabel("$SWTICH_END");
		ctx.break_label.push(switchEndLabel);

		case_pair start = std::make_pair(static_cast<Expression*>(NULL),""); //starting point
		ctx.switch_case_data.push(start);
		ctx.switch_case_default.push("start");
		s_ptr->to_mips(ss,ctx);

		//evalute expr
		auto switchMemReg = ctx.assignNewStorage();
		std::string switchReg = "v0";
		expr->to_mips(dst,ctx);
		
		auto caseMemReg = ctx.assignNewStorage();
		std::string caseReg = "v1";

		while( ctx.switch_case_data.top().first != NULL){  //EMIT CODE TO DO JUMP
			case_pair cp = ctx.switch_case_data.top();
			cp.first->to_mips(dst,ctx);

			ctx.memReg_read(switchMemReg,switchReg,dst);
			ctx.memReg_read(caseMemReg,caseReg,dst);

			dst << "beq $"<<switchReg<<",$"<<caseReg<<","<<cp.second<<std::endl;
			dst << "nop" << std::endl;
			ctx.switch_case_data.pop();
		}

		if( ctx.switch_case_default.top() != "start" ){  //check for default
			std::string defaultLabel = ctx.switch_case_default.top();
			dst << "b " << defaultLabel << std::endl;
			dst << "nop" << std::endl;
			ctx.switch_case_default.pop();
		}

		dst << "b " << switchEndLabel << std::endl;
		dst << "nop" << std::endl;

		dst << ss.str() << std::endl;
		dst << switchEndLabel << ":" <<std::endl;

		ctx.deAllocStorage();
		ctx.deAllocStorage();
		ctx.switch_case_data.pop();
		ctx.break_label.pop();

	}

};

class LabeledCaseStatement : public Statement{
	private:
		Expression* expr;
		Statement* s_ptr;

	public:
		LabeledCaseStatement( Expression* _expr, Statement* _s_ptr)
		:expr(_expr),s_ptr(_s_ptr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			std::string caseLabel = ctx.generateLabel("$CASE");
			case_pair cp = std::make_pair(expr,caseLabel);
			ctx.switch_case_data.push(cp);

			dst << cp.second << ":" << std::endl;

			s_ptr->to_mips(dst,ctx);

		}

};

class LabeledDefaultStatement : public Statement{
	private:
		Statement* s_ptr;

	public:
		LabeledDefaultStatement( Statement* _s_ptr )
		:s_ptr(_s_ptr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			std::string defaultLabel = ctx.generateLabel("$DEFAULT");
			ctx.switch_case_default.push(defaultLabel);

			dst << defaultLabel << ":" << std::endl;

			s_ptr->to_mips(dst,ctx);
		}

};

















#endif