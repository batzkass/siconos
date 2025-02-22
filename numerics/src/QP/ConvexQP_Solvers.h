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
#ifndef CONVEXQP_SOLVERS_H
#define CONVEXQP_SOLVERS_H

/*!\file ConvexQP_Solvers.h
  \brief Subroutines for the resolution of Variational Inequalites (VI) problems
*/


#include "ConvexQP.h"
#include "SolverOptions.h"
#include "ConvexQP_cst.h"

#if defined(__cplusplus) && !defined(BUILD_AS_CPP)
extern "C"
{
#endif

  /**
     Projected Gradient solver for Convex QP problem
     
     \param problem the variational inequality problem to solve
     \param z global vector (n), in-out parameter
     \param w global vector (n), in-out parameters
     \param info return 0 if the solution is found
     \param options the solver options :
     iparam[0] : Maximum iteration number
     
     dparam[3] : rho  parameter.
     If rho >0, then self-adaptive (Armijo like) procedure.
     If rho <0, then constant rho parameter  (rho <-- -rho)
     Adaptive step-size parameters:
     Adaptive step-size parameters:
     dparam[4] = 2/3.0;  tau
     dparam[5] = 3.0/2.0;  tauinv
     dparam[6] = 0.9;   L
     dparam[7] = 0.3;   Lmin
  */
  void convexQP_ProjectedGradient(ConvexQP* problem,
                                  double *z, double *w,
                                  int* info, SolverOptions* options);

  void convexQP_VI_solver(ConvexQP* problem, double *z, double *w, int* info, SolverOptions* options);
  
  void convexQP_ADMM(ConvexQP* problem,
                     double *z, double *w, double *xi, double *u,
                     int* info, SolverOptions* options);

  void convexQP_ADMM_init(ConvexQP* problem, SolverOptions* options);
  void convexQP_ADMM_free(ConvexQP* problem, SolverOptions* options);


  /** @addtogroup SetSolverOptions
      @{
  */
  void convexQP_ProjectedGradient_set_default(SolverOptions* options);
  void convexQP_ADMM_set_default(SolverOptions* options);
  
  /** @} */


  
#if defined(__cplusplus) && !defined(BUILD_AS_CPP)
}
#endif

#endif
