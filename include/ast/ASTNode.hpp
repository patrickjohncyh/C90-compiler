#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <vector>


class ASTNode;


typedef const ASTNode* astNodePtr;

struct TranslateContext{
	std::vector<std::string> global_var;
};


class ASTNode{

	public:
		virtual ~ASTNode(){}

		virtual void print_struct(std::ostream &dst, int m) const =0;
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const =0;

};








#endif