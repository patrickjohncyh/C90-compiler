#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <iomanip>


class Expression : public ASTNode{
	public:
		virtual void print_struct(std::ostream &dst, int m) const =0;
};


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

class AssignmentExpression : public Expression{
	private:
		std::string id;
		Expression* assign_expr;
 		
	public:
		AssignmentExpression(std::string _id, Expression* _assign_expr):id(_id),assign_expr(_assign_expr){}
		virtual void print_struct(std::ostream &dst, int m) const override{
			dst << "AssignmentExpression [ ";
			dst << id << " = ";
			assign_expr->print_struct(dst,m);
			dst << " ]";
		}
};

/*
class MultExpression : public Expression{
	public:
		virtual void print_struct(std::ostream &dst, int m) const override {}

};

class MultExpression : public Expression{
	public:
		virtual void print_struct(std::ostream &dst, int m) const override {}

};
*/







#endif