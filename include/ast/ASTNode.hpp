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

typedef std::pair <std::string,std::string> var_pair;	// loc, type (global_init,global,local);
typedef	std::pair <std::string,std::string> store_pair; // loc, type (reg,stack)


const int global = 0;
const int local  = 1;


struct Context{
	std::map<std::string,var_pair> var_location;
	const std::string temp_reg[14] = {"s0","s1","s2","s3","s4","s5","t0","t1","t2","t3","t4","t5","t6","t7"};
	const std::string stack_reg[2] = {"s6","s7"};

	int curr_dest_reg = 12;
	int scope = global;
	int var_fp_offset_count = 0; // easiest if fp is just set to the old sp. arguments easily known. 
	int dmem_fp_offset_count = 0; // growing negatveily
	int dmem_reg_count = 0;


	void scopeLocal(){
		scope = local;
	}
	void scopeGlobal(){
		scope = global;
	}


	int getScope(){
		return scope;
	}

	void resetRegisters(){
		curr_dest_reg = 12; // shld be -1, but testing robustness of register allocation
	}

	store_pair assignNewStorage(){
		if(curr_dest_reg<13){	//use registers
			curr_dest_reg++;
			return std::make_pair(temp_reg[curr_dest_reg],"reg");
		}
		else{					//use stack
			curr_dest_reg++; // make it 14
			dmem_fp_offset_count-=4;
			dmem_reg_count++;
			return std::make_pair(std::to_string(dmem_fp_offset_count),"s");
		}
	}

	void deAllocStorage(){
		if(dmem_reg_count >0){
			dmem_fp_offset_count+=4; //?? not sure
			dmem_reg_count--;
		}
		else{
			curr_dest_reg--;
		}
	}

	store_pair getCurrStorage(){
		if(curr_dest_reg<=13){	//use registers
			return std::make_pair(temp_reg[curr_dest_reg],"reg");
		}
		else{					//use stack
			return std::make_pair(std::to_string(dmem_fp_offset_count),"s");
		}
	}

	void assignNewVariable(std::string name,std::string type = "int"){
		if(scope == global){
			
		}
		else if(scope == local){
			var_location[name] = std::make_pair(std::to_string(var_fp_offset_count),"local");
			var_fp_offset_count+=4;
		}
	}

	var_pair getVariable(std::string name){		//returns var type and address
		if(var_location.count(name)){
			return var_location[name];
		}
		else{
			std::cout << "Error : Variable ( "<<name<<" ) not declared " << std::endl;
			exit(1);
		}
	}

};


/*


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
			var_binding[var] = loc;
	}

	std::string get_binding(std::string var){
		if(var_binding[var] == ".globl"){
			std::stringstream ss;
			ss <<"    lui $"<<curr_dest_reg<<",%hi("<<var<<")"<<std::endl;
			ss <<"	  ori $"<<curr_dest_reg<<",%lo("<<var<<")"<<std::endl;
			return ss.str();
		}
		else if(var_binding[var] == ".comm"){
			std::stringstream ss;
			ss <<"    lui $28,%hi(__gnu_local_gp)"<<std::endl;
			ss <<"    addiu $28,$28,%lo(__gnu_local_gp)"<<std::endl;
			ss <<"    addiu $"<<curr_dest_reg<<",$28,%got("<<var<<")"<<std::endl;
			return ss.str();
		}
		else{
			std::stringstream ss;
			//ss <<"    addiu $"<<curr_dest_reg<<",$fp,"<<std::endl;
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
*/

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