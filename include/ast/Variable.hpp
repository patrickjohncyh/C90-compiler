#ifndef VARIABLE_HPP
#define VARIABLE_HPP


enum dType{
	Basic,
	Array,
	Pointer,
	Union,
	Struct,
	Function
};

enum Scope{
	global,
	local
};


class Variable{
	private:
		Scope 	scope;
		Type 	type;
		dType 	dtype;
		int 	addr;


	public:
		Variable();

		Variable(Scope _scope, Type _type, dType _dtype,  int _addr)
		:scope(_scope),type(_type),dtype(_dtype),addr(_addr){}

		int getAddr(){
			return addr;
		}
		bool getScope(){
			return scope;
		}

		Type getType(){
			return type;
		}

		bool isArray(){
			if(dtype != Array)
				return false;
			return true;
		}

};










#endif
