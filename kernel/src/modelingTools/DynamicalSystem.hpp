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

/*! \file DynamicalSystem.hpp
  Abstract class - General interface for all Dynamical Systems.
*/

#ifndef DynamicalSystem_H
#define DynamicalSystem_H

#include "SiconosPointers.hpp"
#include "SiconosFwd.hpp"

#include "SSLH.hpp"
#include "SiconosException.hpp"

#include "SiconosVector.hpp"
#include "SimpleMatrix.hpp"
#include "SiconosMemory.hpp"
#include "DynamicalSystemTypes.hpp"
#include "PluggedObject.hpp"
#include "PluginTypes.hpp"
#include "SiconosVisitor.hpp"

#include <iostream>
/** 

    Abstract interface to Dynamical Systems

    This class is used to describe dynamical systems of the form :
    
    \f$ g(\dot x, x, t, z) = 0 \f$
    
    where
    
    - \f$ x \in R^{n} \f$ is the state.
    - \f$ z \in R^{zSize} \f$ is a vector of arbitrary algebraic
    variables, some sort of discret state.  For example, z may be used
    to set some perturbation parameters, to control the system (z
    set by actuators) and so on.
    - \f$ g : R^{n} \times R  \to  R^{n}   \f$ .
    
    By default, the DynamicalSystem is considered to be an Initial Value
    Problem (IVP) and the initial conditions are given by
    
    \f$ x(t_0)=x_0 \f$
    
    Under some specific conditions, the system can be written as:
    
    \f$ \dot x = rhs(x, t, z) \f$
    
    In that case, \f$ \nabla_{\dot x} g \f$ must be invertible.
    
*/

class DynamicalSystem
{

public:
  /** List of indices used to save tmp work vectors
   * The last value is the size of the present list, so you HAVE to leave it at the end position.
   */
  enum DSWorkVectorId {local_buffer, freeresidu, free, acce_memory, acce_like, sizeWorkV};

private:
  ACCEPT_SERIALIZATION(DynamicalSystem);

  /** used to set ds number */
  static size_t __count;

protected:

  /** An id number for the DynamicalSystem */
  size_t _number;

  /** the dimension of the system (\e ie size of the state vector x) */
  unsigned int _n;

  /** initial state of the system */
  SP::SiconosVector _x0;

  /** the input vector due to the non-smooth law \f$ r \in R^{n} \f$
   * (multiplier, force, ...)
   * \remark V.A. 17/09/2011 :
   * This should be a VectorOfVectors as for _x when higher relative degree
   * systems will be simulated
   */
  SP::SiconosVector _r;

  /** state of the system,
   *  \f$  x \in R^{n} \f$ - With _x[0]= \f$ x \f$ , _x[1]= \f$ \dot{x} \f$ . */
  VectorOfVectors _x;

  /** jacobian according to x of the right-hand side (\f$ rhs = \dot x =
      f(x,t) + r \f$) */
  SP::SiconosMatrix _jacxRhs;

  /** Arbitrary algebraic values vector, z, discrete state of the
      system. */
  SP::SiconosVector _z;

  /** the  previous state vectors stored in memory
   */
  SiconosMemory _xMemory;

  /** number of previous states stored in memory */
  unsigned int _stepsInMemory;

  // ===== CONSTRUCTORS =====

  /** default constructor */
  DynamicalSystem();

  /** minimal constructor, from state dimension
      result in \f$ \dot x = r \f$
   *  \param dimension size of the system (n)
   */
  DynamicalSystem(unsigned int dimension);

  /** Copy constructor
   * \param ds the DynamicalSystem to copy
   */
  DynamicalSystem(const DynamicalSystem & ds);

  /** Initialize all PluggedObject whether they are used or not.
   */
  virtual void _zeroPlugin() = 0;

  /** Common code for constructors
      should be replaced in C++11 by delegating constructors
   */
  void _init();

public:

  /** destructor */
  virtual ~DynamicalSystem() {};

