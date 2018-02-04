#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP



#include <string>

class StringLiteral : public ASTNode{

private:
	std::string str;
public:
	StringLiteral(std::string _str)str(_str){}

	virtual void print(std::ostream &dst) const override{
		dst << str;
	}

};





#endif