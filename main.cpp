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

const char* helpMessage = "usage:\n\tinvert-matrix -d | --dimension Dimension -i | --input Input [-o | --output Output] [-p | --precision Precision] [-h Help]\n\tDimension: The dimension of the matrix, greater than 1\n\tInput: Input file name of file containing input matrix\n\tPrecision: Desired precision of output matrix\n\tHelp: This help message";
const int defaultPrecision = 3;
const int numArgs = 5;

enum ArgCode{
    DIMENSION = 0,
    INPUT,
    OUTPUT,
    PRECISION,
    HELP
};

struct Argument{
    const char* longCode;
    const char* shortCode;
    ArgCode code;
    bool mandatory;
    Argument(const char* l, const char* s, ArgCode c, bool m):longCode(l), shortCode(s), code(c), mandatory(m){};

}   arguments[numArgs] {
Argument("--dimension", "-d", DIMENSION, true),
Argument("--input", "-i", INPUT, true),
Argument("--output", "-o", OUTPUT, false),
Argument("--precision", "-p", PRECISION, false),
Argument("--help", "-h", HELP, false)
};

typedef std::map<ArgCode, std::string> argMap;

bool setPrec(std::ostream &out, const std::string &str);
bool argGiven(const argMap &m, ArgCode a);



int main(int argc, char *argv[])
{
    //Place all command line arguments into argument map using Argument as key
    argMap inputArguments;

    for (int i = 1; i < argc; i+=2){
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
            std::cout << "Musg have at least:";
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
        if (p < 0 || p > 12) {
            std::cout << str << " is not a valid precision\n precision must be greater than or equal to 0 and less than or equal to 12" << std::endl;
            return false;
        }
        out.precision(p);
    } catch (std::invalid_argument e) {
        std::cout << str << " is not a valid precision, precision must be an integer" << std::endl;
        return false;
    }
    return true;
}
