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

#ifndef SparseBlockMatrix_H
#define SparseBlockMatrix_H

#include <stdio.h>          // for size_t, FILE
#include "CSparseMatrix.h"  // for CSparseMatrix
#include "NumericsFwd.h"    // for SparseBlockStructuredMatrix, SparseBlockC...

#include "SiconosConfig.h" // for BUILD_AS_CPP // IWYU pragma: keep
#include "NumericsDataVersion.h" // versioning

/*!\file SparseBlockMatrix.h
  Structure definition and functions related to
  SparseBlockStructuredMatrix

*/

/**

   Structure to store sparse block matrices with square diagonal blocks.

   \param nbblocks the total number of non null blocks
   \param **block : *block contains the double values of one block in
   Fortran storage (column by column) **block is
   the list of non null blocks
   \param blocknumber0 the first dimension of the block matrix
   (number of block rows)
   \param blocknumber1 the second dimension of the block matrix
   (number of block columns)
   \param *blocksize0 the list of sums of the number of rows of the
   first column of blocks of M: blocksize0[i] = blocksize0[i-1] +
   ni, ni being the number of rows of the block at row i
   \param *blocksize1 the list of sums of the number of columns of the
   first row of blocks of M: blocksize1[i] = blocksize1[i-1] + ni,
   ni being the number of columns of the block at column i
   \param filled1 index of the last non empty line + 1
   \param filled2 number of non null blocks
   \param index1_data index1_data is of size equal to number of non
   empty lines + 1. A block with number blockNumber inside a row
   numbered rowNumber verify index1_data[rowNumber]<= blockNumber
   <index1_data[rowNumber+1]`
 
   \param index2_data index2_data is of size filled2
   index2_data[blockNumber] -> columnNumber.

   
   Related functions: SBM_gemv(), SBM_row_prod(), SBM_clear(),
   SBM_print, SBM_diagonal_block_index()
   
   Note: the sparse format is the same as the one used by Boost C++
   library to store compressed sparse row matrices. The same member
   names have been adopted in order to simplify usage from Siconos
   Kernel : filled1, filled2, index1_data, index2_data.
   Reference :
   http://ublas.sourceforge.net/refdoc/classboost_1_1numeric_1_1ublas_1_1compressed__matrix.html
   
   
   If we consider the matrix M and the right-hand-side q defined as
   
   \f[
   M=\left[\begin{array}{cccc|cc|cc}
   1 & 2 & 0 & 4   & 3 &-1   & 0 & 0  \\
   2 & 1 & 0 & 0   & 4 & 1   & 0 & 0  \\
   0 & 0 & 1 &-1   & 0 & 0   & 0 & 0  \\
   5 & 0 &-1 & 6   & 0 & 6   & 0 & 0  \\
   \hline
   0 & 0 & 0 & 0   & 1 & 0   & 0 & 5 \\
   0 & 0 & 0 & 0   & 0 & 2   & 0 & 2 \\
   \hline
   0 & 0 & 2 & 1   & 0 & 0   & 2 & 2 \\
   0 & 0 & 2 & 2   & 0 & 0   & -1 & 2 \\
   \end{array}\right] \quad, q=\left[\begin{array}{c}-1 \\ -1 \\ 0 \\ -1 \\ \hline 1 \\ 0 \\ \hline -1 \\ 2 \end{array}\right].
   \f]
   
   then
   
   - the number of non null blocks is 6 (nbblocks=6)
   - the number of rows of blocks is 3 (blocknumber0 =3) and the
   number of columns of blocks is 3 (blocknumber1 =3)
   - the vector blocksize0 is equal to {4,6,8} and the vector
   blocksize1 is equal to {4,6,8}
   - the integer filled1 is equal to 4
   - the integer filled2 is equal to 6
   - the vector index1_data is equal to {0,2,4,6}
   - the vector index2_data is equal to {0,1,1,2,0,2}
   - the block contains all non null block matrices stored in Fortran
   order (column by column) as
   block[0] = {1,2,0,5,2,1,0,0,0,0,1,-1,4,0,-1,6}
   block[1] = {3,4,0,0,-1,1,0,6}
   ...
   block[5] = {2,-1,2,2}
*/


struct SparseBlockStructuredMatrix
{
  /* the number of non null blocks */
  unsigned int nbblocks;
  double **block;
  /* the number of rows of blocks */
  unsigned int blocknumber0;
  /* the number of columns of blocks */
  unsigned int blocknumber1;
  /* the vector of cumulated row sizes of blocks */
  unsigned int *blocksize0;
  /* the vector of cumulated column sizes of blocks */
  unsigned int *blocksize1;

