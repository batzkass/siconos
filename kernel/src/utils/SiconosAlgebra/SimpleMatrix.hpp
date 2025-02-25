/* Siconos is a program dedicated to modeling, simulation and control
 * of non smooth dynamical systems.
 *
 * Copyright 2022 INRIA.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/*! \file SimpleMatrix.hpp
 */

#ifndef __SimpleMatrix__
#define __SimpleMatrix__

#include <iosfwd>                     // for ostream
#include "SiconosSerialization.hpp" // For ACCEPT_SERIALIZATION
#include "SiconosVisitor.hpp" // for ACCEPT_STD_VISITORS
#include "SiconosAlgebraTypeDef.hpp"  // for DENSE, DenseMat, Index, BandedMat
#include "SiconosFwd.hpp"             // for SiconosVector, SiconosMatrix
#include "SiconosMatrix.hpp"          // for SiconosMatrix, VInt, MATRIX_UBL...
#include "SiconosVector.hpp"          // for SiconosVector






class BlockVector;

/**
   Matrix (embedded various types of Boost matrices of double)
   
   SimpleMatrix is used in the platform to store matrices (mathematical object) of double.
   
   Possible types: Siconos::DENSE (default),
   TRIANGULAR, SYMMETRIC, SPARSE, BANDED, ZERO,
   Siconos::IDENTITY,
   Siconos::SPARSE_COORDINATE.
   
   \todo: review resize function for Banded, Symetric and Triangular. Error in tests.
   
*/
class SimpleMatrix: public SiconosMatrix
{
protected:
  ACCEPT_SERIALIZATION(SimpleMatrix);

  /** Union of The Boost Matrices : DenseMat, TriangMat, SymMat ...
      (See SiconosMatrix.h for more details on MATRIX_UBLAS_TYPE);
  */
  MATRIX_UBLAS_TYPE mat;


private:
  /** VInt _ipiv;
   * The pivot indices obtained from DGETRF (PLUFactorizationInPlace)
   */
  SP::VInt _ipiv;

  /** True if the Matrix is PLU Factorized. */
  bool _isPLUFactorized = false;

  /** True if the Matrix is PLU Factorized in place.*/
  bool _isPLUFactorizedInPlace = false;

  /** True if the Matrix is Cholesky Factorized. */
  bool _isCholeskyFactorized = false;

  /** True if the Matrix is Cholesky Factorized in place. */
  bool _isCholeskyFactorizedInPlace = false;

  /** True if the Matrix has been QR Factorized in place. */
  bool _isQRFactorized = false;

  /** True if the Matrix has been Inversed in Place. */
  bool _isPLUInversed = false;

  /** Numerics Matrix structure
   *  This matrix is used to perform  computation using Numerics,
   *  for instance, the LU factorization of a sparse matrix.
   *  It may contains copy or pointer on the SimpleMatrix.
   */
  SP::NumericsMatrix _numericsMatrix;


  /* computes res = subA*x +res, subA being a submatrix of A (rows from startRow to startRow+sizeY and columns between startCol and startCol+sizeX).
   *  If x is a block vector, it call the present function for all blocks.
   *
   *  \param A a pointer to SiconosMatrix
   *  \param startRow an int, sub-block position
   *  \param startCol an int, sub-block position
   *  \param x a pointer to a SiconosVector
   *  \param res a DenseVect
   */
  // friend void private_addprod(const SiconosMatrix& A, unsigned int startRow, unsigned int startCol, const SiconosVector& x, SiconosVector& res);


  /* computes res = subA*x +res, subA being a submatrix of trans(A) (rows from startRow to startRow+sizeY and columns between startCol and startCol+sizeX).
   *  If x is a block vector, it call the present function for all blocks.
   * \param x a pointer to a SiconosVector
   * \param A a pointer to SiconosMatrix
   * \param startRow an int, sub-block position
   * \param startCol an int, sub-block position
   * \param res a DenseVect, res.
   */
  // friend void private_addprod(SPC::SiconosVector x , SPC::SiconosMatrix A,
  //                             unsigned int startRow, unsigned int startCol,
  //                             SP::SiconosVector res);

