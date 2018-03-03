#ifndef CONTEXT_HPP
#define CONTEXT_HPP


#include <vector>
#include <map>
#include <stack>
#include <cassert>
#include <sstream>



class Expression; //forward declaration

typedef std::pair <std::string,std::string> var_pair;	// loc, type (global_init,global,local);
typedef std::pair <Expression*,std::string> case_pair;

typedef int memReg;


const int global = 0;
const int local  = 1;

extern int labelCount;


typedef std::map<std::string,var_pair> mapType;
typedef mapType* mapPtr;




struct Context{


	mapPtr var_location = new mapType();

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

	void assignNewArgument(std::string name,int loc){
		(*var_location)[name] = std::make_pair(std::to_string(loc),"local");
	}

	void assignNewVariable(std::string name,std::string type = "int"){
		if(scope == global){
			(*var_location)[name] = std::make_pair(std::to_string(mem_fp_offset_count),"global");
		}
		else if(scope == local){
			mem_fp_offset_count-=4;
			(*var_location)[name] = std::make_pair(std::to_string(mem_fp_offset_count),"local");
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

};



#endif