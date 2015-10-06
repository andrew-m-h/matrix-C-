#include <string>

namespace Matrix{
	using namespace std;
	enum possible_errors {
		SUCCESS = 0,
		MATH_ERROR,
		DIMENSION_ERROR,
		MEMORY_ERROR,
		BOUNDS_ERROR,
		OTHER
	};

	class matrixException {
	private:
		string errorMessage;
		possible_errors errorCode;
		matrixException() {}
	public:
		matrixException(string errormsg)
			:errorMessage(errormsg), errorCode(OTHER){}
		matrixException(possible_errors code)
			:errorCode(code){
			switch (code){
			case SUCCESS:
				errorMessage = "Success";
				break;
			case MATH_ERROR:
				errorMessage = "Matrix Math error, possible cause:\nInverse, cofactor, adjoint or determinant does not exist";
				break;
			case DIMENSION_ERROR:
				errorMessage = "Matrix Dimension Error, possible cause:\n\tTake Inverse, cofactor, adjoint of non square matrix\n\tdot product of vectors of different sizes";
				break;
			case MEMORY_ERROR:
				errorMessage = "Matrix error occured when allocating dynamic memory";
				break;
			case BOUNDS_ERROR:
				errorMessage = "Matrix error occured when trying to access array element";
				break;
			case OTHER:
				errorMessage = "An error occured during matrix operation";
			}
		}
		string getErrorMessage(){
			return errorMessage;
		}
		possible_errors getErrorCode(){
			return errorCode;
		}
	};
}
