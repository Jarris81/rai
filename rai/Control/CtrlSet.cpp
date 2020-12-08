/*  ------------------------------------------------------------------
    Copyright (c) 2011-2020 Marc Toussaint
    email: toussaint@tu-berlin.de

    This code is distributed under the MIT License.
    Please see <root-path>/LICENSE for details.
    --------------------------------------------------------------  */

#include "CtrlSet.h"

#include "CtrlTargets.h"

ptr<CtrlObjective> CtrlSet::addObjective(const ptr<Feature>& f, ObjectiveType type, double transientStep) {
  std::shared_ptr<CtrlObjective> t = make_shared<CtrlObjective>();
  t->feat = f;
  t->type = type;
  t->transientStep = transientStep;
  if(t->transientStep>0.) {
    t->setRef(make_shared<CtrlTarget_MaxCarrot>(*t, t->transientStep));
  }
  objectives.append(t);
  return t;
}

shared_ptr<CtrlObjective> CtrlSet::add_qControlObjective(uint order, double _scale, const rai::Configuration& C) {
  return addObjective(symbols2feature(FS_qControl, {}, C, {_scale}, NoArr, order), OT_sos);
}

void CtrlSet::report(std::ostream& os) const {
  for(auto& o: objectives) {
    o->reportState(os);
  }
}

bool CtrlSet::canBeInitiated(const rai::Configuration& pathConfig) const {
  return isFeasible(*this, pathConfig, true);
}

bool CtrlSet::isConverged(const rai::Configuration& pathConfig) const {
  return isFeasible(*this, pathConfig, false);
}

bool isFeasible(const CtrlSet& CS, const rai::Configuration& Ctuple, bool initOnly, double eqPrecision) {
  bool isFeasible=true;
  for(const auto& o: CS.objectives) {
    if(o->type==OT_ineq || o->type==OT_eq) {
      if(!initOnly && o->transientStep>0. && o->movingTarget->isTransient) { isFeasible=false; break; }
      if(!initOnly || o->transientStep<=0.) {
        arr y, J;
        o->feat->eval(y, J, o->feat->getFrames(pathConfig));
        if(o->type==OT_ineq) {
          for(double& yi : y) if(yi>eqPrecision) { isFeasible=false; break; }
        }
        if(o->type==OT_eq) {
          for(double& yi : y) if(fabs(yi)>eqPrecision) { isFeasible=false; break; }
        }
      }
    }
    if(!isFeasible) break;
  }
  //also check symbolic commands
  for (const auto& sc : CS.symbolicCommands){
      if(sc.elem(0) == "grasp"){
          //if we check initiation, and command is not init, skip
          if(sc.elem(1) != "init" && initOnly) continue;

          rai::Frame* gripper = Ctuple.getFrame(sc.elem(2));
          rai::Frame* object = Ctuple.getFrame(sc.elem(3));
          // is object has as parent gripper, its grasping
          if(object->parent != gripper) isFeasible = false;
      }
      if(sc.elem(0) == "open") {
          //if we check initiation, and command is not init, skip
          if (sc.elem(1) != "init" && initOnly) continue;

          rai::Frame *gripper = Ctuple.getFrame(sc.elem(2));
          rai::Frame *object = Ctuple.getFrame(sc.elem(3));
          // is object has as parent gripper, its grasping
          if (object->parent->name != "world") isFeasible = false;
      }
      if(!isFeasible) break;
  }

  return isFeasible;
}

CtrlSet operator+(const CtrlSet& A, const CtrlSet& B){
  CtrlSet CS;
  CS.objectives.resize(A.objectives.N+B.objectives.N);
  CS.objectives.setVectorBlock(A.objectives, 0);
  CS.objectives.setVectorBlock(B.objectives, A.objectives.N);
  return CS;
}