  /** allocate (if needed)  and compute rhs and its jacobian.
   *
   *  \param time of initialization
   */
  virtual void initRhs(double time) = 0 ;

  /** set nonsmooth input to zero
   *
   *  \param level input-level to be initialized.
   */
  virtual void initializeNonSmoothInput(unsigned int level) = 0;

  /** compute all component of the dynamical system, for the current state.
   *
   *  \param time current time (the one used to update ds component)
   */
  void update(double time);

  /** update right-hand side for the current state
   *
   *  \param time of interest
   */
  virtual void computeRhs(double time) = 0;

  /** update \f$ \nabla_x rhs \f$ for the current state
   *
   *  \param time of interest
   */
  virtual void computeJacobianRhsx(double time) = 0;

  /** reset nonsmooth part of the rhs, for all 'levels' */
  virtual void resetAllNonSmoothParts() = 0;

  /** set nonsmooth part of the rhs to zero for a given level
   *
   *  \param level
   */
  virtual void resetNonSmoothPart(unsigned int level) = 0;

  /** returns the id of the dynamical system */
  inline size_t number() const
  {
    return _number;
  }

  /** set the id of the DynamicalSystem
   *
   *  \return the previous value of number
   */
  inline size_t setNumber(size_t new_number)
  {
    size_t old_n = _number;
    _number = new_number;
    return old_n;
  }

  /** returns the size of the vector state x */
  inline unsigned int n() const
  {
    return _n;
  }

  /** 
      returns the dimension of the system 
      (depends on system type, e.g. n for first order,
      ndof for Lagrangian).
   */
  virtual inline unsigned int dimension() const
  {
    return _n;
  };

  /** returns a pointer to the initial state vector */
  inline SP::SiconosVector x0() const
  {
    return _x0;
  };

  /** get a copy of the initial state vector */
  inline const SiconosVector getX0() const
  {
    return *_x0;
  }

  /** set initial state (copy)
   *
   *  \param newValue input vector to copy
   */
  void setX0(const SiconosVector& newValue);

  /** set initial state (pointer link)
   *
   *  \param newPtr vector (pointer) to set x0
   */
  void setX0Ptr(SP::SiconosVector newPtr);

  /** returns a pointer to the state vector \f$ x \f$
   *
   *  \return SP::SiconosVector
   */
  inline SP::SiconosVector x() const
  {
    return _x[0];
  }
 
  /** get a copy of the current state vector \f$ x \f$
   *
   *  \return SiconosVector
   */
  inline const SiconosVector& getx() const
  {
    return *(_x[0]);
  }

  /** set content of current state vector \f$ x \f$
   *
   *  \param newValue SiconosVector 
   */
  void setX(const SiconosVector& newValue);

  /** set state vector \f$ x \f$ (pointer link)
   *
   *  \param newPtr SP::SiconosVector 
   */
  void setXPtr(SP::SiconosVector newPtr);

  /** returns a pointer to r vector (input due to nonsmooth behavior)
   *
   *  \return SP::SiconosVector
   */
  inline SP::SiconosVector r() const
  {
    return _r;
  }

  /** get a copy of r vector (input due to nonsmooth behavior)
   *
   *  \return a SiconosVector
   */
  inline const SiconosVector getR() const
  {
    return *_r;
  }

  /** set r vector (input due to nonsmooth behavior) content (copy)
   *
   *  \param newValue SiconosVector 
   */
  void setR(const SiconosVector& newValue );

  /** set r vector (input due to nonsmooth behavior) (pointer link)
   *
   *  \param newPtr SP::SiconosVector newPtr
   */
  void setRPtr(SP::SiconosVector newPtr);

  /** returns a pointer to the right-hand side vector, (i.e. \f$ \dot x \f$)
   *
   *  \return SP::SiconosVector
   */
  inline SP::SiconosVector rhs() const
  {
    return _x[1];
  }

