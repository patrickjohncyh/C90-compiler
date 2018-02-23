#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <vector>


class ASTNode;


typedef const ASTNode* astNodePtr;

struct TranslateContext{
	std::vector<std::string> global_var;
};


struct Context{
	std::vector< std::pair<std::string,std::string> > var_bindings_stack; //global,local : <var_name,memloc>, 
																		  //most recent represents the one visible 
																		  //to the scope
	int temp_reg_free = 0; 	//t0-t9
	int saved_reg_free = 0;	//s0-s7
	std::string curr_dest_reg = "t0";




	void reset_free_reg(){
		temp_reg_free = 0;
		saved_reg_free = 0;
		curr_dest_reg = "t0";
	}

	std::string get_dest_reg(){
		return curr_dest_reg;
	}

	std::string alloc_free_reg(){
		if(temp_reg_free <= 9){
			temp_reg_free++;
			curr_dest_reg = "t"+std::to_string(temp_reg_free);
			return curr_dest_reg;
		}
		else if(saved_reg_free <= 7){
			curr_dest_reg = "s"+std::to_string(saved_reg_free);
			saved_reg_free++;
			return curr_dest_reg;
		}
		else{
			std::cout << "Out of registers" << std::endl;
			return "NULL";
		}
	}
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