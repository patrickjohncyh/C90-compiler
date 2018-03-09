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

	public:
		Type();
		Type(basicType _type):type(_type){}

		basicType getType(){
			return type;
		}

		bool is(basicType t){
			if(type == t)
				return true;
			return false;
		}

		bool isIntegral(){
			if(type > 0 && type < 9 )
				return true;
			return false;
		}

		bool isSigned(){
			if(is(UChar) || is(UShort) || is(UInt) || is(ULong)){
				return false;
			}
			return true;
		}


		int bytes(){
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

};








#endif
