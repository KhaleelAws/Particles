//made by Venkata G. Ande and Aws Khaleel

#include "Matrices.h"

namespace Matrices
{
    Matrix::Matrix(int _rows, int _cols)
    {
        rows = _rows;
        cols = _cols;

        a.resize(rows, vector<double>(cols));
        for (int i = 0; i < _rows; i++)
        {
            for (int j = 0; j < _cols; j++)
            {
                a[i][j] = 0;
            }
        }
    }
    Matrix operator+(const Matrix& a, const Matrix& b)
    {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
        {
            throw ("Incomatable sizes \n");
        }
        else
        {
            Matrix c(a.getRows(), a.getCols());
            for (int i = 0; i < a.getRows(); i++)
            {
                for (int j = 0; j < a.getCols(); j++)
                {
                    c(i, j) = a(i, j) + b(i, j);
                }
            }

            return c;

        }
    }

    Matrix operator*(const Matrix& a, const Matrix& b)
    {


        if (a.getCols() != b.getRows())
        {
            throw ("incomatable sizes");

        }
        else
        {
            Matrix c(a.getRows(), b.getCols());
            int i, j, k;
            double s = 0.0;
            for (k = 0; k < b.getCols(); k++)
            {
                for (i = 0; i < a.getRows(); i++)
                {
                    s = 0.0;
                    for (j = 0; j < a.getCols(); j++)
                    {
                        s += a(i, j) * b(j, k);
                    }

                    c(i, k) = s;
                }
            }
            return c;
        }
    }

    bool operator==(const Matrix& a, const Matrix& b)
    {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
        {

            return false;
        }
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                if (a(i, j) != b(i, j))
                    return false;
            }
        }
        return true;


    }
    bool operator!=(const Matrix& a, const Matrix& b)
    {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
        {

            return true;
        }
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                if (a(i, j) != b(i, j))
                    return true;
            }
        }
        return false;
    }
    ostream& operator<<(ostream& os, Matrix const& a) {

        int i, j;
        for (i = 0; i < a.getRows(); i++)
        {
            cout << "         ";
            for (j = 0; j < a.getCols(); j++)
                os << a(i, j) << "\t";
            os << "\n";
        }
        return os;
    }

    //use the linear algebra formula for rotation
    RotationMatrix::RotationMatrix(double theta) : Matrix(2,2) {
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);

        (*this)(0,0) = cosTheta;
        (*this)(0,1) = -sinTheta;
        (*this)(1,0) = sinTheta;
        (*this)(1,1) = cosTheta;
    }

    //use the linear algebra formula for scaling: 
	//expand or shrink by a factor 
    ScalingMatrix::ScalingMatrix(double scale) : Matrix(2,2) {
        (*this)(0,0) = scale;
        (*this)(1,1) = scale;
    }

    //shift each column value by a certain amount to translate
    TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols){
        for (int j = 0; j < nCols; j++)
        {
            (*this)(0, j) = xShift;
            (*this)(1, j) = yShift;
        }
    }

}


