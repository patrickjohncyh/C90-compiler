#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <iomanip>
#include <string>

class Primitive : public Expression{
	virtual void print_struct(std::ostream &dst, int m) const=0;
};

class StringLiteral : public Primitive{
	private:
		std::string str;
	public:
		StringLiteral(std::string _str):str(_str){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << str;
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst << indent << str;
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			dst << indent << str;
		}
};

class Constant : public Primitive{
	private:
		unsigned val;
	public:
		Constant(int _val):val(_val){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			dst << "##### Constant #####" << std::endl;
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			dst <<"li $"<<destReg<<","<<val<<std::endl;
			ctx.memReg_write(destMemReg, destReg,dst);	
		}

		virtual Type exprType(Context& ctx) const override{
			if(val > 2147483647)
				return Type(UInt);
			return Type(Int);
		}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << val;
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst << indent << val;
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			dst << indent << val;
		}
		virtual int to_mips_eval() const{
			return val;
		}
};

class Identifier : public Primitive{
	private:
		std::string id;
	public:
		Identifier(std::string _id):id(_id){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			dst << "##### accessVar #####" << std::endl;
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			to_mips_getAddr(dst,ctx);

			Variable var = ctx.getVariable(id);

			if(!var.isArray()){	//array identifier evaluates to its address, pointer too possibly
				ctx.memReg_read(destMemReg, destReg,dst);
				dst<<ctx.memoryOffsetRead(exprType(ctx),destReg,destReg,0);
				ctx.memReg_write(destMemReg, destReg,dst);
			}
		}	

		virtual void to_mips_getAddr(std::ostream &dst, Context& ctx) const override{
			dst << "##### getAddr #####" << std::endl;
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			
			Variable var = ctx.getVariable(id);
			if(var.getScope() == local)
				dst<<"addiu $"<<destReg<<",$fp,"<<var.getAddr()<<std::endl;
			else
				dst<<"la $"<<destReg<<","<<id << std::endl;

			ctx.memReg_write(destMemReg, destReg,dst);
		}


		virtual std::string to_mips_getId() const{
			return id;
		}
		virtual Type exprType(Context& ctx) const override{
			Variable var = ctx.getVariable(id);
			return var.getType();;
		}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << id;
		}
		virtual void to_c(std::ostream &dst,std::string indent) const override{
			dst << indent << id;
		}
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const override{
			dst << indent << id;
		}


};

#endif