  /** computes y = subA*x (init =true) or += subA * x (init = false), subA being a submatrix of A (all columns, and rows between start and start+sizeY).
   *  If x is a block vector, it call the present function for all blocks.
   *
   *  \param A a pointer to SiconosMatrix
   *  \param startRow an int, sub-block position
   *  \param x a pointer to a SiconosVector
   *  \param y a pointer to a SiconosVector
   *  \param init a bool
   */
  void private_prod(unsigned int startRow, const SiconosVector& x, SiconosVector& y, bool init);


  /** computes res = subA*x +res, subA being a submatrix of A (rows from startRow to startRow+sizeY and columns between startCol and startCol+sizeX).
   *  If x is a block vector, it call the present function for all blocks.
   *
   *  \param A a pointer to SiconosMatrix
   *  \param startRow an int, sub-block position
   *  \param startCol an int, sub-block position
   *  \param x a pointer to a SiconosVector
   *  \param res a DenseVect
   */
  void private_addprod(unsigned int startRow, unsigned int startCol, const SiconosVector& x, SiconosVector& res);

   /* computes res = a*subA*x +res, subA being a submatrix of A (rows from startRow to startRow+sizeY and columns between startCol and startCol+sizeX).
    *   If x is a block vector, it call the present function for all blocks.
    *   \param a a double
    *   \param A a pointer to SiconosMatrix
    *   \param startRow an int, sub-block position
    *   \param startCol an int, sub-block position
    *   \param x a pointer to a SiconosVector
    *   \param res a DenseVect
    */
  // friend void private_addprod(double a, SPC::SiconosMatrix A,
  //                             unsigned int startRow, unsigned int startCol,
  //                             SPC::SiconosVector x, SP::SiconosVector res);


  /* computes y = a*subA*x (init =true) or += a*subA * x (init = false), subA being a submatrix of A (all columns, and rows between start and start+sizeY).
   *   If x is a block vector, it call the present function for all blocks.
   *    \param a a double
   *    \param A a pointer to SiconosMatrix
   *    \param start an int, sub-block position
   *    \param x a pointer to a SiconosVector
   *    \param y a pointer to a SiconosVector
   *    \param init, a bool
   */
  // friend void private_prod(double a, SPC::SiconosMatrix A, unsigned int start,
  //                          SPC::SiconosVector x, SP::SiconosVector y, bool init);

  /*  computes y = subA*x (init =true) or += subA * x (init = false), subA being a submatrix of trans(A) (all columns, and rows between start and start+sizeY).
   *    If x is a block vector, it call the present function for all blocks.
   *    \param x a pointer to a SiconosVector
   *    \param A a pointer to SiconosMatrix
   *    \param start an int, sub-block position
   *    \param y a pointer to a SiconosVector
   *    \param init a bool
  */
  // friend void private_prod(SPC::SiconosVector x, SPC::SiconosMatrix A, unsigned int start, SP::SiconosVector y, bool init);
  // friend void private_prod(SPC::BlockVector, SPC::SiconosMatrix, unsigned int, SP::SiconosVector, bool);
  // friend void private_prod(SPC::BlockVector, SPC::SiconosMatrix, unsigned int, SP::BlockVector, bool);
  // friend void private_prod(SPC::SiconosVector, SPC::SiconosMatrix, unsigned int, SP::BlockVector, bool);

public:
  /** Default constructor */
  SimpleMatrix();

  /** constructor with the type and the dimension of the Boost matrix
   *
   *  \param row number of rows.
   *  \param col number of columns.
   *  \param typ the type of matrix
   *  \param upper if Siconos::UBLAS_TYPE==SPARSE, number of non-zero terms, if Siconos::UBLAS_TYPE == BANDED, number of diags. under the main diagonal
   *  \param lower if Siconos::UBLAS_TYPE == BANDED, number of diags. over the main diagonal
   */
  SimpleMatrix(unsigned int row, unsigned int col, Siconos::UBLAS_TYPE typ = Siconos::DENSE, unsigned int upper = 1, unsigned int lower = 1);

