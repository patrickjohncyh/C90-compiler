#ifndef CONTEXT_HPP
#define CONTEXT_HPP


#include <vector>
#include <map>
#include <stack>
#include <cassert>
#include <sstream>
#include <cmath>
#include <cstring>


struct TranslateContext{
	std::vector<std::string> global_var;
};

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
	std::stack<std::string> cont_label;
	std::string return_label;
	Type returnType;

	std::map<std::string,std::string> goto_label;
	std::map<std::string,std::pair<std::string,std::string> > labeled_constant;


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
		dst<<"lw $"<<reg<<","<<loc<<"($fp)"<<std::endl;
	}

	void memReg_write(memReg loc, std::string reg, std::ostream& dst){
		dst<<"sw $"<<reg<<","<<loc<<"($fp)"<<std::endl;
	}

	void memReg_read_f(memReg loc, std::string reg, std::ostream& dst){
		dst<<"lwc1 $"<<reg<<","<<loc<<"($fp)"<<std::endl;
	}

	void memReg_write_f(memReg loc, std::string reg, std::ostream& dst){
		dst<<"swc1 $"<<reg<<","<<loc<<"($fp)"<<std::endl;
	}

	void moveToFloatReg(std::string iReg, std::string fReg, std::ostream& dst){
		dst<<"mtc1 $"<<iReg<<",$"<<fReg<<std::endl;
	}

	void moveFromFloatReg(std::string iReg,std::string fReg, std::ostream& dst){
		dst<<"mfc1 $"<<iReg<<",$"<<fReg<<std::endl;
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

	Variable assignNewVariable(std::string name,Type type, dType dtype, int size = 1){
		if(scope == local){
			//std::cout << name << std::endl;
			mem_fp_offset_count-= size *  type.bytes();
			//std::cout << mem_fp_offset_count << std::endl;
			mem_fp_offset_count-= (4-(-mem_fp_offset_count)%4);	//make aligned
			//std::cout << mem_fp_offset_count << std::endl;
		}
		(*var_location)[name] = new Variable(scope,type,dtype,mem_fp_offset_count);
		return *( (*var_location)[name] );
	}


	bool isFunctionDeclared(std::string name){					//returns Variable Object
		if((*var_location).count(name)){
			//lets just assume that it is a correct dtype...
			return 1;
		}
		else{
			return 0;	// function has not been declared...
		}
	}

	Variable getVariable(std::string name){					//returns Variable Object
		if((*var_location).count(name)){
			return * ( (*var_location)[name] );
		}
		std::cout << "Error : Variable ( "<<name<<" ) not declared " << std::endl;
		exit(1);
	}

	void  memoryOffsetRead(Type type, std::string r1, std::string r2, int offset, std::ostream& dst){
		if(type.isPointer())				dst<<"lw  $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;

		else if( type.is(Char) ) 			dst<<"lb  $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else if(type.is(UChar) ) 			dst<<"lbu $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else if(type.is(Short) ) 			dst<<"lh  $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else if(type.is(UShort))			dst<<"lhu $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else if(type.isIntegral()) 			dst<<"lw  $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else 								dst<<"lw  $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
	}

	void memoryOffsetWrite(Type type, std::string r1, std::string r2, int offset, std::ostream& dst){
		if(type.isPointer())						dst<<"sw  $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;

		else if( type.is(Char)||type.is(UChar) ) 	dst<<"sb   $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else if(type.is(Short) || type.is(UShort)) 	dst<<"sh   $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else if(type.isIntegral()) 					dst<<"sw   $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
		else 										dst<<"sw   $"<<r1<<","<<offset<<"($"<<r2<<")"<<std::endl;
	}

	Type integralPromotion(Type t){
		if(t.isIntegral()){
			if(t.is(Char) || t.is(UChar) || t.is(Short) ||  t.is(UShort)){
				return Type(Int);
			}
		}
		return t;
	}

	Type arithmeticConversion(Type t1, Type t2){

		if(t1.isPointer()){
			if(t2.isEqual(t1) || t2.isIntegral()){
				return t1;
			}
			else{
				std::cout << "Error : Unable to perform poitner arithmetic with given types" << std::endl;
			}
		}

		if(t2.isPointer()){
			if(t1.isEqual(t2) || t1.isIntegral()){
				return t2;
			}
			else{
				std::cout << "Error : Unable to perform poitner arithmetic with given types" << std::endl;
			}
		}

		//neither pointers

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
			t1 = integralPromotion(t1);	//perform integral promotion
			t2 = integralPromotion(t2);	//perform integral promotion

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

	void convertMemRegType(Type origT, Type targetT, memReg Reg, std::ostream& dst){
		origT = integralPromotion(origT);
		if(!origT.isEqual(targetT)){		//only convert if they differ...
			if(targetT.isPointer()){
				if(origT.isIntegral()){			//double check..
					memReg_read(Reg,"v0",dst); //load from mem into reg
					Type baseType(targetT);
					baseType.dec_pLevel();
					dst<<"sll $v0,$v0,"<<log2(baseType.bytes())<<std::endl;
					memReg_write(Reg, "v0",dst); //store from float_reg into mem
				}
			}
			else{
				if(origT.isIntegral() && targetT.isIntegral()){	//both integral
					memReg_read(Reg,"v0",dst); //load from mem into reg
					//memoryOffsetRead(origT, "v0", "fp", (int)Reg, dst);
					if(targetT.is(UChar)){
						dst<<"andi $v0,$v0,0x000000ff"<<std::endl;
					}
					else if(targetT.is(UShort)){
						dst<<"andi $v0,$v0,0x0000ffff"<<std::endl;
					}
					else if(targetT.is(UInt) || targetT.is(ULong)){
						//dst<<"andi $v0,$v0,0xffffffff"<<std::endl;
					}
					else if(targetT.is(Char)){
						dst<<"sll $v0,$v0,24"<<std::endl;
						dst<<"sra $v0,$v0,24"<<std::endl;
					}
					else if(targetT.is(Short)){
						dst<<"sll $v0,$v0,16"<<std::endl;
						dst<<"sra $v0,$v0,16"<<std::endl;
					}
					else if(targetT.is(Int) || targetT.is(Long)){
					}
					//memoryOffsetWrite(targetT, "v0", "fp", (int)Reg, dst);
					memReg_write(Reg, "v0",dst); //store  into mem
				}
				else if(!origT.isIntegral() && !targetT.isIntegral()){	//both float
					//assume single float only so do nothing
				}
				else{	//one float, one integral...
					if(origT.isIntegral()){ //integral to float
						memReg_read_f(Reg, "f0",dst); //load from mem into float_reg
						dst<<"cvt.s.w	$f0,$f0"<<std::endl;//conversion from word to single
						memReg_write_f(Reg, "f0",dst); //store from float_reg into mem
					}
					else{	//float to signed int
						memReg_read_f(Reg, "f0",dst); //load from mem into float_reg
						dst<<".set	macro"		<<std::endl;
						dst<<"trunc.w.s $f0,$f0,$2"<<std::endl;
						dst<<".set	nomacro"	<<std::endl;
						dst<<"nop"				<<std::endl;
						memReg_write_f(Reg, "f0",dst); //store from float_reg into mem
						convertMemRegType(Type(Int), targetT, Reg, dst); //signed int to target.
					}
				}
			}
		}
	}
};


inline void parseCharSeq(std::string str_val, std::vector<char>& v){
	const char* str = str_val.c_str();
	std::stringstream ss;
	int escMode = 0;
	int octMode = 0;
	int hexMode = 0;
	int simMode = 0;
	int tmpVal = 0;
	for(unsigned int i =0; i<strlen(str);i++){
		if(str[i] == '\\' && ! escMode){
			escMode = 1;
			continue; //skip the slash
		}
		if(escMode){
			escMode = 0;
			if(str[i] == 'x'){
				hexMode = 1;
				continue;	//skip the x
			}
			else if(str[i] == '0'){
				octMode = 1;
			}
			else{
				simMode = 1;
			}
		}
		if(simMode){
			if(str[i] == 'a')
				v.push_back('\a');
			else if(str[i] == 'b')
				v.push_back('\b');
			else if(str[i] == 'f')
				v.push_back('\f');
			else if(str[i] == 'n')
				v.push_back('\n');
			else if(str[i] == 'r')
				v.push_back('\r');
			else if(str[i] == 't')
				v.push_back('\t');
			else if(str[i] == 'v')
				v.push_back('\v');
			else if(str[i] == '\'')
				v.push_back('\'');
			else if(str[i] == '?')
				v.push_back('\?');
			else if(str[i] == '"')
				v.push_back('\"');
			else if(str[i] == '\\')
				v.push_back('\\');
			simMode = 0;
		}
		else if(hexMode){
			bool conv = 0;
			if(isxdigit(str[i])){
				ss << str[i];
				if(i == strlen(str)-1){	//last char
					i = i + 1;
					conv = 1;
				}
			}
			else{
				conv = 1;
			}
			if(conv){
				tmpVal = std::stoull(ss.str(),0,16);
				v.push_back(tmpVal);
				ss.str("");
				hexMode = 0;
				i = i -1;
			}
		}
		else if(octMode){
			bool conv = 0;
			std::string temp; temp = (char)str[i];
			if(isdigit(str[i]) && std::stoull(temp,0,10) < 8){
				ss << str[i];
				octMode++;
				if(i == strlen(str)-1 || octMode == 4){	//last char or 3rd oct num
					i = i + 1;
					conv = 1;
				}
			}
			else{
				conv = 1;
			}
			if(conv){
				tmpVal = std::stoull(ss.str(),0,8);
				v.push_back(tmpVal);
				ss.str("");
				octMode = 0;
				i = i-1;
			}
		}
		else{
			v.push_back(str[i]);
		}
	}
}


















#endif