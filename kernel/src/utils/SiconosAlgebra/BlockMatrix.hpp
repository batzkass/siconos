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

/*! \file BlockMatrix.hpp
  \brief Object to handle block-matrices.

*/

#ifndef __BlockMatrix__
#define __BlockMatrix__

#include "SiconosMatrix.hpp"

/** "Block" matrix, ie container of matrices 
 *
 * A BlockMatrix is a boost::ublas::compressed_matrix of SP::SiconosMatrix.
 *
 * The blocks positions are given by two Index objects, tabRow and tabCol.
 *
 * If block 1 is n1xm1, block2 n2xm2, block3 n3xm3 ..., then:\n
 *  tabRow = [ n1 n1+n2 n1+n2+n3 ...] \n
 *  tabCol = [ m1 m1+m2 m1+m2+m3 ...] \n
 *
 */
class BlockMatrix : public SiconosMatrix
{
private:

  ACCEPT_SERIALIZATION(BlockMatrix);

  /** A container of pointers to SiconosMatrix
   */
  SP::BlocksMat _mat;

  /** list of blocks dimension - tabRow[i] = tabRow[i-1] + ni, ni being the number of rows of block i.
   */
  SP::Index _tabRow;

  /** list of blocks dimension - tabCol[i] = tabCol[i-1] + ni, ni being the number of columns of block i.
   */
  SP::Index _tabCol;

  /** Number of rows (Warning: total number of scalar elements, not number of blocks) */
  unsigned int _dimRow;

  /** Number of columns (Warning: total number of scalar elements, not number of blocks) */
  unsigned int _dimCol;


  /** default constructor
   */
  BlockMatrix() {};

public:

  /** copy constructor
   *  \param m a SiconosMatrix
   */
  BlockMatrix(const SiconosMatrix& m);

  /** copy constructor
   *  \param m a BlockMatrix
   */
  BlockMatrix(const BlockMatrix& m);

  /** constructor with a list of pointer to SiconosMatrix (!links with pointer, no copy!)
   *  \param m a vector of SiconosMatrix
   *  \param row number of blocks in a row
   *  \param col number of col in a row
   */
  BlockMatrix(const std::vector<SP::SiconosMatrix>& m, unsigned int row, unsigned int col);

  /** contructor with a list of 4 pointer to SiconosMatrix (!links with pointer, no copy!)
   *  \param A block (0,0)
   *  \param B block (0,1)
   *  \param C block (1,0)
   *  \param D block (1,1)
   */
  BlockMatrix(SP::SiconosMatrix A, SP::SiconosMatrix B, SP::SiconosMatrix C, SP::SiconosMatrix D);

  /** destructor
   */
  ~BlockMatrix(void) noexcept;

  inline bool checkSymmetry(double tol) const override
  {
    return false;
  };

  /** get the number of block (i=0, row, i=1 col)
   *  \param i unsigned int(i=0, row, i=1 col)
   *  \return an unsigned int
   */
  unsigned int numberOfBlocks(unsigned int i) const override;

  /** get DenseMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return a DenseMat
   */
  const DenseMat getDense(unsigned int row = 0, unsigned int col = 0) const override;

  /** get TriangMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return a TriangMat
   */
  const TriangMat getTriang(unsigned int row = 0, unsigned int col = 0) const override;

  /** get SymMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return a SymMat
   */
  const SymMat getSym(unsigned int row = 0, unsigned int col = 0)const override;

  /** get BandedMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return a BandedMat
   */
  const BandedMat getBanded(unsigned int row = 0, unsigned int col = 0)const override;

  /** get SparseMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return a SparseMat
   */
  const SparseMat getSparse(unsigned int row = 0, unsigned int col = 0)const override;

  /** get SparseCoordinateMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return a SparseCoordinateMat
   */
  const SparseCoordinateMat getSparseCoordinate(unsigned int row = 0, unsigned int col = 0)const override;

  /** get ZeroMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return a ZeroMat
   */
  const ZeroMat getZero(unsigned int row = 0, unsigned int col = 0) const override;

  /** get  getIdentity matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return an IdentityMat
   */
  const IdentityMat getIdentity(unsigned int row = 0, unsigned int col = 0) const override;

  /** get a pointer on DenseMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return a DenseMat*
   */
  DenseMat* dense(unsigned int row = 0, unsigned int col = 0)const override;

  /** get a pointer on TriangMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return a TriangMat*
   */
  TriangMat* triang(unsigned int row = 0, unsigned int col = 0)const override;

