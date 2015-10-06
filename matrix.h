/*
Written by Andrew M. Hall
*/

#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <vector>
#include <iostream>
#include "matrixError.h"

namespace Matrix
{

template <class Type>
class matrix;

template <class Type>
class vector;

//Generic Matrix Friend functions
template <class Type> Type determinant(const matrix<Type> &a, int row);
template <class Type> Type determinant2x2(const matrix<Type> &a);
template <class Type> matrix<Type> adjoint(const matrix<Type> &a);
template <class Type> matrix<Type> cofactor(const matrix<Type> &a);
template <class Type> matrix<Type> transpose(const matrix<Type> &a);
template <class Type> matrix<double> invert2x2(const matrix<Type> &a);
template <class Type> matrix<double> invert(const matrix<Type> &a);
template <class Type> matrix<Type> operator+(const matrix<Type> &a, const matrix<Type> &b);
template <class Type> matrix<Type> operator-(const matrix<Type> &a);
template <class Type> matrix<Type> operator-(const matrix<Type> &a, const matrix<Type> &b);
template <class Type> vector<Type> operator*(const matrix<Type> &a, const vector<Type> &b);
template <class Type> matrix<Type> operator*(const matrix<Type> &a, Type b);
template <class Type> matrix<Type> operator*(const matrix<Type> &a, const matrix<Type> &b);
template <class Type> bool operator==(const matrix<Type> &a, const matrix<Type> &b);
template <class Type> bool operator!=(const matrix<Type> &a, const matrix<Type> &b);
template <class Type> std::ostream& operator<<(std::ostream &out, const matrix<Type> &a);
template <class Type> std::string toString(const matrix<Type> &m);
template <class Type> Type operator*(const vector<Type> &a, const vector<Type> &b);

//matrix is the base class for this library, deskgned to be used with all numeric types
template <class Type>
class matrix
{
protected:
    int size;
    int width;
    int height;
    Type* data;
public:
    matrix(int in_width, int in_height);
    matrix() : data(nullptr), width(1), height(0), size(0) {};
    matrix(const matrix<Type> &in_matrix);
    matrix(int in_width, int in_height, std::vector<Type>* input);
    ~matrix()
    {
        delete[] data;
        data = nullptr;
    };
    int getWidth()const
    {
        return width;
    };
    int getHeight()const
    {
        return height;
    };
    Type* getRow(int y)const;
    Type* getColum(int x)const;
    void map(Type(*function)(Type));
    //Type conversion
    operator matrix<bool>();
    operator matrix<unsigned char>();
    operator matrix<short>();
    operator matrix<int>();
    operator matrix<float>();
    operator matrix<double>();
    //Matrix manipulation
    friend Type determinant <>(const matrix<Type> &a, int col);
    friend Type determinant2x2 <>(const matrix<Type> &a);
    friend matrix<Type> adjoint <>(const matrix<Type> &a);
    friend matrix<Type> cofactor <>(const matrix<Type> &a);
    friend matrix<Type> transpose <>(const matrix<Type> &a);
    friend matrix<double> invert2x2 <>(const matrix<Type> &a);
    friend matrix<double> invert <>(const matrix<Type> &a);
    matrix<Type>& operator=(const matrix<Type> &a);
    Type* operator[](int a)const;
    friend matrix<Type> operator+ <>(const matrix<Type> &a, const matrix<Type> &b);
    friend matrix<Type> operator- <>(const matrix<Type> &a);
    friend matrix<Type> operator- <>(const matrix<Type> &a, const matrix<Type> &b);
    friend vector<Type> operator* <>(const matrix<Type> &a, const vector<Type> &b);
    friend matrix<Type> operator* <>(const matrix<Type> &a, Type b);
    friend matrix<Type> operator* <>(const matrix<Type> &a, const matrix<Type> &b);
    friend bool operator==<>(const matrix<Type> &a, const matrix<Type> &b);
    friend bool operator!=<>(const matrix<Type> &a, const matrix<Type> &b);
    friend std::ostream& operator<< <>(std::ostream &out, const matrix<Type> &a);
    friend std::string toString <>(const matrix<Type> &m);
};

//Vector is a special case of Matrix, allowing slightly different operations
template <class Type>
class vector : public matrix < Type >
{
public:
    vector() : matrix<Type>::data(nullptr), matrix<Type>::height(0), matrix<Type>::width(0){};
    vector(int in_height, Type* in_data);
    vector(int in_height);
    Type operator[](int a)const;
    friend Type operator* <>(const vector<Type> &a, const vector<Type> &b);
};

//Constructor Section

/*
Constructor adds items from a vector into a matrix of specified width and height.
If more items are in the input data, than in the matrix, they are ignored. If there are less entries in the
vector, 0's are added to fill in the difference.
*/
template <class Type>
matrix<Type>::matrix(int in_width, int in_height, std::vector<Type>* input)
{
    /*static_assert(std::_Is_numeric<Type>::value,
    	"Cannot initialise matrix with non numeric types");*/
    width = in_width;
    height = in_height;
    size = (width > height) ? width : height;
    try
    {
        data = new Type[size*size];
    }
    catch (std::bad_alloc)
    {
        throw(matrixException(MEMORY_ERROR));
    }
    int i = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (i >= input->size()){
                data[y*size + x] = 0;
            } else {
                data[y*size + x] = input->at(i++);
            }
        }
    }
}