  /** constructor with the the dimensions of the Boost matrix, a default value and the type.
   *
   *  \param row number of rows.
   *  \param col number of columns.
   *  \param inputValue double a, so that *this = [a a a ...]
   *  \param typ the type of matrix
   *  \param upper if Siconos::UBLAS_TYPE==SPARSE, number of non-zero terms, if Siconos::UBLAS_TYPE == BANDED, number of diags. under the main diagonal
   *  \param lower if Siconos::UBLAS_TYPE == BANDED, number of diags. over the main diagonal
   */
  SimpleMatrix(unsigned int row, unsigned int col, double inputValue,
               Siconos::UBLAS_TYPE typ = Siconos::DENSE,
               unsigned int upper = 1, unsigned int lower = 1);

  /** copy constructor
   *
   *  \param smat the matrix to copy
   */
  SimpleMatrix(const SimpleMatrix& smat);

  /** copy constructor of a block given by the coord = [r0A r1A c0A c1A]
   *
   *  \param A the matrix which contains the block to extract
   *  \param coord positions of the block to be extracted (row:start, row:end, col:start, col:end)
   */
  SimpleMatrix(const SimpleMatrix& A , const Index& coord );

  /** copy constructor
   *
   *  \param m the matrix to copy
   */
  SimpleMatrix(const SiconosMatrix& m);

  /** constructor with a DenseMat matrix (see SiconosMatrix.h for details)
   *
   *  \param m a DenseMat
   */
  SimpleMatrix(const DenseMat& m);

  /** constructor with a TriangMat matrix (see SiconosMatrix.h for details)
   *
   *  \param m a TriangMat
   */
  SimpleMatrix(const TriangMat& m);

  /** constructor with a SymMat matrix (see SiconosMatrix.h for details)
   *
   *  \param m a SymMat
   */
  SimpleMatrix(const SymMat& m);

  /** constructor with a BandedMat matrix (see SiconosMatrix.h for details)
   *
   *  \param m a BandedMat
   */
  SimpleMatrix(const BandedMat& m);

  /** constructor with a SparseMat matrix (see SiconosMatrix.h for details)
   *
   *  \param m a SparseMat
   */
  SimpleMatrix(const SparseMat& m);

  /** constructor with a SparseCoordinateMat matrix (see SiconosMatrix.h for details)
   *
   *  \param m a SparseMat
   */
  SimpleMatrix(const SparseCoordinateMat& m);

  /** constructor with a ZeroMat matrix (see SiconosMatrix.h for details)
   *
   *  \param m a ZeroMat
   */
  SimpleMatrix(const ZeroMat& m);

  /** constructor with a IdentityMat matrix (see SiconosMatrix.h for details)
   *
   *  \param m a IdentityMat
   */
  SimpleMatrix(const IdentityMat& m);

  /** constructor with an input file
   *
   *  \param file the input file path
   *  \param ascii a boolean to indicate if the file is in ascii
   */
  SimpleMatrix(const std::string& file, bool ascii = true);

  /** destructor
   */
  ~SimpleMatrix();
  //************************** GETTERS/SETTERS  **************************

  void updateNumericsMatrix() override;

  NumericsMatrix * numericsMatrix() const override
  {
    return _numericsMatrix.get();
  };

  /** determines if the matrix has been inversed
   *
   *  \return true if the matrix is inversed
   */
  inline bool isPLUInversed() const override
  {
    return _isPLUInversed;
  }

  /** determines if the matrix has been factorized
   *
   *  \return true if the matrix is factorized
   */
  inline bool isPLUFactorized() const override
  {
    return _isPLUFactorized;
  }

 /** determines if the matrix has been factorized
   *
   *  \return true if the matrix is factorized
   */
  inline bool isPLUFactorizedInPlace() const override
  {
    return _isPLUFactorizedInPlace;
  }
  /** determines if the matrix has been factorized
   *
   *  \return true if the matrix is factorized
   */
  inline bool isCholeskyFactorized() const override
  {
    return _isCholeskyFactorized;
  }

