#ifndef ASTNODE_HPP
#define ASTNODE_HPP


class ASTNode;


typedef const ASTNode* astNodePtr;


class ASTNode{

	public:
		virtual ~ASTNode(){}

	virtual void print(std::ostream &dst) const =0;

};



#endif