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
/** \file KneeJointR.hpp
 */
#ifndef KneeJointRELATION_H
#define KneeJointRELATION_H

#include <MechanicsFwd.hpp>
#include <SiconosFwd.hpp>
#include <NewtonEulerJointR.hpp>

/**
   This class implements a knee joint between one or two Newton/Euler Dynamical system
*/
class KneeJointR : public NewtonEulerJointR
{
protected:

  ACCEPT_SERIALIZATION(KneeJointR);

  /** Coordinate of the knee point in the body frame of the first dynamical system _d1
   */
  SP::SiconosVector _P0;

  /**Absolute coodinates of the vector  G1P0 when d1 is located in q=(0,0,0,1,0,0,0)
   * i.e. P0 in the body frame of d1.
   * These values are computed when the constructor is called.
   */
  double _G1P0x;
  double _G1P0y;
  double _G1P0z;

  /** Absolute coodinates of the vector G2P0 when d2 is located in q=(0,0,0,1,0,0,0)
   *  i.e. P0 in the body frame of d2.
   * These values are computed when the constructor is called.
   */
  double _G2P0x;
  double _G2P0y;
  double _G2P0z;


public:


  /** Empty constructor. The relation may be initialized later by
   *  setPoint, setAbsolute, and setBasePositions. */
  KneeJointR();

  /** Constructor based on one or two dynamical systems and a point.
   *
   *  \param d1 first DynamicalSystem linked by the joint.
   *  \param d2 second DynamicalSystem linked by the joint, or NULL
   *  for absolute frame.
   *  \param P SiconosVector of size 3 that defines the point around
   *  which rotation is allowed.
   *  \param absoluteRef if true, P is in the absolute frame,
   *  otherwise P is in d1 frame.
   */
  KneeJointR(SP::SiconosVector P, bool absoluteRef,
             SP::NewtonEulerDS d1 = SP::NewtonEulerDS(),
             SP::NewtonEulerDS d2 = SP::NewtonEulerDS());

  /** destructor
   */
  virtual ~KneeJointR() {};

  virtual void initialize(Interaction& inter);

  /** Initialize the joint constants based on the provided base positions.
   *
   *  \param q1 A SiconosVector of size 7 indicating translation and
   *  orientation in inertial coordinates.
   *  \param q2 An optional SiconosVector of size 7 indicating
   *  translation and orientation; if null, the inertial
   *  frame will be considered as the second base. */
  virtual void setBasePositions(SP::SiconosVector q1,
                                SP::SiconosVector q2 = SP::SiconosVector());

  /** Perform some checks on the initial conditions. */
  void checkInitPos(SP::SiconosVector q1, SP::SiconosVector q2);

  /**
     Get the number of constraints defined in the joint
     
     \return the number of constraints
   */
  virtual unsigned int numberOfConstraints() { return 3; }

  /**
     Get the number of degrees of freedom defined in the joint
     
     \return the number of degrees of freedom (DoF)
   */
  virtual unsigned int numberOfDoF() { return 3; }

  /**
     Return the type of a degree of freedom of this joint.
     
     \return the type of the degree of freedom (DoF)
  */
  virtual DoF_Type typeOfDoF(unsigned int axis) {
    if (axis<3) return DOF_TYPE_ANGULAR;
    else return DOF_TYPE_INVALID;
  };

  virtual void computeJachq(double time, Interaction& inter, SP::BlockVector q0);


  /**
     to compute the output y = h(t,q,z) of the Relation
     
     \param time current time value
     \param q coordinates of the dynamical systems involved in the relation
     \param y the resulting vector
  */
  virtual void computeh(double time, const BlockVector& q0, SiconosVector& y);

  virtual void computeDotJachq(double time, const BlockVector& workQ, BlockVector& workZ, const BlockVector& workQdot);

  virtual void computeDotJachq(double time, SP::SiconosVector qdot1, SP::SiconosVector qdot2=SP::SiconosVector());

  SP::SiconosVector P() { return _P0; }

protected:

  virtual void Jd1d2(double X1, double Y1, double Z1, double q10, double q11, double q12, double q13,
                     double X2, double Y2, double Z2, double q20, double q21, double q22, double q23);
  virtual void Jd1(double X1, double Y1, double Z1, double q10, double q11, double q12, double q13);

  /* \warning, the following function should also depend on q */
  virtual void DotJd1d2(double Xdot1, double Ydot1, double Zdot1, double qdot10, double qdot11, double qdot12, double qdot13,
                        double Xdot2, double Ydot2, double Zdot2, double qdot20, double qdot21, double qdot22, double qdot23);
  virtual void DotJd1(double Xdot1, double Ydot1, double Zdot1, double qdot10, double qdot11, double qdot12, double qdot13);


//public:
  double Hx(double X1, double Y1, double Z1, double  q10, double q11, double q12, double q13,
            double X2, double Y2, double Z2, double q20, double q21, double q22, double q23);
  double Hy(double X1, double Y1, double Z1, double q10, double q11, double q12, double q13,
            double X2, double Y2, double Z2, double q20, double q21, double q22, double q23);
  double Hz(double X1, double Y1, double Z1, double q10, double q11, double q12, double q13,
            double X2, double Y2, double Z2, double q20, double q21, double q22, double q23);
};
#endif // KneeJointRELATION_H
