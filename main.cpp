/*
Written by Andrew M. Hall
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <map>
#include <string>
#include <cstring>
#include "matrix.h"

const int defaultPrecision = 3;
const int numArgs = 5;

//Codes used to identify command line options, also used as keys for ArgMap
enum ArgCode{
    DIMENSION = 0,
    INPUT,
    OUTPUT,
    PRECISION,
    HELP
};

//Hold data about arguments, used to dynamically create help message and parse arguments from command line
struct Argument{
    const char* longCode;
    const char* shortCode;
    const char* description;
    ArgCode code;
    bool mandatory;
    Argument(const char* l, const char* s, const char* d, ArgCode c, bool m):
        longCode(l), shortCode(s), description(d), code(c), mandatory(m){};
}   arguments[numArgs] {
Argument("--dimension", "-d", "The dimension, n, of the input nxn matrix", DIMENSION, true),
Argument("--input", "-i", "The name of the input file that contains the matrix to be inverted",INPUT, true),
Argument("--output", "-o", "The name of a file, which the inverted matrix will be written to", OUTPUT, false),
Argument("--precision", "-p", "The precision (number of decimal places) which the inverted matrix will be displayed (default 3)", PRECISION, false),
Argument("--help", "-h", "Display help message", HELP, false)
};

//Map used to hold ArgCodes/Value pairs
typedef std::map<ArgCode, std::string> argMap;

//Helper functions
bool setPrec(std::ostream &out, const std::string &str);
inline bool argGiven(const argMap &m, ArgCode a);
std::string getHelpMessage(const char* name);

int main(int argc, char *argv[])
{
    //Place all command line arguments into argument map using Argument as key
    argMap inputArguments;
    std::string helpMessage = getHelpMessage(argv[0]);
    //Iterate through command line arguments
    for (int i = 1; i < argc; i+=2){
        //Deal with --help flag being the
        if (i+1 == argc && strcmp(argv[i], arguments[HELP].shortCode) && strcmp(argv[i], arguments[HELP].longCode)){
            std::cout << "Incorrect command line arguments, mismatch on " << argv[i] << std::endl;
            return 0;
        }
        for (int a = 0; a < numArgs; a++){
            if (!strcmp(argv[i], arguments[a].longCode) || !strcmp(argv[i], arguments[a].shortCode)){
                if (arguments[a].code == HELP){
                    std::cout << helpMessage << std::endl;
                    return 0;
                }
                inputArguments[arguments[a].code] = argv[i+1];
            }
        }
    }
    //The mandadtory arguments are Dimension and Input, if they are not present, then warn the user to user and exit.
    for (int i = 0; i < numArgs; i++){
        if (arguments[i].mandatory && !argGiven(inputArguments, arguments[i].code)){
            std::cout << "Must have at least:";
            for (int a = 0; a < numArgs; a++){
                if (arguments[a].mandatory){
                    std::cout << "\n\t" << arguments[a].shortCode << " or " << arguments[a].longCode;
                }
            }
            std::cout << "\n\t or use the -h or --help flag to display help";
            std::cout << '\n' << helpMessage << std::endl;
            return 0;
        }
    }

    //Hold the dimension of the matrix in question
    int dim;
    //casting command line argument to integer requires much checking
    try{
        dim = std::stoi(inputArguments[DIMENSION]);
        if (dim <= 1){
            std::cout << "The dimension must be greater than 1" << std::endl;
            return 0;
        }
    } catch (std::invalid_argument e){
        std::cout << inputArguments[DIMENSION] << " is not a valid dimension, dimension must be an integer" << std::endl;
        return 0;
    }

    //Open input file
    std::ifstream matrix_file(inputArguments[INPUT]);
    if( !matrix_file.is_open() ){
        std::cout << "could not open file: " << inputArguments[INPUT] << std::endl;
        return 0;
    }

    //Read file contents into data vector
    std::vector<double> data;
    std::copy(
        std::istream_iterator<double>(matrix_file),
        std::istream_iterator<double>(),
        std::back_inserter(data)
    );
    matrix_file.close();

    try {
        Matrix::matrix<double> A(dim, dim, &data);
        //If the output is to go to an output file
        if (!argGiven(inputArguments, OUTPUT)){
            //Set precision
            if (!argGiven(inputArguments, PRECISION)){
                std::cout.precision(defaultPrecision);
            } else if (!setPrec(std::cout, inputArguments[PRECISION])){
                return 0;
            }
            //INVERT!
            std::cout << Matrix::invert(A);
        } else {
            //Open/Create output file
            std::ofstream outputFile (inputArguments[OUTPUT]);
            if (!outputFile.is_open()){
                std::cout << "Could not open file: " << inputArguments[OUTPUT] << std::endl;
                return 0;
            }
            //Set precision
            if (!argGiven(inputArguments, PRECISION)){
                outputFile.precision(defaultPrecision);
            } else if (!setPrec(outputFile, inputArguments[PRECISION])){
                return 0;
            }
            //INVERT!
            outputFile << Matrix::invert(A);
            outputFile.close();
        }
    } catch (Matrix::matrixException e){
        std::cout << e.getErrorMessage() << std::endl;
        return 0;
    }
    return 0;
}

inline bool argGiven(const argMap &m, ArgCode a){
    return (m.find(a) != m.end() );
}

/*
Set the precision of an output stream based on the command line argument
This saves having to do all the checking twice
return true if all is successful, return false if operaion fails
*/
bool setPrec(std::ostream &out, const std::string &str){
    try {
        int p = std::stoi(str);
        if (p < 0 || p > 20) {
            std::cout << str << " is not a valid precision\n precision must be greater than or equal to 0 and less than or equal to 20" << std::endl;
            return false;
        }
        out.precision(p);
    } catch (std::invalid_argument e) {
        std::cout << str << " is not a valid precision, precision must be an integer" << std::endl;
        return false;
    }
    return true;
}
/*
Create dynamic help message based on possible arguments
This is better than a static message, because it makes the help message much easier to keep  up to date
*/
std::string getHelpMessage(const char* name){
    std::string output = "usage: $ ";
    output.append(name);
    for (int i = 0; i < numArgs; i++){
        if (!arguments[i].mandatory){
            output.append("   [");
            output.append(arguments[i].shortCode);
            output.append(" | ");
            output.append(arguments[i].longCode);
            output.append("]");
        } else {
            output.append("   ");
            output.append(arguments[i].shortCode);
            output.append(" | ");
            output.append(arguments[i].longCode);
        }
    }
    output.append("\n\t");
    for (int i = 0; i < numArgs; i++){
        output.append(arguments[i].shortCode);
        output.append(", ");
        output.append(arguments[i].longCode);
        output.append(": ");
        output.append(arguments[i].description);
        output.append("\n\t");
    }
    return output;
}