  /* the index of the last non empty line + 1 */
  size_t filled1;
  /* the size of index2_data that corresponds to the number of non null blocks*/
  size_t filled2;

  size_t *index1_data;
  size_t *index2_data;

  /* the indices of the diagonal blocks */
  unsigned int * diagonal_blocks;

  NumericsDataVersion version; /**< version of storage */
};

struct SparseBlockCoordinateMatrix
{
  /** number of blocks */
  unsigned int nbblocks;

  /** number of rows */
  unsigned int blocknumber0;

  /** number of columns */
  unsigned int blocknumber1;

  /** block pointers */
  double **block;

  /** cumulative number of rows in blocks */
  unsigned int *blocksize0;

  /** cumulative number of columns in blocks */

  unsigned int *blocksize1;

  /** row indices */
  unsigned int *row;

  /** column indices */
  unsigned int *column;
};

struct SparseBlockStructuredMatrixPred
{
  int nbbldiag;
  int **indic;
  int **indicop;
  double **submatlcp;
  double **submatlcpop;
  int **ipiv;
  int *sizesublcp;
  int *sizesublcpop;
  double **subq;
  double **bufz;
  double **newz;
  double **workspace;
};

struct SBM_index_by_column
{
  /* the index of the last non empty line + 1 */
  size_t filled3;
  /* the size of index2_data that corresponds of the number of non null blocks*/
  size_t filled4;

  size_t * index3_data;
  size_t * index4_data;
  size_t * blockMap;
};



#define NUMERICS_SBM_FREE_BLOCK 4
#define NUMERICS_SBM_FREE_SBM 8