/*
Constructor creates array of specified width & height, but does not input ANY values,
this can be fixed by the c++ equivalent of the haskell: fmap (const 0) matrix
*/
template <class Type>
matrix<Type>::matrix(int in_width, int in_height)
{
    /*static_assert(std::_Is_numeric<Type>::value,
    	"Cannot initialise matrix with non numeric types");*/
    width = in_width;
    height = in_height;
    size = (width > height) ? width : height;
    try
    {
        data = new Type[size*size];
    }
    catch (std::bad_alloc)
    {
        throw(matrixException(MEMORY_ERROR));
    }
}

/*
Copy one matrix into another
*/
template <class Type>
matrix<Type>::matrix(const matrix<Type> &in_matrix)
{
    /*static_assert(std::_Is_numeric<Type>::value,
    	"Cannot initialise matrix with non numeric types");*/
    if (in_matrix.data)
    {
        width = in_matrix.width;
        height = in_matrix.height;
        size = in_matrix.size;
        try
        {
            data = new Type[size*size];
        }
        catch (std::bad_alloc)
        {
            throw(matrixException(MEMORY_ERROR));
        }
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                data[y*size + x] = in_matrix[y][x];
            }
        }
    }
    else
    {
        data = nullptr;
    }
}

/*
getColumn returns a pointer to a new array containing the x'th column
of a matrix.
*/
template <class Type>
Type* matrix<Type>::getColum(int x)const
{
    Type * output = nullptr;
    try
    {
        output = new Type[height];
    }
    catch (std::bad_alloc)
    {
        throw(matrixException(MEMORY_ERROR));
    }
    for (int y = 0; y < height; y++)
    {
        output[y] = data[x + y*size];
    }
    return output;
}

//Getters
/*
getRow returns a pointer to a new array containing the y'th row
of a matrix.
*/
template <class Type>
Type* matrix<Type>::getRow(int y)const
{
    Type* output = nullptr;
    try
    {
        output = new Type[width];
    }
    catch (std::bad_alloc)
    {
        throw(matrixException(MEMORY_ERROR));
    }
    int start = y*size;
    for (int x = 0; x < width; x++)
    {
        output[x] = data[start++];
    }
    return output;
}

/*
Inspired by haskell's fmap, map will apply a function to each cell within a matrix.
This reduces user boilerplate code.
*/
template <class Type>
void matrix<Type>::map(Type(*function)(Type))
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            data[y*size + x] = function(data[y*size + x]);
        }
    }
}

