#ifndef CONTEXT_HPP
#define CONTEXT_HPP


#include <vector>
#include <map>
#include <stack>
#include <cassert>
#include <sstream>



class Expression; //forward declaration

typedef std::tuple <int,std::string,std::string,std::string> var_pair;	// loc, scope (global,local), primitive_type, derived_type ;
typedef std::pair <Expression*,std::string> case_pair;

typedef int memReg;

const int global = 0;
const int local  = 1;

extern int labelCount;


typedef std::map<std::string,var_pair> mapType;
typedef mapType* mapPtr;




struct Context{
	mapPtr var_location = new mapType();

	std::map<std::string,int> type_size = {{"int", 4}, {"char", 1}}; //in terms of bytes


	std::stack<std::map<std::string,var_pair>*> var_scope_stack;
	std::stack<int> var_scope_fp_count_stack;

	std::stack<case_pair> switch_case_data;
	std::stack<std::string> switch_case_default;
	std::stack<std::string> break_label;
	std::string return_label;



	int scope = global;
	int mem_fp_offset_count = 0; // growing negatveily
	int mem_reg_count = 0;


	void open_scope(){
		var_scope_stack.push(var_location);
		var_scope_fp_count_stack.push(mem_fp_offset_count);
		var_location = new std::map<std::string,var_pair>(*var_location);
	}

	void close_scope(){
		delete var_location;
		mem_fp_offset_count = var_scope_fp_count_stack.top();
		var_location = var_scope_stack.top();

		var_scope_fp_count_stack.pop();
		var_scope_stack.pop();
	}


	void memReg_read(memReg loc, std::string reg, std::ostream& dst){
		dst << "lw $"<<reg<<","<<loc<<"($fp)"<<std::endl;
	}

	void memReg_write(memReg loc, std::string reg, std::ostream& dst){
		dst << "sw $"<<reg<<","<<loc<<"($fp)"<<std::endl;
	}



	std::string generateLabel(std::string label){
		return label + "_" + std::to_string(labelCount++);
	}

	void scopeLocal(){
		scope = local;
	}
	void scopeGlobal(){
		scope = global;
	}

	int getScope(){
		return scope;
	}

	memReg assignNewStorage(){
		mem_fp_offset_count-=4;
		mem_reg_count++;
		return mem_fp_offset_count;
	}

	void deAllocStorage(){
		if(mem_reg_count >0){
			mem_fp_offset_count+=4;
			mem_reg_count--;
		}
	}

	memReg getCurrStorage(){
		return (mem_fp_offset_count);
	}

	void assignNewArgument(std::string name,int loc,std::string type = "int",std::string d_type = "basic"){
		(*var_location)[name] = std::make_tuple(loc,"local","int","basic");
	}

	void assignNewVariable(std::string name,std::string type = "int",std::string d_type = "basic", int size = 1){
		if(scope == global){
			(*var_location)[name] = std::make_tuple((mem_fp_offset_count),"global",type,d_type);
		}
		else if(scope == local){
			int num_bytes = type_size[type];
			mem_fp_offset_count-=num_bytes*size; // mainly for array. first element is nearer top of stack
			(*var_location)[name] = std::make_tuple(mem_fp_offset_count,"local",type,d_type); 
		}
	}

	var_pair getVariable(std::string name){		//returns var type and address
		if((*var_location).count(name)){
			return (*var_location)[name];
		}
		else{
			std::cout << "Error : Variable ( "<<name<<" ) not declared " << std::endl;
			exit(1);
		}
	}


	int getVariable_loc(std::string name){
		auto var = getVariable(name);
		return std::get<0>(var);

	}

	std::string getVariable_scope(std::string name){
		auto var = getVariable(name);
		return std::get<1>(var);
	}

	std::string getVariable_type(std::string name){
		auto var = getVariable(name);
		return std::get<2>(var);
	}

	std::string getVariable_dtype(std::string name){
		auto var = getVariable(name);
		return std::get<3>(var);
	}

};



#endif