  /** determines if the matrix has been factorized
   *
   *  \return true if the matrix is factorized
   */
  inline bool isCholeskyFactorizedInPlace() const
  {
    return _isCholeskyFactorizedInPlace;
  }

  /** determines if the matrix has been factorized
   *
   *  \return true if the matrix is factorized
   */
  inline bool isQRFactorized() const
  {
    return _isQRFactorized;
  }


  inline SP::VInt ipiv() const override
  {
    return _ipiv;
  }


  bool checkSymmetry(double tol) const override;

  /** get DenseMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a DenseMat
   */
  const DenseMat getDense(unsigned int row = 0, unsigned int col = 0) const override;

  /** get TriangMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a TriangMat
   */
  const TriangMat getTriang(unsigned int row = 0, unsigned int col = 0) const override;

  /** get SymMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a SymMat
   */
  const SymMat getSym(unsigned int row = 0, unsigned int col = 0) const override;

  /** get BandedMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a BandedMat
   */
  const BandedMat getBanded(unsigned int row = 0, unsigned int col = 0) const override;

  /** get SparseMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a SparseMat
   */
  const SparseMat getSparse(unsigned int row = 0, unsigned int col = 0) const override;

  /** get SparseCoordinateMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a SparseCoordinateMat
   */
  const SparseCoordinateMat getSparseCoordinate(unsigned int row = 0, unsigned int col = 0) const override;

  /** get ZeroMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a ZeroMat
   */
  const ZeroMat getZero(unsigned int row = 0, unsigned int col = 0) const override;

  /** get  getIdentity matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return an IdentityMat
   */
  const IdentityMat getIdentity(unsigned int row = 0, unsigned int col = 0) const override;

  /** get a pointer on DenseMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a DenseMat*
   */
  DenseMat* dense(unsigned int row = 0, unsigned int col = 0) const override;

  /** get a pointer on TriangMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a TriangMat*
   */
  TriangMat* triang(unsigned int row = 0, unsigned int col = 0) const override;

  /** get a pointer on SymMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a SymMat*
   */
  SymMat* sym(unsigned int row = 0, unsigned int col = 0) const override;

  /** get a pointer on BandedMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a BandedMat*
   */
  BandedMat* banded(unsigned int row = 0, unsigned int col = 0) const override;

  /** get a pointer on SparseMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a SparseMat*
   */
  SparseMat* sparse(unsigned int row = 0, unsigned int col = 0) const override;

  /** get a pointer on SparseCoordinateMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a SparseCoordinateMat*
   */
  SparseCoordinateMat* sparseCoordinate(unsigned int row = 0, unsigned int col = 0) const override;

  /** get a pointer on ZeroMat matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return a ZeroMat*
   */
  ZeroMat* zero_mat(unsigned int row = 0, unsigned int col = 0) const override;

  /** get a pointer on Identity matrix
   *
   *  \param row an unsigned int, position of the block - Useless for SimpleMatrix
   *  \param col an unsigned int, position of the block - Useless for SimpleMatrix
   *  \return an IdentityMat*
   */
  IdentityMat* identity(unsigned int row = 0, unsigned int col = 0) const override;

  /** return the address of the array of double values of the matrix
   *
   *  \param row position for the required block ->useless for SimpleMatrix
   *  \param col position for the required block ->useless for SimpleMatrix
   *  \return double* : the pointer on the double array
   */
  double* getArray(unsigned int row = 0, unsigned int col = 0) const override;

  /** sets all the values of the matrix to 0.0
   */
  void zero() override;

  /** Initialize the matrix with random values
   */
  void randomize() override;

  /** Initialize a symmetric matrix with random values
   */
  void randomize_sym() override;

  /** set an identity matrix
   */
  void eye() override;

  /** copy the matrix data to the array given in parameter'
   *  Works only for dense matrices !
   *
   *  \param data array where the matrix is copied
   *  \return the size of the matrix
   */
  unsigned copyData(double* data) const;

  void assign(const SimpleMatrix &smat);


  /** get the number of rows or columns of the matrix
   *
   *  \param index 0 for rows, 1 for columns
   *  \return the size
   */
  unsigned int size(unsigned int index) const override;

