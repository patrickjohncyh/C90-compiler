#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <vector>
#include <map>
#include <cassert>
#include <sstream>


class ASTNode;


typedef const ASTNode* astNodePtr;

struct TranslateContext{
	std::vector<std::string> global_var;
};


struct Context{
	std::map<std::string,std::string> var_binding;

	int temp_reg_free = 0; 	//t0-t9
	int saved_reg_free = 0;	//s0-s7
	std::string curr_dest_reg = "t0";
	bool global_scope = true;
	int fp_max_offset = 0;
	int fp_curr_offset = -8 ;


	std::string alloc_stack(int n){
		fp_max_offset = fp_max_offset - n;
		return "addiu $sp,$sp,-" + std::to_string(n);
	}

	std::string get_free_stack(){
		fp_curr_offset = fp_curr_offset-4;
		assert(fp_curr_offset>=fp_max_offset && "Not enough stack allocated!");
		return std::to_string(fp_curr_offset)+"($fp)";
	}

	std::string get_curr_offset(){
		return std::to_string(fp_curr_offset)+"($fp)";
	}


	void set_binding(std::string var, std::string loc){
		if(global_scope){
			var_binding[var] = loc;
		}
		else{
			var_binding[var]= (loc+"($fp)");
		}
	}
	std::string get_binding(std::string var){
		if(var_binding[var] == ".globl"){
			std::stringstream ss;
			ss <<"    lui $"<<curr_dest_reg<<",%hi("<<var<<")"<<std::endl;
			ss <<"    lw  $"<<curr_dest_reg<<",%lo("<<var<<")($"<<curr_dest_reg<<")"<<std::endl; 
			return ss.str();
		}
		else if(var_binding[var] == ".comm"){
			std::stringstream ss;
			ss << "    lui $28,%hi(__gnu_local_gp)"<<std::endl;
			ss << "    addiu $28,$28,%lo(__gnu_local_gp)"<<std::endl;
			ss << "    lw $"<<curr_dest_reg<<",%got("<<var<<")($28)";
			return ss.str();
		}
		else{
			return var_binding[var];
		}
	}

	bool in_global_scope(){
		return global_scope;
	}

	void in_global(){
		global_scope = true;
	}

	void in_local(){
		global_scope = false;
	}

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