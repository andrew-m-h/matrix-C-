#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include "matrix.h"

int main(int argc, char *argv[])
{
    if (argc == 1){
        std::cout << "usage:\n\t invert-matrix height filename [-o output file]\n\t to invert a space seperated matrix\n\t invert-matrix" << std::endl;
        return 0;
    } else {
        int length;

        std::istringstream ass(argv[1]);
        ass >> length;

        std::ifstream matrix_file (argv[2]);
        if( !matrix_file.is_open() ){
            std::cout << "could not open file: " << argv[2] << std::endl;
            return 0;
        } else {
            std::vector<double> data;
            std::copy(
                std::istream_iterator<double>(matrix_file),
                std::istream_iterator<double>(),
                std::back_inserter(data)
            );
            try {
                Matrix::matrix<double> A(length, length, &data);
                if (argc == 3) {
                    std::cout.precision(3);
                    std::cout << Matrix::invert(A);
                } else if (argc == 5 && argv[3][1] == 'o') {
                    std::ofstream output (argv[4]);
                    output.precision(3);
                    output << Matrix::invert(A);
                    output.close();
                }
                matrix_file.close();
            } catch (Matrix::matrixException e){
                std::cerr << e.getErrorMessage() << std::endl;
                return 0;
            }
        }
    }
}
