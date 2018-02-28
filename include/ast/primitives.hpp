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
		int val;
	public:
		Constant(int _val):val(_val){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			auto destReg = ctx.getCurrStorage();
			std::string destReg_r = "v0";
	
			dst << "##### Constant #####" << std::endl;
			
			dst <<"addiu $"<<destReg_r<<",$0,"<<val<<std::endl;

			dst << "sw $v0,"<<destReg<<"($fp)"<<std::endl;
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

class Identifier : public Primitive{
	private:
		std::string id;
	public:
		Identifier(std::string _id):id(_id){}

		virtual void to_mips(std::ostream &dst, Context& ctx) const override{
			dst << "##### accessVar #####" << std::endl;
			auto destReg = ctx.getCurrStorage();
			std::string destReg_r = "v0";

			to_mips_getAddr(dst,ctx);
			
			dst << "lw $"<<destReg_r<<",0($"<<destReg_r<<")"<<std::endl;

			
			dst << "sw $v0,"<<destReg<<"($fp)"<<std::endl;

		}



		virtual void to_mips_getAddr(std::ostream &dst, Context ctx) const{
			dst << "##### getAddr #####" << std::endl;

			auto destReg = ctx.getCurrStorage();
			auto var_loc = ctx.getVariable(id);
			std::string destReg_r = "v0";


			if(var_loc.second == "local"){
					dst<<"addiu $"<<destReg_r<<",$fp,"<<var_loc.first<<std::endl;
			}
			else{

			}


			dst << "sw $v0,"<<destReg<<"($fp)"<<std::endl;

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