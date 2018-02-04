#include "ast.hpp"

int main(){

	const ASTNode *root = parse();
	root->print(std::cout);
	std::cout << std::endl;

	return 0;
}