#ifndef STATEMENTS_HPP
#define STATEMENTS_HPP



class Statement : public ASTNode{

protected:
	Statement *next;
	Statement(Statement *_next = NULL):next(_next){} 

};



class CompoundStatement : public Statement{

	private:
		Statement *s_list;

	public:
		CompoundStatement( Statement* _s_list = NULL, Statement *_next = NULL ):Statement(_next),s_list(_s_list){}

		virtual void print(std::ostream &dst) const override{
			dst << "CompoundStatement ( ";
			s_list->print(dst);
			dst << ")";
		}


};

class JumpStatement : public Statement{

	private:
		std::string str;

	public:

		JumpStatement( std::string _str, Statement *_next = NULL):Statement(_next),str(_str){
			
		}

		virtual void print(std::ostream &dst) const override{
			dst << "JumpStatement ( " << str << "; )";
		}

};




#endif