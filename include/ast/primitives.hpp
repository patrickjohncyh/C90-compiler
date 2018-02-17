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
		virtual void to_python(std::ostream &dst, std::string indent) const override{
			dst << indent << str;
		}
};

class Constant : public Primitive{
	private:
		int val;
	public:
		Constant(int _val):val(_val){}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << val;
		}
		virtual void to_python(std::ostream &dst, std::string indent) const override{
			dst << indent << val;
		}
};

class Identifier : public Primitive{
	private:
		std::string id;
	public:
		Identifier(std::string _id):id(_id){}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << id;
		}
		virtual void to_python(std::ostream &dst, std::string indent) const override{
			dst << indent << id;
		}
};

#endif