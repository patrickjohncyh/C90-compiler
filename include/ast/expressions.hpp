#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <iomanip>


class Expression : public ASTNode{
	public:
		virtual void print_struct(std::ostream &dst, int m) const =0;
};


/********************** Unary Expressions ************************/

class UnaryExpression : public Expression{
	protected:
		Expression* expr;
	public:
		UnaryExpression(Expression* _expr):expr(_expr){}
};

/********************** Post Fix Expressions *********/


class PostIncrementExpression : public UnaryExpression{
	public:
		PostIncrementExpression(Expression* _expr):UnaryExpression(_expr){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			expr->print_struct(dst,m);
			dst << "++" << std::endl;
		}
};


class FunctionCallExpression : public UnaryExpression{
	private:
		Expression* a_list;

	public:

		FunctionCallExpression(Expression *_expr , Expression* _a_list = NULL)
		:UnaryExpression(_expr),a_list(_a_list){}
		
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "FunctionCallExpression [ Identifier ( ";  expr->print_struct(dst,m); dst << " ) ";
			// TO IMPLEMENT ARUGMENT LIST
			dst << "]" << std::endl;
		}
};


// TO IMPLEMENT OTHER POSTFIX EXPRESSIONS


/********************** Pre Fix Expressions *********/






/********************** Binary Expressions ************************/
class BinaryExpression : public Expression{
	protected:
		Expression* left;
		Expression* right;

	public:
		BinaryExpression(Expression* _left, Expression* _right):left(_left),right(_right){}

		virtual const char *getOpcode() const =0;

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst<<"( ";
	        left->print_struct(dst,m);
	        dst<<" ";
	        dst<<getOpcode();
	        dst<<" ";
	        right->print_struct(dst,m);
	        dst<<" )";
		}
};


/********* Arithmetic Binary Expressions *********/

class MultExpression : public BinaryExpression{
	public:
		MultExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "*";
		}
};

class DivExpression : public BinaryExpression{
	public:
		DivExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "/";
		}
};

class AddExpression : public BinaryExpression{
	public:
		AddExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "+";
		}
};

class SubExpression : public BinaryExpression{
	public:
		SubExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "-";
		}
};

/********* Relational Binary Expressions *********/

class LessThanExpression : public BinaryExpression{
	public:
		LessThanExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "<";
		}
};

class MoreThanExpression : public BinaryExpression{
	public:
		MoreThanExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return ">";
		}
};

class LessThanEqExpression : public BinaryExpression{
	public:
		LessThanEqExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "<=";
		}
};

class MoreThanEqExpression : public BinaryExpression{
	public:
		MoreThanEqExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return ">=";
		}
};

class EqualityExpression : public BinaryExpression{
	public:
		EqualityExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "==";
		}
};

class NotEqualityExpression : public BinaryExpression{
	public:
		NotEqualityExpression(Expression* _left, Expression* _right):BinaryExpression(_left,_right){}

		virtual const char *getOpcode() const override{
			return "!=";
		}
};


/********* Bitwise Binary Expressions *********/




/********* Logical Binary Expressions *********/




/********************** Assignment Expressions ************************/

class AssignmentExpression : public Expression{
	private:
		Expression* lvalue;
		Expression* expr;
		
 		
	public:
		AssignmentExpression(Expression* _lvalue, Expression* _expr)
		:lvalue(_lvalue),expr(_expr){}
};



class DirectAssignmentExpression : public AssignmentExpression{

	public:
		DirectAssignmentExpression(Expression* _lvalue, Expression* _expr)
		: AssignmentExpression(_lvalue,_expr){}

		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "DirectAssignemntExpression" << std::endl;
		}

};


// TO IMPLMENET OTHER ASSIGNMENT I.E += ,-=, *= ETC






/********************** Ternary Expressions ************************/

// TO IMPLMENET CONDITIONAL EXPRESSION


#endif