  /** resize the matrix with nbrow rows and nbcol columns The existing elements of the matrix are preseved when specified.
   *
   *  \param row the new number of rows
   *  \param col the mew number of columns
   *  \param lower (only for Banded)
   *  \param upper (only for Banded)
   *  \param preserve preserve existing elements
   */
  void resize(unsigned int row, unsigned int col, unsigned int lower = 0, unsigned int upper = 0, bool preserve = true) override;

  /** compute the infinite norm of the matrix
   *
   *  \return a double
   */
  double normInf() const override;

  /** Compute the normInf for each column
   *
   *  \param vIn column
   */
  void normInfByColumn(SP::SiconosVector vIn) const;

  /** compute the determinant of the matrix (use LU factorization)
   *
   *  \return a double
   */
  double det() const;

  /** display data on standard output
   */
  void display() const override;

  void displayExpert(bool  brief = true) const override;
  /** put data of the matrix into a std::string
   *
   *  \return std::string
   */
  std::string toString() const override;

  /** get or set the element matrix[i,j]
   *
   *  \param i an unsigned int
   *  \param j an unsigned int
   *  \return the element matrix[i,j]
   */
  double& operator()(unsigned int i, unsigned int j) override;

  /** get or set the element matrix[i,j]
   *
   *  \param i an unsigned int
   *  \param j an unsigned int
   *  \return the element matrix[i,j]
   */
  double operator()(unsigned int i, unsigned int j) const override;

  /** return the element matrix[i,j]
   *
   *  \param i an unsigned int
   *  \param j an unsigned int
   *  \return a double
   */
  double getValue(unsigned int i, unsigned int j) const override;

  /** set the element matrix[i,j]
   *
   *  \param i an unsigned int
   *  \param j an unsigned int
   *  \param value
   */
  void setValue(unsigned int i, unsigned int j, double value) override;

  /** Copy of the content of a given matrix into the current object,
      at position (posRow, posCol).

      Defined in SimpleMatrixSetGet.cpp.

      \param posRow row-index of the targeted block
      \param posCol col-index of the targeted block
      \param m source matrix to be copied. Can be a SimpleMatrix or a BlockMatrix.
  */
  void setBlock(unsigned int posRow, unsigned int posCol, const SiconosMatrix& m);

  // friend void setBlock(SPC::SiconosMatrix , SP::SiconosMatrix , const Index&, const Index&);

  // /** get block at position row-col, (current matrix in SimpleMatrix case)
  //  * \param row row index
  //  * \param col column index
  //  * \return a sub-matrix
  //  */
  // inline SP::SiconosMatrix block(unsigned int row = 0, unsigned int col = 0)
  // {
  //   return shared_from_this();
  // };

  // /** get block at position row-col, (current matrix in SimpleMatrix case)
  //  * \param row row index
  //  * \param col column index
  //  * \return a sub-matrix
  //  */
  // inline SPC::SiconosMatrix block(unsigned int row = 0, unsigned int col = 0) const
  // {
  //   return shared_from_this();
  // };

  /** get row index of current matrix and save it into vOut
   *
   *  \param row index row we want to get
   *  \param[out] vOut SiconosVector that will contain the desired row
   */
  void getRow(unsigned int row, SiconosVector& vOut) const override;

  /** get column index of current matrix and save it into vOut
   *
   *  \param col index column we want to get
   *  \param[out] vOut SiconosVector that will contain the desired column
   */
  void getCol(unsigned int col, SiconosVector& vOut) const override;

  /** set line row of the current matrix with vector v
   *
   *  \param row index row we want to set
   *  \param vIn SiconosVector containing the new row
   */
  void setRow(unsigned int row , const SiconosVector& vIn) override;

  /** set column col of the current matrix with vector v
   *
   *  \param col index column we want to set
   *  \param vIn a SiconosVector containing the new column
   */
  void setCol(unsigned int col, const SiconosVector& vIn) override;