  /** get a pointer on SymMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col `an unsigned int, position of the block (column)
   *  \return a SymMat*
   */
  SymMat* sym(unsigned int row = 0, unsigned int col = 0)const override;
 
  /** get a pointer on BandedMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return a BandedMat*
   */
  BandedMat* banded(unsigned int row = 0, unsigned int col = 0)const override;

  /** get a pointer on SparseMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return a SparseMat*
   */
  SparseMat* sparse(unsigned int row = 0, unsigned int col = 0)const override;

  /** get a pointer on SparseCoordinateMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return a SparseCoordinateMat*
   */
  SparseCoordinateMat* sparseCoordinate(unsigned int row = 0, unsigned int col = 0)const override;

  /** get a pointer on ZeroMat matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return a ZeroMat*
   */
  ZeroMat* zero_mat(unsigned int row = 0, unsigned int col = 0) const override;

  /** get a pointer on Identity matrix
   *  \param row an unsigned int, position of the block (row)
   *  \param col an unsigned int, position of the block (column)
   *  \return an IdentityMat*
   */
  IdentityMat* identity(unsigned int row = 0, unsigned int col = 0) const override;

  /** return the address of the array of double values of the matrix
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

  /** get the number of rows or columns of the matrix
   *  \param index 0 for rows, 1 for columns
   *  \return an int
   */
  unsigned int size(unsigned int index) const override;

  /** resize the matrix with nbrow rows and nbcol columns, lower and upper are useful only for SparseMat.The existing elements of the Block matrix are preseved when specified.
   * \param nbrow
   * \param nbcol
   * \param lower
   * \param upper
   * \param b
   */
  void resize(unsigned int nbrow, unsigned int nbcol, unsigned int lower = 0,
              unsigned int upper = 0, bool b = true) override;

  /** compute the infinite norm of the Block matrix
   *  \return a double
   */
  double normInf() const override;

  /** display data on standard output
   */
  void display() const override;
  
  /** display data on standard output
   */
  void displayExpert(bool brief = true) const override;

  /** put data of the matrix into a std::string
   * \return std::string
   */
  std::string toString() const override;

  /** send data of the matrix to an ostream
   * \param os An output stream
   * \param bm a BlockMatrix
   * \return The same output stream
   */
  friend std::ostream& operator<<(std::ostream& os, const BlockMatrix& bm) ;

  /** get or set the element matrix[i,j]
   *  \param i an unsigned int 
   *  \param j an unsigned int 
   *  \return the element matrix[i,j]
   */
  double& operator()(unsigned int i, unsigned int j) override;

  /** get or set the element matrix[i,j]
   *  \param i an unsigned int 
   *  \param j an unsigned int
   *  \return the element matrix[i,j]
   */
  double operator()(unsigned int i, unsigned int j) const override;

  /** return the element matrix[i,j]
   *  \param i an unsigned int 
   *  \param j an unsigned int 
   *  \return a double
   */
  double getValue(unsigned int i, unsigned int j) const override;

  /** set the element matrix[i,j]
   *  \param i an unsigned int i
   *  \param j an unsigned int j
   *  \param value
   */
  void setValue(unsigned int i, unsigned int j, double value) override;

  /** transpose in place: x->trans() is x = transpose of x.
   */
  void trans() override;

  /** transpose a matrix: x->trans(m) is x = transpose of m.
   *  \param m the matrix to be transposed.
   */
  void trans(const SiconosMatrix& m) override;

  /** get the vector tabRow
   *  \return a vector of int
   */
  inline Index getTabRow() const
  {
    return *_tabRow;
  };

  /** get the vector tabCol
   *  \return a vector of int
   */
  inline Index getTabCol() const
  {
    return *_tabCol;
  };

  /** get the vector tabRow
   *  \return a pointer to vector of int
   */
  inline const SP::Index tabRow() const override
  {
    return _tabRow;
  };

  /** get the vector tabCol
   *  \return a pointer to vector of int
   */
  inline const SP::Index tabCol() const override
  {
    return _tabCol;
  };

  /** get block at position row-col
   *  \param row unsigned int
   *  \param col unsigned int
   *  \return SP::SiconosMatrix the requested block
   */
  SP::SiconosMatrix block(unsigned int row = 0, unsigned int col = 0) override;

