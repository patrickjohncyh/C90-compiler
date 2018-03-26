#include "ast.hpp"
#include <fstream>

int labelCount = 0;

void print_labaled_const(std::ostream &dst, Context& ctx);
void print_python_main(std::ostream &dst);

int main(int argc, char* argv[]){

	if(argc==5){
		yyin = fopen(argv[2],"r");
		std::ofstream outfile(argv[4]);
		const ASTNode *root = parse();

		if(std::string(argv[1]) == "--translate"){
			TranslateContext tc;
			root->to_python(outfile,"",tc);
			print_python_main(outfile);
		}
		else if(std::string(argv[1]) == "--parse"){
			root->to_c(outfile,"");
		}
		else if(std::string(argv[1]) == "-S"){
			Context c;
			root->to_mips(outfile,c);
			print_labaled_const(outfile,c);
		}
	}
	else{
		const ASTNode *root = parse();
		if(std::string(argv[1]) == "--parse"){
			root->to_c(std::cout,"");
		}
		else if(std::string(argv[1]) == "--translate"){
			TranslateContext tc;
			root->to_python(std::cout,"",tc);
			print_python_main(std::cout);
		}
		else if(std::string(argv[1]) == "-S"){
			Context c;
			root->to_mips(std::cout,c);
			print_labaled_const(std::cout,c);
		}
	}
	return 0;
}

void print_python_main(std::ostream &dst){
	dst << "if __name__ == \"__main__\":" << std::endl;
	dst << "  import sys" << std::endl;
	dst << "  ret=main()" << std::endl;
	dst << "  sys.exit(ret)" << std::endl;	
}

void print_labaled_const(std::ostream &dst, Context& ctx){
	for(auto it = ctx.labeled_constant.cbegin(); it !=  ctx.labeled_constant.cend(); ++it){
		if(it->second.second == "asciiz"){
			dst<<".rdata"<<std::endl;
			dst<<it->first<< ":\n";
			dst<<"."<<it->second.second<<" "<<it->second.first<<"\n";
		}
		else{
			dst<<it->first<<":\n";
			dst<<"."<<it->second.second<<" "<<it->second.first<< "\n";
		}
	}
}




