#ifndef TYPE_HPP
#define TYPE_HPP




enum basicType {
	Void,
	Char,
	UChar,
	Short,
	UShort,
	Int,
	UInt,
	Long,
	ULong,
	Float,
	Double,
	LongDouble
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
