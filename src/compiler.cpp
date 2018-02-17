#include "ast.hpp"

int main(){

	const ASTNode *root = parse();
	root->print_struct(std::cout,0);
	root->to_python(std::cout,"");
	std::cout << std::endl;

	return 0;
}