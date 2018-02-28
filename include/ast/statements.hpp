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

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			ctx.assignNewStorage();
			expr->to_mips(dst,ctx);
		}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << std::setw(m) << "";
			dst << "ExpressionStatement [ ";
			expr->print_struct(dst,m);
			dst << " ]" << std::endl;
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			expr->to_c(dst,indent);
			dst << ";";
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			expr->to_python(dst,indent,tc);
			dst << std::endl;
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
			Context tmpCtx = Context(ctx);

			if(d_list != NULL){
				for(auto it=d_list->begin();it!=d_list->end();it++){
					(*it)->to_mips(dst,tmpCtx);
					tmpCtx.resetRegisters();
				}
			}
			if(s_list !=NULL){
				for(auto it=s_list->begin();it!=s_list->end();it++){
					(*it)->to_mips(dst,tmpCtx);
					tmpCtx.resetRegisters();
				}
			}
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

class WhileStatement : public Statement{
	private:
		Expression* cond_expr;
		Statement* s_true;
		
	public:
		WhileStatement(Expression* _cond_expr, Statement* _s_true)
		:cond_expr(_cond_expr),s_true(_s_true){}

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
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << std::setw(m) << "";
			dst << "WhileStatement [ " ;
			cond_expr->print_struct(dst,m+2);
			dst << std::endl;
			s_true->print_struct(dst,m+2);
			dst << std::setw(m) << "";
			dst << " ]" << std::endl;
		}

};

class ForStatement : public Statement{
	private:
		Expression* init_expr;
		Expression* cond_expr;
		Expression* update_expr;
		Statement*	s_true;
		
	public:
		ForStatement(Expression* _init_expr, Expression* _cond_expr, Expression* _update_expr, Statement* _s_true)
		:init_expr(_init_expr),cond_expr(_cond_expr),update_expr(_update_expr),s_true(_s_true){}

		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst << indent << "for(";
			init_expr->to_c(dst,"");
			dst << ";";
			cond_expr->to_c(dst,"");
			dst << ";";
			update_expr->to_c(dst,"");
			dst << ")" << std::endl;
			s_true->to_c(dst,indent);
		} 
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "ForStatement" << std::endl;
		}
};


class JumpStatement : public Statement{
	private:
		Expression* expr;

	public:
		JumpStatement( Expression* _expr = NULL)
		:expr(_expr){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			dst << "##### Return #####" << std::endl;
			auto destReg = ctx.getCurrStorage();
			std::string destReg_r = destReg.first;

			if(destReg.second == "s"){
				destReg_r = "v0";
			}	

			expr->to_mips(dst,ctx);

			dst <<"move $2,$"<<destReg_r<<std::endl;

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