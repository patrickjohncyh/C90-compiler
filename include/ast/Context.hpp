#ifndef CONTEXT_HPP
#define CONTEXT_HPP


#include <vector>
#include <map>
#include <cassert>
#include <sstream>

typedef std::pair <std::string,std::string> var_pair;	// loc, type (global_init,global,local);

typedef int memReg;


const int global = 0;
const int local  = 1;

extern int labelCount;


struct Context{
	std::map<std::string,var_pair> var_location;

	int scope = global;
	int mem_fp_offset_count = 0; // growing negatveily
	int mem_reg_count = 0;


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
		var_location[name] = std::make_pair(std::to_string(loc),"local");
	}


	void assignNewVariable(std::string name,std::string type = "int"){
		if(scope == global){
			
		}
		else if(scope == local){
			mem_fp_offset_count-=4;
			var_location[name] = std::make_pair(std::to_string(mem_fp_offset_count),"local");

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



#endif