  /** get column number index of current matrix, starting from element at position pos and save it into vOut
   *
   *  \param index index of required column
   *  \param pos index of the first required element in the column
   *  \param[out] vOut a SP::SiconosVector
   */
  void getSubCol(unsigned int index, unsigned int pos, SP::SiconosVector vOut) const;

  /** get row number index of current matrix, starting from element at position pos and save it into vOut
   *
   *  \param index index of the required row
   *  \param pos index of the first required element in the row
   *  \param[out] vOut a SP::SiconosVector that will contain the sub row
   */
  void getSubRow(unsigned int index, unsigned int pos, SP::SiconosVector vOut) const;

  /** set column number index of current matrix, starting from element at position pos, with vIn
   *
   *  \param index index of required column
   *  \param pos index of the first required element in the column
   *  \param vIn a vector
   */
  void setSubCol(unsigned int index, unsigned int pos, SP::SiconosVector vIn);

  /** set row number index of current matrix, starting from element at position pos, with vIn
   *
   *  \param index index of required row
   *  \param pos index of the first required element in the row
   *  \param vIn a vector
   */
  void setSubRow(unsigned int index, unsigned int pos, SP::SiconosVector vIn);

  /** add the input matrix to the elements starting from position i (row) and j (col).
   *
   *  \param i an unsigned int
   *  \param j an unsigned int
   *  \param m a SiconosMatrix
   */
  void addBlock(unsigned int i, unsigned int j, const SiconosMatrix& m);

  /** subtract the input matrix to the elements starting from position i (row) and j (col).
   *
   *  \param i an unsigned int
   *  \param j an unsigned int
   *  \param m a SiconosMatrix
   */
  void subBlock(unsigned int i, unsigned int j, const SiconosMatrix& m);

  /** transpose in place: x->trans() is x = transpose of x.
   */
  void trans() override;

  /** transpose a matrix: x->trans(m) is x = transpose of m.
   *
   *  \param mat the matrix to be transposed.
   */
  void trans(const SiconosMatrix& mat) override;

  /** assignment
   *
   *  \param m the matrix to be copied
   *  \return SimpleMatrix&
   */
  SimpleMatrix& operator = (const SiconosMatrix& m) override;

  /** assignment
   *
   *  \param m the matrix to be copied
   *  \return SimpleMatrix&
   */
  SimpleMatrix& operator = (const SimpleMatrix& m);

  /** assignment to a DenseMat
   *
   *  \param m the matrix to be copied
   *  \return SimpleMatrix&
   */
  SimpleMatrix& operator = (const DenseMat& m) override;

  /** operator +=
   *
   *  \param m a matrix to add
   *  \return SimpleMatrix&
   */
  SimpleMatrix& operator +=(const SiconosMatrix& m) override;

  /** operator -=
   *
   *  \param m a matrix to subtract
   *  \return SimpleMatrix&
   */
  SimpleMatrix& operator -=(const SiconosMatrix& m) override;

  /** computes an LU factorization of a general M-by-N matrix using partial pivoting with row interchanges.
   *  The result is returned in this (InPlace). Based on Blas dgetrf function.
   */
  void PLUFactorizationInPlace() override;

  /** computes a factorization of a general M-by-N matrix
   */
  void Factorize() override;

  /** compute inverse of this thanks to LU factorization with Partial pivoting.
   *  This method inverts U and then computes inv(A) by solving the system
   *  inv(A)*L = inv(U) for inv(A). The result is returned in this (InPlace). Based on Blas dgetri function.
   */
  void PLUInverseInPlace() override;

  /** solves a system of linear equations A * X = B  (A=this) with a general N-by-N matrix A using the LU factorization computed
   *  by PLUFactorizationInPlace. Based on Blas dgetrs function.
   *
   *  \param[in,out] B on input the RHS matrix b; on output the result x
   */
  void PLUForwardBackwardInPlace(SiconosMatrix& B) override;
  void Solve(SiconosMatrix& B) override;

  /** solves a system of linear equations A * X = B  (A=this) with a general N-by-N matrix A using the LU factorization computed
   *  by PLUFactorizationInPlace.  Based on Blas dgetrs function.
   *
   *  \param[in,out] B on input the RHS matrix b; on output the result x
   */
  void PLUForwardBackwardInPlace(SiconosVector& B) override;
  void Solve(SiconosVector& B) override;