//Mathematical operations
/*
This is the basic case used for all other inversion, cofactor expansion recursivly aproaches this
simple case
    [a  b]
A = [c  d]

invert2x2(A) = 1/      [d  -b]
              det(A) * [-c  a]

*/
template <class Type>
matrix<double> invert2x2(const matrix<Type> &a)
{
    matrix<double> output(2, 2);
    double det = static_cast<double>(determinant2x2(a));
    if (!det)
        throw matrixException(MATH_ERROR);
    double d = 1.0 / det;
    output[0][0] = (a[1][1] * d);
    output[0][1] = (-a[0][1] * d);
    output[1][0] = (-a[1][0] * d);
    output[1][1] = (a[0][0] * d);
    return output;
}

/*
Invert a matrix using recursive method. This is find for up to 10*10.
If the matrix is not square, dimension error is thrown.
If the matrix has a determinant of 0, math error is thrown.
*/
template <class Type>
matrix<double> invert(const matrix<Type> &a)
{
    int w = a.width;
    int h = a.height;
    if (w != h)
        throw matrixException(DIMENSION_ERROR);
    if (h == 2)
    {
        return invert2x2(a);
    }
    matrix<double> output(w, h);
    double det = static_cast<double>(determinant(a,0));
    if (!det)
        throw matrixException(MATH_ERROR);
    return static_cast<matrix<double> >(adjoint(a)) * (1.0 / det);
}

/*
adjoint is the transpose of the cofactor expansion of a matrix
*/
template <class Type>
matrix<Type> adjoint(const matrix<Type> &a)
{
    return transpose(cofactor(a));
}

/*
calculate the determinant of a square, n*n matrix
*/
template <class Type>
Type determinant(const matrix<Type> &a, int row)
{
    int w = a.width;
    int h = a.height;
    if (h != w)
        throw matrixException(DIMENSION_ERROR);
    Type output = 0;
    bool s = (row == 0) || (row % 2 == 0);
    if (h == 2)
    {
        return (determinant2x2(a));
    }
    else
    {
        for (int tmpX = 0; tmpX < w; tmpX++)
        {
            int ycount = 0;
            matrix<Type> tmp(w - 1, h - 1);
            for (int y = 0; y < h; y++)
            {
                if (y != row)
                {
                    int xcount = 0;
                    for (int x = 0; x < w; x++)
                    {
                        if (x != tmpX)
                        {
                            tmp[ycount][xcount++] = a[y][x];
                        }
                    }
                    ycount++;
                }
            }
            output += s ? ((determinant(tmp,0) * (a[row][tmpX]))) : -((determinant(tmp,0) * (a[row][tmpX])));
            s = !s;
        }
    }
    return output;
}

/*
basic case for the determinant, the determinant of a 2*2 matrix has a simple formula
*/
template <class Type>
Type determinant2x2(const matrix<Type> &a)
{
    return a[0][0] * a[1][1] - a[0][1] * a[1][0];
}

/*
cofactor expansion of a matrix, used to find the inverse of a matrix
*/
template <class Type>
matrix<Type> cofactor(const matrix<Type> &a)
{
    int w = a.width;
    int h = a.height;
    if (h != w)
        throw matrixException(DIMENSION_ERROR);
    matrix<Type> output(w, h);
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            matrix<Type> det(w - 1, h - 1);
            int ycount = 0;
            for (int ydet = 0; ydet < h; ydet++)
            {
                int xcount = 0;
                if (ydet != y)
                {
                    for (int xdet = 0; xdet < w; xdet++)
                    {
                        if (xdet != x)
                        {
                            det[ycount][xcount++] = a[ydet][xdet];
                        }
                    }
                    ycount++;
                }
            }
            if ((y + x) % 2 == 0)
            {
                output[y][x] = determinant(det,0);
            }
            else
            {
                output[y][x] = -determinant(det,0);
            }
        }
    }
    return output;
}
/*
simple transpose of a matrix
*/
template <class Type>
matrix<Type> transpose(const matrix<Type> &a)
{
    matrix<Type> output(a);
    for (int y = 0; y < a.size; y++)
    {
        for (int x = 0; x < y; x++)
        {
            Type tmp = output.data[y*output.size + x];
            output.data[y*output.size + x] = output.data[x*output.size + y];
            output.data[x*output.size + y] = tmp;
        }
    }
    int tmp = output.width;
    output.width = output.height;
    output.height = tmp;
    return output;
}

