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

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			std::string stringConstLabel = ctx.generateLabel("$SC");
			ctx.labeled_constant[stringConstLabel] = str;
			dst<<"la   $"<<destReg<<","<<stringConstLabel<<std::endl;	 //load address of string label
			ctx.memReg_write(destMemReg, destReg,dst);						 
		}

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


class IntegralConstant : public Primitive{
	private:
		std::string str_val;
		unsigned int val;
	public:
		IntegralConstant(std::string _str_val)
		:str_val(_str_val),val(std::stoull(str_val)){}

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
		virtual double eval() const{
			return val;
		}
};

class FloatingConstant : public Primitive{
	private:
		std::string str_val;
		double val;
	public:
		FloatingConstant(std::string _str_val)
		:str_val(_str_val),val(std::stod(str_val)){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			std::string addrReg = "v1";
			std::string floatConstLabel = ctx.generateLabel("$FC");
			ctx.labeled_constant[floatConstLabel] = str_val;
			dst<<"la   $"<<addrReg<<","<< floatConstLabel << std::endl;	 //address of float label
			ctx.memoryOffsetRead(exprType(ctx),destReg,addrReg,0,dst);	 //read from address into v0
			ctx.memReg_write(destMemReg, destReg,dst);					 //write from f0 into mem...

		}

		virtual Type exprType(Context& ctx) const override{
			return Type(Float); //shld be double as per c90 spec;
		}

		virtual double eval() const{
			return val;
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
};

class CharacterConstant : public Primitive{
	private:
		std::string str_val;
		unsigned int val;
		bool wide;
	public:
		CharacterConstant(std::string _str_val):str_val(_str_val){
			val = 0;
			if(str_val.substr(0,1).find("L") == -1){ //normal
				str_val = str_val.substr(1,str_val.length()-2);
				for(int i=0; i <str_val.length();i++){
					val = (val<<8) + (unsigned int)str_val.c_str()[i];
				}
			}
			else{
				str_val = str_val.substr(str_val.length()-2,1);
				val = (unsigned int)str_val.c_str()[0];
				wide = true;
			}
		}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			dst << "##### Character Constant #####" << std::endl;
			auto destMemReg = ctx.getCurrStorage();
			std::string destReg = "v0";
			dst <<"li $"<<destReg<<","<<val<<std::endl;
			ctx.memReg_write(destMemReg, destReg,dst);	
		}

		virtual Type exprType(Context& ctx) const override{
			if(wide)
				return Type(Char);
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
		virtual double eval() const{
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
				ctx.memoryOffsetRead(exprType(ctx),destReg,destReg,0,dst);
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
			return var.getType();
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