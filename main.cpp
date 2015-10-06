#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <map>
#include <string>
#include "matrix.h"

enum Argument{
    Dimension = 0,
    Input,
    Output,
    Precision
};

const char* helpMessage = "usage:\n\tinvert-matrix -d Dimension -i Input [-o Output] [-p Precision] [-h Help]\n\tDimension: The dimension of the matrix, greater than 1\n\tInput: Input file name of file containing input matrix\n\tPrecision: Desired precision of output matrix\n\tHelp: This help message";
const int defaultPrecision = 3;

bool setPrec(std::ostream &out, const std::string &str);

int main(int argc, char *argv[])
{
    //Place all command line arguments into argument map using Argument Enum as key
    std::map<Argument, std::string> arguments;
    for (int i = 1; i < argc; i+=2){
        if (i+1 == argc){
            std::cout << "Incorrect command line arguments, mismatch on " << argv[i] << std::endl;
            return 0;
        }
        switch(argv[i][1]){
            case 'd' : arguments[Dimension] = argv[i+1];break;
            case 'i' : arguments[Input] = argv[i+1];break;
            case 'o' : arguments[Output] = argv[i+1];break;
            case 'p' : arguments[Precision] = argv[i+1];break;
            case 'h' :
                std::cout << helpMessage << std::endl;
                return 0;
            default  : std::cout << argv[i][1] << " is not a valid command line argument"; return 0;
        }
    }
    //The mandatory arguments are Dimension and Input, if they are not present, then warn the user to user and exit.
    if ( arguments.find(Input) == arguments.end() || arguments.find(Dimension) == arguments.end() ){
        std::cout << "Must have -i and -d arguments\n" << helpMessage << std::endl;
        return 0;
    } else {
        //Hold the dimension of the matrix in question
        int dim;
        //casting command line argument to integer requires much checking
        try{
            dim = std::stoi(arguments[Dimension]);
            if (dim <= 1){
                std::cout << "The dimension must be greater than 1" << std::endl;
                return 0;
            }
        } catch (std::invalid_argument e){
            std::cout << arguments[Dimension] << " is not a valid dimension, dimension must be an integer" << std::endl;
            return 0;
        }

        //Open input file
        std::ifstream matrix_file(arguments[Input]);
        if( !matrix_file.is_open() ){
            std::cout << "could not open file: " << arguments[Input] << std::endl;
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
            if (arguments.find(Output) == arguments.end()){
                //Set precision
                if (arguments.find(Precision) == arguments.end()){
                    std::cout.precision(defaultPrecision);
                } else {
                    if (!setPrec(std::cout, arguments[Precision]))
                        return 0;
                }
                //INVERT!
                std::cout << Matrix::invert(A);
            } else {
                //Open/Create output file
                std::ofstream output(arguments[Output]);
                if (!output.is_open()){
                    std::cout << "Could not open file: " << arguments[Output] << std::endl;
                    return 0;
                }
                //Set precision
                if (arguments.find(Precision) == arguments.end()){
                    output.precision(defaultPrecision);
                } else {
                    if (!setPrec(output, arguments[Precision]))
                        return 0;
                }
                //INVERT!
                output << Matrix::invert(A);
                output.close();
            }
        } catch (Matrix::matrixException e){
            std::cerr << e.getErrorMessage() << std::endl;
            return 0;
        }
    }
    return 0;
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