#if defined(__cplusplus) && !defined(BUILD_AS_CPP)
extern "C"
{
#endif

  /** Creation of an empty Sparse Block Matrix.
   *
   *  \return a pointer on allocated and initialized space
   */
  SparseBlockStructuredMatrix* SBM_new(void);

  /** set Sparse Block Matrix. fields to NULL
   *
   *  \param sbm a matrix
   */
  void SBM_null(SparseBlockStructuredMatrix* sbm);

  /**
     SparseMatrix - vector product y = alpha*A*x + beta*y

     \param[in] sizeX dim of the vectors x
     \param[in] sizeY dim of the vectors y
     \param[in] alpha coefficient
     \param[in] A the matrix to be multiplied
     \param[in] x the vector to be multiplied
     \param[in] beta coefficient
     \param[in,out] y the resulting vector
  */
  void SBM_gemv(unsigned int sizeX, unsigned int sizeY,
               double alpha, const SparseBlockStructuredMatrix* const A,
               const double* x, double beta, double* y);

  /**
     SparseMatrix - vector product y = A*x + y for block of size 3x3

     \param[in] sizeX dim of the vectors x
     \param[in] sizeY dim of the vectors y
     \param[in] A the matrix to be multiplied
     \param[in] x the vector to be multiplied
     \param[in,out] y the resulting vector
  */
  void SBM_gemv_3x3(unsigned int sizeX, unsigned int sizeY,
                  const SparseBlockStructuredMatrix* const A,
                  double* const x, double* y);

  /**
     SparseBlockStructuredMatrix - SparseBlockStructuredMatrix product C = alpha*A*B + beta*C
     The routine has to be used with precaution. The allocation of C is not done
     since we want to add beta*C. We assume that the structure and the allocation
     of the matrix C are right. Especially:
     
     - the blocks C(i,j) must exists
     - the sizes of blocks must be consistent
     - no extra block must be present in C
     
     \param[in] alpha coefficient
     \param[in] A the matrix to be multiplied
     \param[in] B the matrix to be multiplied
     \param[in] beta coefficient
     \param[in,out] C the resulting matrix
   */
  void SBM_gemm_without_allocation(double alpha, const SparseBlockStructuredMatrix* const A,
                                  const SparseBlockStructuredMatrix* const B,
                                  double beta, SparseBlockStructuredMatrix*  C);
  
  /**
     SparseBlockStructuredMatrix - SparseBlockStructuredMatrix multiplication C = A *B
     Correct allocation is performed
     
     \param[in] A the matrix to be multiplied
     \param[in] B the matrix to be multiplied
     \return C the resulting matrix
  */
  SparseBlockStructuredMatrix*  SBM_multiply(const SparseBlockStructuredMatrix* const A, const SparseBlockStructuredMatrix* const B);

  /** Perform the allocation of a zero matrix that is compatible qith multiplication
   */
  SparseBlockStructuredMatrix*  SBM_zero_matrix_for_multiply(const SparseBlockStructuredMatrix* const A, const SparseBlockStructuredMatrix* const B);

  /**
     SparseBlockStructuredMatrix - SparseBlockStructuredMatrix addition C = alpha*A + beta*B
     
     \param[in] A the matrix to be added
     \param[in] B the matrix to be added
     \param[in] alpha coefficient
     \param[in] beta coefficient
     \return C the resulting matrix
  */
  SparseBlockStructuredMatrix * SBM_add(SparseBlockStructuredMatrix * A, SparseBlockStructuredMatrix * B, double alpha, double beta);

  /**
     SparseBlockStructuredMatrix - SparseBlockStructuredMatrix addition C = alpha*A + beta*B + gamma*C without allocation.
     We assume that C has the correct structure
     
     \param[in] A the matrix to be added
     \param[in] B the matrix to be added
     \param[in] alpha coefficient
     \param[in] beta coefficient
     \param[in] gamma coefficient
     \param[in,out] C the resulting matrix
  */
  void SBM_add_without_allocation(SparseBlockStructuredMatrix * A, SparseBlockStructuredMatrix * B,
                                  double alpha, double beta,
                                  SparseBlockStructuredMatrix * C,
                                  double gamma);

  
  /**
     Multiply a matrix with a double alpha*A --> A
  
     \param alpha the  coefficient
     \param A the   matrix
   */
  void SBM_scal(double alpha, SparseBlockStructuredMatrix * A);
  

  /**
     Row of a SparseMatrix - vector product y = rowA*x or y += rowA*x, rowA being a row of blocks of A
     
     \param[in] sizeX dim of the vector x
     \param[in] sizeY dim of the vector y
     \param[in] currentRowNumber number of the required row of blocks
     \param[in] A the matrix to be multiplied
     \param[in] x the vector to be multiplied
     \param[in,out] y the resulting vector
     \param[in] init = 0 for y += Ax, =1 for y = Ax
  */
  void SBM_row_prod(unsigned int sizeX, unsigned int sizeY, unsigned int currentRowNumber, const SparseBlockStructuredMatrix* const A, const double* const x, double* y, int init);

  /**
     Row of a SparseMatrix - vector product y = rowA*x or y += rowA*x, rowA being a row of blocks of A
     
     \param[in] sizeX dim of the vector x
     \param[in] sizeY dim of the vector y
     \param[in] currentRowNumber number of the required row of blocks
     \param[in] A the matrix to be multiplied
     \param[in] x the vector to be multiplied
     \param[in,out] y the resulting vector
     \param[in] init = 0 for y += Ax, =1 for y = Ax
  */
  void SBM_row_prod_no_diag(unsigned int sizeX, unsigned int sizeY, unsigned int currentRowNumber, const SparseBlockStructuredMatrix* const A, const double* const x, double* y, int init);

  /**
     Row of a SparseMatrix - vector product y = rowA*x or y += rowA*x, rowA being a row of blocks of A of size 3x3
     
     \param[in] sizeX dim of the vector x
     \param[in] sizeY dim of the vector y
     \param[in] currentRowNumber number of the required row of blocks
     \param[in] A the matrix to be multiplied
     \param[in] x the vector to be multiplied
     \param[in,out] y the resulting vector
  */
  void SBM_row_prod_no_diag_3x3(unsigned int sizeX, unsigned int sizeY, unsigned int currentRowNumber, const SparseBlockStructuredMatrix* const A, double* const x, double* y);
  void SBM_row_prod_no_diag_2x2(unsigned int sizeX, unsigned int sizeY, unsigned int currentRowNumber, const SparseBlockStructuredMatrix* const A, double* const x, double* y);
  void SBM_row_prod_no_diag_1x1(unsigned int sizeX, unsigned int sizeY, unsigned int currentRowNumber, const SparseBlockStructuredMatrix* const A, double* const x, double* y);


  void SBM_extract_component_3x3(const SparseBlockStructuredMatrix* const A,
                                 SparseBlockStructuredMatrix*  B,
                                 unsigned int *row_components, unsigned int row_components_size,
                                 unsigned int *col_components, unsigned int col_components_size);

  /** 
      Destructor for SparseBlockStructuredMatrix objects
    
      \param blmat SparseBlockStructuredMatrix the matrix to be destroyed.
   */
  void SBM_clear(SparseBlockStructuredMatrix * blmat);

  /** To free a SBM matrix (for example allocated by NM_new_from_file).
   *
   *  \param[in] A the SparseBlockStructuredMatrix that mus be de-allocated.
   *  \param[in] level use NUMERICS_SBM_FREE_BLOCK | NUMERICS_SBM_FREE_SBM
   */
  void SBMfree(SparseBlockStructuredMatrix* A, unsigned int level);

  /** 
      Screen display of the matrix content
      
      \param m the matrix to be displayed
   */
  void SBM_print(const SparseBlockStructuredMatrix* const m);

  /**
     print in file  of the matrix content

     \param m the matrix to be displayed
     \param file the corresponding  file
  */
  void SBM_write_in_file(const SparseBlockStructuredMatrix* const m, FILE* file);

  /**
     read in file  of the matrix content without performing memory allocation

     \param M the matrix to be displayed
     \param file the corresponding name of the file
  */
  void SBM_read_in_file(SparseBlockStructuredMatrix* const M, FILE *file);

  /**
     Create from file a SparseBlockStructuredMatrix with  memory allocation
     
     \param file the corresponding name of the file
     \return the matrix to be displayed
   */
  SparseBlockStructuredMatrix* SBM_new_from_file(FILE *file);

  /**
     print in file  of the matrix content in Scilab format for each block
     
     \param M the matrix to be displayed
     \param file the corresponding  file
  */
  void SBM_write_in_fileForScilab(const SparseBlockStructuredMatrix* const M, FILE* file);


  /**
     print in file  of the matrix content
     
     \param M the matrix to be displayed
     \param filename the corresponding file
  */
  void SBM_write_in_filename(const SparseBlockStructuredMatrix* const M, const char *filename);

  /** 
      read in file  of the matrix content

      \param M the matrix to be displayed
      \param filename the corresponding name of the file
  */
  void SBM_read_in_filename(SparseBlockStructuredMatrix* const M, const char *filename);

  /**
     Destructor for SparseBlockStructuredMatrixPred objects
     
     \param blmatpred SparseBlockStructuredMatrix, the matrix to be destroyed.
   */
  void SBM_clear_pred(SparseBlockStructuredMatrixPred *blmatpred);

  /** 
      Compute the indices of blocks of the diagonal block
      
      \param M the SparseBlockStructuredMatrix matrix
      \return the indices for all the rows
  */
  unsigned int * SBM_diagonal_block_indices(SparseBlockStructuredMatrix* const M);

  /** 
      Find index of the diagonal block in a row
      
      \param M the SparseBlockStructuredMatrix matrix
      \param row the row of the required block
      \return pos the position of the block
  */
  unsigned int SBM_diagonal_block_index(SparseBlockStructuredMatrix* const M, unsigned int row);

  /** 
      insert an entry into a SparseBlockStructuredMatrix.
      This method is expensive in terms of memory management. For a lot of entries, use
      an alternative
      
      \param M the SparseBlockStructuredMatrix
      \param i row index
      \param j column index
      \param val the value to be inserted.
   */
  int SBM_entry(SparseBlockStructuredMatrix* M, unsigned int row, unsigned int col, double val);

  /**
     get the element of row i and column j of the matrix M
     
     \param M the SparseBlockStructuredMatrix matrix
     \param row the row index
     \param col the column index
     \return the value
  */
  double SBM_get_value(const SparseBlockStructuredMatrix* const M, unsigned int row, unsigned int col);

  /**
     Copy of a SBM  A into B
     
     \param[in] A the SparseBlockStructuredMatrix matrix to be copied
     \param[out]  B the SparseBlockStructuredMatrix matrix copy of A
     \param[in] copyBlock if copyBlock then the content of block are copied, else only the pointers are copied.
     \return 0 if ok
  */
  int SBM_copy(const SparseBlockStructuredMatrix* const A, SparseBlockStructuredMatrix*  B, unsigned int copyBlock);


  /**
     Transpose  by copy of a SBM  A into B
     
     \param[in] A the SparseBlockStructuredMatrix matrix to be copied
     \param[out]  B the SparseBlockStructuredMatrix matrix copy of transpose A
     \return 0 if ok
  */
  int SBM_transpose(const SparseBlockStructuredMatrix* const A, SparseBlockStructuredMatrix*  B);

  /**
     Inverse (in place) a square diagonal block matrix

     \param[in,out] M the SparseBlockStructuredMatrix matrix to be inversed
     \param ipiv worksapce for storign ipiv
     \return 0 ik ok
  */
  int SBM_inverse_diagonal_block_matrix_in_place(const SparseBlockStructuredMatrix*  M, int* ipiv);


  /**
     Copy a SBM into a Dense Matrix
     
     \param[in] A the SparseBlockStructuredMatrix matrix
     \param[in] denseMat pointer on the filled dense Matrix
  */
  void SBM_to_dense(const SparseBlockStructuredMatrix* const A, double *denseMat);

  /** 
      Copy a SBM into a Sparse (CSR) Matrix

      \param[in] A the SparseBlockStructuredMatrix matrix
      \param[in] outSparseMat pointer on the filled sparse Matrix
      \return 0 if ok
  */
  int SBM_to_sparse(const SparseBlockStructuredMatrix* const A, CSparseMatrix *outSparseMat);

  /**
     initMemory of a Sparse (CSR) Matrix form a SBM matrix

     \param[in] A the SparseBlockStructuredMatrix matrix
     \param[in] sparseMat pointer on the initialized sparse Matrix
     \return 0 if ok
  */
  int SBM_to_sparse_init_memory(const SparseBlockStructuredMatrix* const A, CSparseMatrix *sparseMat);

  /**
     Copy a block row of the SBM into a Dense Matrix
     
     \param[in] A the SparseBlockStructuredMatrix matrix to be inversed.
     \param[in] row the block row index copied.
     \param[in] denseMat pointer on the filled dense Matrix.
     \param[in] rowPos line pos in the dense matrix.
     \param[in] rowNb total number of line of the dense matrix.
     The number of line copied is contained in M.
     
   */
  void SBM_row_to_dense(const SparseBlockStructuredMatrix* const A, int row, double *denseMat, int rowPos, int rowNb);

  /**
     
     \param [in] rowIndex: permutation: the row numC of C is the row rowIndex[numC] of A.
     \param [in] A The source SBM.
     \param [out] C The target SBM. It assumes the structure SBM has been allocated.
     The memory allocation for its menber is done inside.
     NB : The blocks are not copied.
  */
  void SBM_row_permutation(unsigned int *rowIndex, SparseBlockStructuredMatrix* A, SparseBlockStructuredMatrix*  C);

  /**
     
     \param [in] colIndex: permutation: the col numC of C is the col colIndex[numC] of A.
     \param [in] A The source SBM.
     \param [out] C The target SBM. It assumes the structure SBM has been allocated.
     The memory allocation for its menber is done inside.
     NB : The blocks are not copied.
  */
  void SBM_column_permutation(unsigned int *colIndex, SparseBlockStructuredMatrix* A, SparseBlockStructuredMatrix*  C);

  void  SBCM_null(SparseBlockCoordinateMatrix* MC);
  
  SparseBlockCoordinateMatrix*  SBCM_new(void);


  

  /**
     allocate a SparseBlockCoordinateMatrix from a list of 3x3
     blocks
     
     \param[in] m the number of rows
     \param[in] n the number of colums
     \param[in] nbblocks the number of blocks
     \param[in] row a pointer to row of each block
     \param[in] column a pointer to column of each block
     \param[in] block a pointer to each block
     \return a pointer to a SparseBlockCoordinateMatrix structure
  */
  SparseBlockCoordinateMatrix*  SBCM_new_3x3(unsigned int m, unsigned int n,
                                             unsigned int nbblocks,
                                             unsigned int *row, unsigned int *column, double *block);


  /**
     free allocated memory in newSparseBlockCoordinateMatrix functions
   
     \param[in] MC matrix pointer */
  void  SBCM_free_3x3(SparseBlockCoordinateMatrix *MC);

  /**
     copy a SparseBlockCoordinateMatrix to a SparseBlockStructuredMatrix
   
     \param[in] MC the SparseBlockCoordinateMatrix matrix
     \return a pointer to a SparseBlockCoordinateMatrix structure
   */
  SparseBlockStructuredMatrix* SBCM_to_SBM(SparseBlockCoordinateMatrix* MC);


  /**
     free a SparseBlockStructuredMatrix created with SBCM_to_SBM
     
     \param[in,out] M a SparseBlockStructuredMatrix to free*/
  void SBM_free_from_SBCM(SparseBlockStructuredMatrix* M);


  /**
     Copy a Sparse Matrix into a SBM, with fixed blocksize
     
     \param[in] blocksize the blocksize
     \param[in] sparseMat pointer on the Sparse Matrix
     \param[in,out] outSBM pointer on an empty SparseBlockStructuredMatrix
     \return 0 in ok
  */
  int SBM_from_csparse(int blocksize, const CSparseMatrix* const sparseMat, SparseBlockStructuredMatrix* outSBM);


#if defined(__cplusplus) && !defined(BUILD_AS_CPP)
}
#endif

#endif /* NSSPACK_H */
