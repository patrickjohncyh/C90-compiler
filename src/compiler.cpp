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
			for(auto it = c.labeled_constant.cbegin(); it !=  c.labeled_constant.cend(); ++it){
				if(it->second.second == "asciiz"){
					outfile<<it->first<< ":\n";
   				 	outfile<<"."<<it->second.second<<" \""<<it->second.first<<"\"\n";
				}
				else{
					outfile<<it->first<<":\n";
   				 	outfile<<"."<<it->second.second<<" "<<it->second.first<< "\n";
				}
   			
			}
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
			for(auto it = c.labeled_constant.cbegin(); it !=  c.labeled_constant.cend(); ++it){
				if(it->second.second == "asciiz"){
					std::cout<<it->first<< ":\n";
   				 	std::cout<<"."<<it->second.second<<" \""<<it->second.first<<"\"\n";
				}
				else{
					std::cout<<it->first<<":\n";
   				 	std::cout<<"."<<it->second.second<<" "<<it->second.first<< "\n";
				}
   			
			}


		}
	}
	return 0;
}





