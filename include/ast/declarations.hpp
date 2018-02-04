#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP


class TranslationUnit : public ASTNode{
	protected:
		astNodePtr left;
		astNodePtr right;

	private:

	public:
		TranslationUnit(astNodePtr _left, astNodePtr _right):left(_left),right(_right){}

		virtual void print(std::ostream &dst) const override{
				left->print(dst);
				dst << " ";
				right->print(dst);

		}
};




class ExternalDeclaration : public ASTNode{
	private:
		std::string str;

	public:
		ExternalDeclaration(std::string _str): str(_str){}

		virtual void print(std::ostream &dst) const override{
			dst << str;
		}
};



#endif