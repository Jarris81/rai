/*  ------------------------------------------------------------------
    Copyright (c) 2011-2020 Marc Toussaint
    email: toussaint@tu-berlin.de

    This code is distributed under the MIT License.
    Please see <root-path>/LICENSE for details.
    --------------------------------------------------------------  */

#pragma once

#include "CtrlObjective.h"
#include "../Kin/feature.h"
#include "CtrlSymCommand.h"


//===========================================================================

struct CtrlSet {
  rai::String name;
  rai::Array<shared_ptr<CtrlObjective>> objectives;    ///< list of objectives
  rai::Array<shared_ptr<CtrlObjective>> startConditions;    ///< list of objectives
  rai::Array<shared_ptr<CtrlSymCommand>> symbolicCommands; ///< list of symbolic commands

  CtrlSet(const char* _name=0) : name(_name) {}
  shared_ptr<CtrlObjective> addObjective(const ptr<Feature>& f, ObjectiveType type, double transientStep=-1.);
  shared_ptr<CtrlObjective> add_qControlObjective(uint order, double scale, const rai::Configuration& C);
  shared_ptr<CtrlObjective> addStartCondition(const ptr<Feature>& f, ObjectiveType type);

  void addSymbolicCommand(CtrlSymCommandType commandType, StringA frames, bool isImmediate);

  operator rai::Array<shared_ptr<CtrlObjective>>&(){ return objectives; }

  void report(ostream& os=cout) const;

  bool canBeInitiated(const rai::Configuration& pathConfig, double eqPrecision=1e-4) const;
  bool isConverged(const rai::Configuration& pathConfig, double eqPrecision=1e-4) const;
  rai::Array<shared_ptr<CtrlObjective>> getObjectives();
  rai::Array<shared_ptr<CtrlObjective>> getStartConditions();
  rai::Array<shared_ptr<CtrlSymCommand>> getSymbolicCommands();
};

//===========================================================================

bool isFeasible(const CtrlSet& CS, const rai::Configuration& pathConfig, bool initOnly=true, double eqPrecision=1e-4);

CtrlSet operator+(const CtrlSet& A, const CtrlSet& B);