  /** get a copy of the right-hand side vector, (i.e. \f$ \dot x \f$)
   *
   *  \return SiconosVector
   */
  inline SiconosVector& getRhs() const
  {
    return *(_x[1]);
  }

  /** set the value of the right-hand side, \f$ \dot x \f$
   *
   *  \param newValue SiconosVector
   */
  virtual void setRhs(const SiconosVector& newValue);

  /** set right-hand side, \f$ \dot x \f$ (pointer link)
   *
   *  \param newPtr SP::SiconosVector
   */
  virtual void setRhsPtr(SP::SiconosVector newPtr);

  /** returns a pointer to \f$ \nabla_x rhs()\f$
   *
   *  \return SP::SiconosMatrix
   */
  inline SP::SiconosMatrix jacobianRhsx() const
  {
    return _jacxRhs;
  }

  /** set the value of \f$ \nabla_x rhs() \f$
   *
   *  \param newValue SiconosMatrix
   */
  void setJacobianRhsx(const SiconosMatrix& newValue);

  /** set \f$ \nabla_x rhs() \f$, pointer link
   *
   *  \param newPtr SP::SiconosMatrix  
   */
  void setJacobianRhsxPtr(SP::SiconosMatrix newPtr);

  /** returns a pointer to \f$ z \f$, the vector of algebraic parameters.
   *
   *  \return SP::SiconosVector
   */
  inline SP::SiconosVector z() const
  {
    return _z;
  }

  /** get a copy of \f$ z \f$, the vector of algebraic parameters.
   *
   *  \return a SiconosVector
   */
  inline const SiconosVector& getz() const
  {
    return *_z;
  }

  /** set the value of \f$ z \f$ (copy)
   *
   *  \param newValue SiconosVector 
   */
  void setz(const SiconosVector& newValue) ;

  /** set \f$ z \f$ (pointer link)
   *
   *  \param newPtr SP::SiconosVector 
   */
  void setzPtr(SP::SiconosVector newPtr);

  /** get all the values of the state vector x stored in a SiconosMemory object
   *  (not const due to LinearSMC::actuate)
   *
   *  \return a reference to the SiconosMemory object
   */
  inline SiconosMemory& xMemory()
  {
    return _xMemory;
  }

  /** get all the values of the state vector x stored in a SiconosMemory object
   *
   *  \return a const reference to the SiconosMemory object
   */
  inline const SiconosMemory& xMemory() const
  {
    return _xMemory;
  }

  /** returns the number of step saved in memory for state vector
   *
   *  \return int
   */
  inline unsigned int stepsInMemory() const
  {
    return _stepsInMemory;
  }
  
  /** set number of steps to be saved
   *
   *  \param steps
   */
  inline void setStepsInMemory(unsigned int steps)
  {
    _stepsInMemory = steps;
  }
  
  /** initialize the SiconosMemory objects: reserve memory for i vectors in memory and reset all to zero.
   *
   *  \param steps the size of the SiconosMemory (i)
   */
  virtual void initMemory(unsigned int steps);

  /** push the current values of x and r in memory (index 0 of memory is the last inserted vector)
   *  xMemory and rMemory,
   */
  virtual void swapInMemory() = 0;

  /** call all plugged functions for the current state
   *
   *  \param time  the current time
   */
  virtual void updatePlugins(double time) = 0;
  
  /** reset the global DynamicSystem counter (for ids)
   *
   *  \return the previous value of count
   */
  static inline size_t resetCount(size_t new_count=0)
  {
    size_t old_count = __count;
    __count = new_count;
    return old_count;
  };

  /** reset the state x() to the initial state x0 */
  virtual void resetToInitialState() = 0;
  
  /** \return true if the system is linear
   */
  virtual bool isLinear()
  {
    return false;
  };

  /** print the data of the dynamical system on the standard output
   */
  virtual void display(bool brief = true) const = 0;

  VIRTUAL_ACCEPT_VISITORS(DynamicalSystem);
  
};


#endif // DynamicalSystem_H
