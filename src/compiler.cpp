#include "ast.hpp"
#include <fstream>

int labelCount = 0;

int main(int argc, char* argv[]){

	if(argc==5){

		yyin = fopen(argv[2],"r");
		std::ofstream outfile(argv[4]);
		std::ostream& ostream(outfile);
		const ASTNode *root = parse();

		if(std::string(argv[1]) == "--translate"){
			TranslateContext tc;
			root->to_python(outfile,"",tc);

			ostream << "if __name__ == \"__main__\":" << std::endl;
		    ostream << "  import sys" << std::endl;
		    ostream << "  ret=main()" << std::endl;
		    ostream << "  sys.exit(ret)" << std::endl;	
		}
		else if(std::string(argv[1]) == "--parse"){
			root->to_c(outfile,"");
		}
		else if(std::string(argv[1]) == "-S"){
			Context c;
			root->to_mips(outfile,c);
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
		}
		else if(std::string(argv[1]) == "-S"){
			Context c;
			root->to_mips(std::cout,c);
		}
	}
	return 0;
}