/*
unary negation is a special case of scalar multiplication on a matrix
-A = A*(-1)
*/
template <class Type>
matrix<Type> operator-(const matrix<Type> &a)
{
    return a * static_cast<Type>(-1);
}

/*
subtraction is a special case of addition combined with unary negation
A - B = A + (-B)
*/
template <class Type>
matrix<Type> operator-(const matrix<Type> &a, const matrix<Type> &b)
{
    return a + (-b);
}

/*
inequality is a special case of equality
(A != B) == !(A == B)
*/
template <class Type>
bool operator!=(const matrix<Type> &a, const matrix<Type> &b)
{
    return !(a == b);
}

/*
matrix addition, done element wise
*/
template <class Type>
matrix<Type> operator+(const matrix<Type> &a, const matrix<Type> &b)
{
    int a_width = a.width;
    int a_height = a.height;
    int b_width = b.width;
    int b_height = b.height;
    if (a_width != b_width || a_height != b_height)
        throw matrixException(DIMENSION_ERROR);
    int myWidth = a_width;
    int myHeight = a_height;
    matrix<Type> output(myWidth, myHeight);
    for (int y = 0; y < myHeight; y++)
    {
        for (int x = 0; x < myWidth; x++)
        {
            output[y][x] = a[y][x] + b[y][x];
        }
    }
    return output;
}

/*
matrix assignment, similar to copy constructor
*/
template <class Type>
matrix<Type>& matrix<Type>::operator=(const matrix<Type> &a)
{
    if (this == &a)
    {
        return *this;
    }
    delete[] data;

    width = a.width;
    height = a.height;
    size = (width > height) ? width : height;

    if (a.data)
    {
        try
        {
            data = new Type[size*size];
        }
        catch (std::bad_alloc)
        {
            throw(matrixException(MEMORY_ERROR));
        }
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                data[y*size + x] = a[y][x];
            }
        }
    }
    else
    {
        data = nullptr;
    }
    return *this;
}

/*
access operator, points to the start of the a'th row.
Throw a bounds error if a is outside the matrix bounds
*/
template <class Type>
Type* matrix<Type>::operator[](int a)const
{
    if (a < 0 || a >= height)
        throw matrixException(BOUNDS_ERROR);
    return data + size * a;
}

template <class Type>
matrix<Type> operator*(const matrix<Type> &a, const matrix<Type> &b)
{
    if (a.width != b.height || a.height != b.width)
        throw matrixException(DIMENSION_ERROR);

    matrix<Type> output(a.height, b.width);
    for (int y = 0; y < a.height; y++)
    {
        Type * tmpRow = a.getRow(y);
        for (int x = 0; x < b.width; x++)
        {
            Type * tmpCol = b.getColum(x);
            Type total = 0;
            for (int i = 0; i < a.width; i++)
            {
                total += tmpRow[i] * tmpCol[i];
            }
            output[y][x] = total;
        }
    }
    return output;
}

//Matrix multiplication
template <class Type>
vector<Type> operator*(const matrix<Type> &a, const vector<Type> &b)
{
    int m_height = a.height;
    int m_width = a.width;
    int v_height = b.height;
    if (m_width != v_height)
        throw matrixException(DIMENSION_ERROR);
    vector<Type> output(m_height);
    for (int y = 0; y < m_height; y++)
    {
        Type total = 0;
        for (int x = 0; x < m_width; x++)
        {
            total += a[y][x] * b[x][0];
        }
        *output[y] = total;
    }
    return output;
}

//Scalar multiplication
template <class Type>
matrix<Type> operator*(const matrix<Type> &a, Type b)
{
    int height = a.height;
    int width = a.width;
    matrix<Type> output(width, height);
    for (int y = 0; y < a.height; y++)
    {
        for (int x = 0; x < a.width; x++)
        {
            output[y][x] = a[y][x] * b;
        }
    }
    return output;
}

