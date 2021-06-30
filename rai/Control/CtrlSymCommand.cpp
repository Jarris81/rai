//
// Created by jason on 07.12.20.
//

#include "CtrlSymCommand.h"


CtrlSymCommand::CtrlSymCommand() {};

bool CtrlSymCommand::isConverged(const rai::Configuration& C) {

  switch (this->command) {
    case SC_CLOSE_GRIPPER:{
      rai::Frame *gripper = C.getFrame(this->frames.elem(0));
      rai::Frame *object = C.getFrame(this->frames.elem(1));

      //if object has as parent gripper, its grasping
      if(object->parent != gripper) return false;
      else return true;
    }
    case SC_OPEN_GRIPPER:{
      rai::Frame *gripper = C.getFrame(this->frames.elem(0));
      rai::Frame *object = C.getFrame(this->frames.elem(1));

      //is object has as parent gripper, its grasping
      //check if gripper has a child children
      for(auto child: gripper->children){
        if(child == object){
          return false;
        }
      }
      return true;

    }
    default:{
      //should not happen
      cout<<"Undefined symbolic command, please add definition"<<endl;
      return false;
    }
  }
}

bool CtrlSymCommand::run(rai::Configuration& C) {

  switch (this->command) {

    case SC_CLOSE_GRIPPER:{
      rai::Frame *gripper = C.getFrame(this->frames.elem(0));
      rai::Frame *object = C.getFrame(this->frames.elem(1));
      C.attach(gripper, object);
      return true;
    }
    case SC_OPEN_GRIPPER:{
      rai::Frame* object = C.getFrame(this->frames.elem(1));
      C.attach("world", object->name);
      return true;

    }
    default:{
      //should not happen
      cout<<"Undefined symbolic command, please add definition"<<endl;
      return false;
    }

  }
  // should not happen
  return false;
}
