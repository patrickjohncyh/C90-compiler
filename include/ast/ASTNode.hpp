#ifndef ASTNODE_HPP
#define ASTNODE_HPP



class ASTNode;


typedef const ASTNode* astNodePtr;

struct TranslateContext{
	std::vector<std::string> global_var;
};



class ASTNode{
	public:
		virtual ~ASTNode(){}

		virtual void print_struct(std::ostream &dst, int m) const =0;
		virtual void to_python(std::ostream &dst, std::string indent, TranslateContext &tc) const{
			std::cerr<<"ASTNode::translate is not implemented by type "<<typeid(this).name()<<"\n";
		}
		virtual void to_c(std::ostream &dst,std::string indent) const{
			std::cerr<<"ASTNode::C printing is not implemented by type "<<typeid(this).name()<<"\n";
		}
		virtual void to_mips(std::ostream &dst,Context& ctx) const{
			std::cerr<<"ASTNode::MIPS compilation is not implemented by type "<<typeid(this).name()<<"\n";
		}

};








#endif