  /** solves a system of linear equations A * X = B  (A=this)
   *  with a general N-by-N matrix A using the Least squares method
   *
   *  \param[in,out] B on input the RHS matrix b; on output the result x
   */
  void SolveByLeastSquares(SiconosMatrix& B);

  /** solves a system of linear equations A * X = B  (A=this)
   *  with a general N-by-N matrix A using the Least squares method
   *
   *  \param[in,out] B on input the RHS matrix b; on output the result x
   */
  void SolveByLeastSquares(SiconosVector& B);

  /** set to false all LU indicators. Useful in case of
      assignment for example.
  */

  void resetLU() override;

  /** set to false all Cholesky indicators. Useful in case of
      assignment for example.
  */

  void resetCholesky();

  /** set to false all QR indicators. Useful in case of
      assignment for example.
  */
  void resetQR();

  /** set to false all factorization indicators. Useful in case of
      assignment for example.
  */
  void resetFactorizationFlags() override;



  /** Visitors hook
   */
  ACCEPT_STD_VISITORS();
  /* List of friend functions of the SimpleMatrix class
     
     Declared in SimpleMatrixFriends.hpp.
     Implemented in SimpleMatrixFriends.cpp.
  */
  friend std::ostream& operator<<(std::ostream& os, const SimpleMatrix& sm);

  friend const SimpleMatrix operator * (const SiconosMatrix&, double);

  friend  SP::SimpleMatrix operator * (const SP::SimpleMatrix, const SP::SimpleMatrix);

  friend  void operator +=(SP::SiconosMatrix, SP::SimpleMatrix);

  friend  SimpleMatrix operator * (double , const SiconosMatrix&);

  friend const SimpleMatrix operator /(const SiconosMatrix&, double);

  friend const SimpleMatrix operator +(const SiconosMatrix&, const SiconosMatrix&);

  friend SP::SimpleMatrix operator +(const SP::SimpleMatrix, const SP::SimpleMatrix);

  friend void add(const SiconosMatrix&, const SiconosMatrix&, SiconosMatrix&);

  friend const SimpleMatrix operator -(const SiconosMatrix&, const SiconosMatrix&);

  friend void sub(const SiconosMatrix&, const SiconosMatrix&, SiconosMatrix&);

  friend bool operator == (const SiconosMatrix&, const SiconosMatrix&);

  friend bool operator!= (const SiconosMatrix&, const SiconosMatrix&);

  // friend const SimpleMatrix prod(const SiconosMatrix&, const SiconosMatrix&);

  // friend void prod(const SiconosMatrix&, const SiconosMatrix&, SiconosMatrix&, bool);

  // friend void axpy_prod(const SiconosMatrix&, const SiconosMatrix&, SiconosMatrix&, bool);

  // friend const SiconosVector prod(const SiconosMatrix&, const SiconosVector&);

  // friend void prod(const SiconosMatrix&, const BlockVector&, SiconosVector&, bool);

  // friend void prod(const SiconosMatrix&, const SiconosVector&, BlockVector&, bool);

  // friend void prod(double, const SiconosMatrix&, const SiconosVector&, SiconosVector&, bool);

  // friend void subprod(const SiconosMatrix&, const SiconosVector&, SiconosVector&, const Index&, bool);

  // friend void axpy_prod(const SiconosMatrix&, const SiconosVector&, SiconosVector&, bool);

  // friend void gemvtranspose(double, const SiconosMatrix&, const SiconosVector&, double, SiconosVector&);

  // friend void gemv(double, const SiconosMatrix&, const SiconosVector&, double, SiconosVector&);

  // friend void gemmtranspose(double, const SiconosMatrix&, const SiconosMatrix&, double, SiconosMatrix&);

  // friend void gemm(double, const SiconosMatrix&, const SiconosMatrix&, double, SiconosMatrix&);

  // friend void scal(double, const SiconosMatrix&, SiconosMatrix&, bool);

};



#endif