  /** get block at position row-col
   *  \param row unsigned int
   *  \param col unsigned int
   *  \return SP::SiconosMatrix the requested block
   */
  SPC::SiconosMatrix block(unsigned int row = 0, unsigned int col = 0) const override;

  /** get row index of current matrix and save it in  v
   *  \param r index of required line
   *  \param[out] v a vector
   */
  void  getRow(unsigned int r, SiconosVector& v) const override;

  /** set line row of the current matrix with vector v
   *  \param r index of required line
   *  \param v a vector
   */
  void  setRow(unsigned int r, const SiconosVector& v) override;

  /** get column index of current matrix and save it into vOut
   *  \param c index of required column
   *  \param[out] v a vector
   */
  void  getCol(unsigned int c, SiconosVector& v) const override;

  /** set column col of the current matrix with vector
   *  \param c index of required column
   *  \param v a vector
   */
  void  setCol(unsigned int c, const SiconosVector& v) override;

  /** add a part of the input matrix (starting from (i,j) pos) to the current matrix
   *  \param i an unsigned int i (in-out)
   *  \param j an unsigned int j (in-out)
   *  \param m a SiconosMatrix (in-out)
   */
  void addSimple(unsigned int& i, unsigned int& j, const SiconosMatrix& m);

  /** subtract a part of the input matrix (starting from (i,j) pos) to the current matrix
   *  \param i an unsigned int i (in-out)
   *  \param j an unsigned int j (in-out)
   *  \param m a SiconosMatrix (in-out)
   */
  void subSimple(unsigned int& i, unsigned int& j, const SiconosMatrix& m);

  /** assignment
   * \param m the matrix to be copied
   * \return  BlockMatrix&
   */
  BlockMatrix& operator = (const SiconosMatrix& m) override;

  /** assignment
   *  \param m the matrix to be copied
   * \return  BlockMatrix&
   */
  BlockMatrix& operator = (const BlockMatrix& m);

  /** assignment
   *  \param m the matrix to be copied
   * \return  BlockMatrix&
   */
  BlockMatrix& operator = (const DenseMat& m) override;

  /** operator +=
   *  \param m the matrix to add
   * \return  BlockMatrix&
   */
  BlockMatrix& operator +=(const SiconosMatrix& m) override;

  /**operator -=
   *  \param m the matrix to subtract
   * \return  BlockMatrix&
   */
  BlockMatrix& operator -=(const SiconosMatrix& m) override;


  void updateNumericsMatrix() override
  {
    THROW_EXCEPTION("BlockMatrix::updateNumericsMatrix(), not implemented fro BlockMatrix");
  };

  
  /** computes an LU factorization of a general M-by-N matrix using partial pivoting with row interchanges.
   *  The result is returned in this (InPlace). Based on Blas dgetrf function.
   */
  void PLUFactorizationInPlace() override;
  
  void Factorize() override;

 
  /**  compute inverse of this thanks to LU factorization with Partial pivoting. This method inverts U and then computes inv(A) by solving the system
   *  inv(A)*L = inv(U) for inv(A). The result is returned in this (InPlace). Based on Blas dgetri function.
   */
  void PLUInverseInPlace() override;

  /** solves a system of linear equations A * X = B  (A=this) with a general N-by-N matrix A using the LU factorization computed
   *   by PLUFactorizationInPlace. Based on Blas dgetrs function.
   *  \param[in,out] B on input the RHS matrix b; on output: the result x
   */
  void PLUForwardBackwardInPlace(SiconosMatrix &B) override;
  void Solve(SiconosMatrix &B) override;
  
  
  /** solves a system of linear equations A * X = B  (A=this) with a general N-by-N matrix A using the LU factorization computed
   *   by PLUFactorizationInPlace.  Based on Blas dgetrs function.
   *  \param[in,out] B on input the RHS matrix b; on output: the result x
   */
  void PLUForwardBackwardInPlace(SiconosVector &B) override;
  void Solve(SiconosVector &B) override;
  
  ACCEPT_STD_VISITORS();

  friend class SimpleMatrix;
  friend void scal(double, const SiconosMatrix&, SiconosMatrix&, bool);
  friend SiconosMatrix& operator *=(SiconosMatrix& m, const double& s);
  friend SiconosMatrix& operator /=(SiconosMatrix& m, const double& s);

  /** number of non-zero in the matrix
   * \param tol the tolerance under which a number is considered zero
   */
  virtual size_t nnz(double tol = 1.e-14) override;
};

//DEFINE_SPTR(BlockMatrix)

#endif
