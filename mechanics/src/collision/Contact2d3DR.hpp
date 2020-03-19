/* Siconos is a program dedicated to modeling, simulation and control
 * of non smooth dynamical systems.
 *
 * Copyright 2018 INRIA.
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

#ifndef Contact2d3DR_hpp
#define Contact2d3DR_hpp

#include "MechanicsFwd.hpp"
#include "SiconosVector.hpp"
#include "Lagrangian2d3DR.hpp"

class Contact2d3DR : public Lagrangian2d3DR
{
private:
  /** serialization hooks
  */
  ACCEPT_SERIALIZATION(Contact2d3DR);

public:
  Contact2d3DR();

  /* For users that may require extra information about contacts. */
  SP::SiconosVector base[2];
  SP::SiconosShape shape[2];
  SP::SiconosContactor contactor[2];
  SP::RigidBody2dDS ds[2];

  virtual void computeh(SiconosVector& q, SiconosVector& z, SiconosVector& y);

  /** Update this contact point information.
   * \param pos1 Position on ds1 in ds1 frame.
   * \param pos2 Position on ds2 in ds2 frame (or world frame if ds2=null).
   * \param normal Normal in ds2 frame (or world frame if ds2=null).
   */
  virtual void updateContactPoints(const SiconosVector& pos1,
                                   const SiconosVector& pos2,
                                   const SiconosVector& normal);

  virtual void preDelete() {}

  ACCEPT_STD_VISITORS();
};

#endif