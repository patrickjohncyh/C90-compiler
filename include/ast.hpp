#ifndef AST_HPP
#define ASTN_HPP

#include "stdio.h"
#include <iostream>
#include <string>
#include <cstdlib>


#include "ast/ASTNode.hpp"
#include "ast/declarations.hpp"
#include "ast/statements.hpp"
#include "ast/expressions.hpp"
#include "ast/primitives.hpp"

//extern const Expression *parseAST();
extern const ASTNode* parse();





#endif