//matrix equality, defined elementwise
template <class Type>
bool operator==(const matrix<Type> &a, const matrix<Type> &b)
{
    if (a.height != b.height || a.width != b.width)
    {
        return false;
    }
    int height = a.height, width = a.width;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (a[y][x] != b[y][x])
            {
                return false;
            }
        }
    }
    return true;
}

//output stream, used for printing and writing to files
template <class Type>
std::ostream& operator<<(std::ostream &out, const matrix<Type> &a)
{
    char space[5] = "    ";
    if (out.precision() <= 4){
        space[0] = '\t';
        space[1] = '\0';
    }
    int height = a.height;
    int width = a.width;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            out << a[y][x] << space;
        }
        out << std::endl;
    }
    return out;
}

//Used for a generic string representation of the matrix
template <class Type>
std::string toString(const matrix<Type> &m)
{
    std::string output;
    for (int y = 0; y < m.height; y++)
    {
        for (int x = 0; x < m.width; x++)
        {
            output.append(to_string(m[y][x]));
            output.append(",\t");
        }
        output.append("\n");
    }
    return output;
}

//static_cast section
//Defined for all numeric types, and boolean and unsigned char
template <class Type>
matrix<Type>::operator matrix<bool>()
{
    matrix<bool> output(width, height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            output[y][x] = static_cast<bool>(data[y*width + x]);
        }
    }
    return output;
}

template <class Type>
matrix<Type>::operator matrix<unsigned char>()
{
    matrix<unsigned char> output(width, height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            output[y][x] = static_cast<unsigned char>(data[y*size + x]);
        }
    }
    return output;
}

template <class Type>
matrix<Type>::operator matrix<short>()
{
    matrix<short> output(width, height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            output[y][x] = static_cast<short>(data[y*size + x]);
        }
    }
    return output;
}

template <class Type>
matrix<Type>::operator matrix<int>()
{
    matrix<int> output(width, height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            output[y][x] = static_cast<int>(data[y*size + x]);
        }
    }
    return output;
}

template <class Type>
matrix<Type>::operator matrix<float>()
{
    matrix<float> output(width, height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            output[y][x] = static_cast<float>(data[y*size + x]);
        }
    }
    return output;
}

template <class Type>
matrix<Type>::operator matrix<double>()
{
    matrix<double> output(width, height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            output[y][x] = static_cast<double>(data[y*size + x]);
        }
    }
    return output;
}

//vector functions
template <class Type>
vector<Type>::vector(int in_height)
{
    /*static_assert(std::_Is_numeric<Type>::value,
    	"Cannot initialise matrix with non numeric types");*/
    matrix<Type>::width = 1;
    matrix<Type>::height = in_height;
    matrix<Type>::size = matrix<Type>::height;
    try
    {
        matrix<Type>::data = new Type[matrix<Type>::size*matrix<Type>::size];
    }
    catch (std::bad_alloc)
    {
        throw(matrixException(MEMORY_ERROR));
    }
}

//Vector code

//Constructors
template <class Type>
vector<Type>::vector(int in_height, Type* in_data)
{
    /*static_assert(std::_Is_numeric<Type>::value,
    	"Cannot initialise matrix with non numeric types");*/
    matrix<Type>::width = 1;
    matrix<Type>::height = in_height;
    matrix<Type>::size = matrix<Type>::height;
    try
    {
        matrix<Type>::data = new Type[matrix<Type>::size*matrix<Type>::size];
    }
    catch (std::bad_alloc)
    {
        throw(matrixException(MEMORY_ERROR));
    }
    for (int i = 0; i < matrix<Type>::height; i++)
    {
        matrix<Type>::data[i*matrix<Type>::size] = in_data[i];
    }
}

//vector dot product
template <class Type>
Type operator* (const vector<Type> &a, const vector<Type> &b){
    if (a.height != b.height)
        throw matrixException(DIMENSION_ERROR);
    Type output = 0;
    for (int i = 0; i < a.height; i++){
        output+= a[i] * b[i];
    }
    return output;
}

template <class Type>
Type vector<Type>::operator[](int a)const
{
    if (a < 0 || a >= matrix<Type>::height)
        throw matrixException(BOUNDS_ERROR);
    return matrix<Type>::data + a;
}

}

#endif
