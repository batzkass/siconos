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

#include "TimeDiscretisation.hpp"
#include "ModelingTools.hpp"
#include "SimulationTools.hpp"

#include "ControlManager.hpp"
#include "Sensor.hpp"
#include "Actuator.hpp"
#include "Observer.hpp"
#include "ControlSimulation.hpp"
#include "ControlSimulation_impl.hpp"

ControlSimulation::ControlSimulation(double t0, double T, double h):
  _t0(t0), _T(T), _h(h), _theta(0.5), _elapsedTime(0.0), _N(0), _saveOnlyMainSimulation(false), _silent(false)
{
  _nsds.reset(new NonSmoothDynamicalSystem(_t0, _T));
  _processTD.reset(new TimeDiscretisation(_t0, _h));

}

void ControlSimulation::initialize()
{
  std::pair<unsigned, std::string> res;
  _dataLegend = "time";

  // Simulation part
  _processSimulation->setNonSmoothDynamicalSystemPtr(_nsds);
  // _model->setSimulation(_processSimulation);
  // _model->initialize();
  // Control part
  _CM->initialize(*_nsds);

  // Output
  _N = (unsigned)ceil((_T - _t0) / _h) + 10; // Number of time steps
  DynamicalSystemsGraph& DSG0 = *_nsds->topology()->dSG(0);
  InteractionsGraph& IG0 = *_nsds->topology()->indexSet0();
  res = getNumberOfStates(DSG0, IG0);
  _nDim = res.first;
  _dataLegend += res.second;
  if(!_saveOnlyMainSimulation)
  {
    // iter over controller and observer
    const Actuators& allActuators = _CM->getActuators();
    for(ActuatorsIterator it = allActuators.begin(); it != allActuators.end(); ++it)
    {
      if((*it)->getInternalNSDS())
      {
        Topology& topo = *(*it)->getInternalNSDS()->topology();
        res = getNumberOfStates(*topo.dSG(0), *topo.indexSet0());
        _nDim += res.first;
        _dataLegend += res.second;
      }
    }
    const Observers& allObservers = _CM->getObservers();
    for(ObserversIterator it = allObservers.begin(); it != allObservers.end(); ++it)
    {
      if((*it)->getInternalNSDS())
      {
        Topology& topo = *(*it)->getInternalNSDS()->topology();
        res = getNumberOfStates(*topo.dSG(0), *topo.indexSet0());
        _nDim += res.first;
        _dataLegend += res.second;
      }
    }
  }
  _dataM.reset(new SimpleMatrix(_N, _nDim + 1)); // we save the system state
}

void ControlSimulation::setTheta(unsigned int newTheta)
{
  _theta = newTheta;
}

void ControlSimulation::addDynamicalSystem(SP::DynamicalSystem ds, const std::string& name)
{
  _nsds->insertDynamicalSystem(ds);
  _processSimulation->associate(_processIntegrator, ds);

  if(!name.empty())
  {
    _nsds->setName(ds, name);
  }
}

void ControlSimulation::addSensor(SP::Sensor sensor, const double h)
{
  if(h < _h)
  {
    THROW_EXCEPTION("The timestep for a sensor cannot be smaller than the one for the simulation");
  }

  SP::TimeDiscretisation td(new TimeDiscretisation(_t0, h));
  _CM->addSensorPtr(sensor, td);
}

void ControlSimulation::addActuator(SP::Actuator actuator, const double h)
{
  if(h < _h)
  {
    THROW_EXCEPTION("The timestep for an actuator cannot be smaller than the one for the simulation");
  }

  SP::TimeDiscretisation td(new TimeDiscretisation(_t0, h));
  _CM->addActuatorPtr(actuator, td);
}

void ControlSimulation::addObserver(SP::Observer observer, const double h)
{
  if(h < _h)
  {
    THROW_EXCEPTION("The timestep for an observer cannot be smaller than the one for the simulation");
  }

  SP::TimeDiscretisation td(new TimeDiscretisation(_t0, h));
  _CM->addObserverPtr(observer, td);
}

void ControlSimulation::storeData(unsigned indx)
{
  unsigned startingColumn = 1;
  startingColumn = storeAllStates(indx, startingColumn, *_DSG0, *_IG0, *_dataM);

  if(!_saveOnlyMainSimulation)
  {
    // iter over controller and observer
    const Actuators& allActuators = _CM->getActuators();
    for(ActuatorsIterator it = allActuators.begin(); it != allActuators.end(); ++it)
    {
      if((*it)->getInternalNSDS())
      {
        Topology& topo = *(*it)->getInternalNSDS()->topology();
        startingColumn = storeAllStates(indx, startingColumn, *topo.dSG(0), *topo.indexSet0(), *_dataM);
      }
    }
    const Observers& allObservers = _CM->getObservers();
    for(ObserversIterator it = allObservers.begin(); it != allObservers.end(); ++it)
    {
      if((*it)->getInternalNSDS())
      {
        Topology& topo = *(*it)->getInternalNSDS()->topology();
        startingColumn = storeAllStates(indx, startingColumn, *topo.dSG(0), *topo.indexSet0(), *_dataM);
      }
    }
  }
}


