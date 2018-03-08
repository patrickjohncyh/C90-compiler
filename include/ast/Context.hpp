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

extern int labelCount;


typedef std::map<std::string,Variable*> mapType;
typedef mapType* mapPtr;





struct Context{
	mapPtr var_location = new mapType();

	std::stack <mapType*> var_scope_stack;
	std::stack<int> var_scope_fp_count_stack;

	std::stack<case_pair> switch_case_data;
	std::stack<std::string> switch_case_default;
	std::stack<std::string> break_label;
	std::string return_label;

	std::map<std::string,std::string> labeled_constant;


	Scope scope = global;
	int mem_fp_offset_count = 0; // growing negatveily
	int mem_reg_count = 0;

	void open_scope(){
		var_scope_stack.push(var_location);
		var_scope_fp_count_stack.push(mem_fp_offset_count);
		var_location = new mapType(*var_location);
	}

	void close_scope(){
		delete var_location;
		mem_fp_offset_count = var_scope_fp_count_stack.top();
		var_location = var_scope_stack.top();

		var_scope_fp_count_stack.pop();
		var_scope_stack.pop();
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

	std::string generateLabel(std::string label){
		return label + "_" + std::to_string(labelCount++);
	}

	void memReg_read(memReg loc, std::string reg, std::ostream& dst){
		dst << "lw $"<<reg<<","<<loc<<"($fp)"<<std::endl;
	}

	void memReg_write(memReg loc, std::string reg, std::ostream& dst){
		dst << "sw $"<<reg<<","<<loc<<"($fp)"<<std::endl;
	}

	void memReg_read_f(memReg loc, std::string reg, std::ostream& dst){
		dst << "lwc1 $"<<reg<<","<<loc<<"($fp)"<<std::endl;
	}

	void memReg_write_f(memReg loc, std::string reg, std::ostream& dst){
		dst << "swc1 $"<<reg<<","<<loc<<"($fp)"<<std::endl;
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

	void assignNewArgument(std::string name, Type type, dType dtype, int addr){
		(*var_location)[name] = new Variable(local,type,dtype,addr);
	}

	void assignNewVariable(std::string name,Type type, dType dtype, int size = 1){
		if(scope == local){
			mem_fp_offset_count-= size *  type.bytes();
			mem_fp_offset_count-= (-mem_fp_offset_count)%4;	//make aligned
		}
		(*var_location)[name] = new Variable(scope,type,dtype,mem_fp_offset_count);
	}
	Variable getVariable(std::string name){		//returns Variable Object
		if((*var_location).count(name)){
			return * ( (*var_location)[name] );
		}
		std::cout << "Error : Variable ( "<<name<<" ) not declared " << std::endl;
		exit(1);
	}



	std::string memoryOffsetRead(Type type, std::string r1, std::string r2, int offset){
		std::stringstream ss;
		if( type.is(Char) ) 				ss<<"lb   $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else if(type.is(UChar) ) 			ss<<"lbu  $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else if(type.is(Short) ) 			ss<<"lh   $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else if(type.is(UShort))			ss<<"lhu  $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else if(type.isIntegral()) 			ss<<"lw   $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else 								ss<<"lwc1 $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		return ss.str();
	}

	std::string memoryOffsetWrite(Type type, std::string r1, std::string r2, int offset){
		std::stringstream ss;
		if( type.is(Char)||type.is(UChar) ) 		ss<<"sb   $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else if(type.is(Short) || type.is(UShort)) 	ss<<"sh   $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else if(type.isIntegral()) 					ss<<"sw   $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else 										ss<<"swc1 $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		return ss.str();
	}
	Type integeralPromotion(Type t){
		if(t.is(Char) || t.is(UChar) || t.is(Short) ||  t.is(UShort)){
			return Type(Int);
		}
		else
			return t;
	}
	Type arithmeticConversion(Type t1, Type t2){
		if(t1.is(LongDouble) || t1.is(LongDouble)){
			return Type(LongDouble);
		}
		else if(t1.is(Double) || t2.is(Double)){
			return Type(Double);
		}
		else if(t1.is(Float) || t2.is(Float)){
			return Type(Float);
		}
		else{
			t1 = integeralPromotion(t1);	//perform integral promotion
			t2 = integeralPromotion(t2);	//perform integral promotion

			if(t1.is(ULong)|| t2.is(ULong)){
				return Type(ULong);
			}
			else if( (t1.is(Long) && t2.is(UInt) ) || ( t1.is(UInt) && t2.is(Long)) ){
				//long int cannot represent all unsigned int in this context
				return Type(ULong);
			}
			else if(t1.is(Long) || t2.is(Long)){	//not much to test here?
				return Type(Long);
			}
			else if(t1.is(UInt) || t2.is(UInt)){ //doesnt really affect add sub moe liek mul div
				return Type(UInt);
			}
			else{
				return Type(Int);
			}
		}
	}
};



#endif