#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP



#include <string>

class Primitive : public Expression{
	virtual void print(std::ostream &dst) const=0;

};

class StringLiteral : public Primitive{
	private:
		std::string str;
	public:
		StringLiteral(std::string _str):str(_str){}

		virtual void print(std::ostream &dst) const override{
			dst << str;
		}
};

class Constant : public Primitive{
	private:
		int val;

	public:
		Constant(int _val):val(_val){}

		virtual void print(std::ostream &dst) const override{
			dst << val;
		}
};

class Identifier : public Primitive{
	private:
		std::string id;

	public:
		Identifier(std::string _id):id(_id){}

		virtual void print(std::ostream &dst) const override{
			dst << id;
		}
};



#endif