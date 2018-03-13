#ifndef TYPE_HPP
#define TYPE_HPP




enum basicType {
	Void,		//0
	Char,		//1
	UChar,		//2
	Short,		//3
	UShort,		//4
	Int,		//5
	UInt,		//6
	Long,		//7
	ULong,		//8
	Float,		//9
	Double,		//10
	LongDouble	//11
};

class Type{
private:
	basicType type;
	int p_level=0;
	int a_level=0;

public:
	Type();
	Type(basicType _type):type(_type){}
	Type(basicType _type, int _p_level):type(_type),p_level(_p_level){}


	void inc_aLevel(){
		a_level++;
	}

	int get_aLevel(){
		return a_level;
	}

	int get_pLevel(){
		return p_level;
	}

	void inc_pLevel(){
		p_level++;
	}

	void dec_pLevel(){
		if(p_level == 0){
			if(a_level){
				a_level--;
			}
		}
		else{
			p_level--;
		}		
	}

	basicType getType(){
		return type;
	}

	bool is(basicType t){
		if(type == t)
			return true;
		return false;
	}

	bool isEqual(Type t){
		if(t.type != type)
			return false;
		if(t.p_level + t.a_level != p_level + a_level) //super hacky. idk if it is correct. some sort of pointer equivalence..
			return false;
		return true;
	}

	bool isPointer(){
		return (p_level>0 || a_level>0);
	}


	bool isIntegral(){
		if(isPointer()) return false;
		return  (type < Float && type > Void);
	}

	bool isSigned(){
		if(isPointer()) return false;
		
		if(is(UChar) || is(UShort) || is(UInt) || is(ULong)){
			return false;
		}
		return true;
	}

	int bytes(){
		if(isPointer()) return 4;
		switch(type){
			case Void:
				return 0;
			case Char:
			case UChar:
				return 1;
			case Short:
			case UShort:
				return 2;;
			case Int:
			case UInt:
			case Long:
			case ULong:
			case Float:
				return 4;
			case Double:
			case LongDouble:
				return 8;
			default:
				return 4;
		}
	}

	std::string storage_type(){
		if(isPointer()){
			return "word";
		}
		if(isIntegral()){
			if(bytes() == 1)
				return "byte";
			else if(bytes() == 2)
				return "half";
			else if(bytes() == 4)
				return "word";
		}
		else
			return "float";
	}

};


#endif
