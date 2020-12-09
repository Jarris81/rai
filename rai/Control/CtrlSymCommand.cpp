//
// Created by jason on 07.12.20.
//

#include "CtrlSymCommand.h"


CtrlSymCommand::CtrlSymCommand(CtrlSymCommandType command, bool isCondition, rai::String gripper_name, rai::String object_name) {
  this->command = command;
  this->isCondition = isCondition;

  this->frame1 = gripper_name;
  this->frame2 = object_name;
}

bool CtrlSymCommand::isConverged(const rai::Configuration& C) {

  switch (this->command) {
    case CLOSE_GRIPPER:{
      rai::Frame *gripper = C.getFrame(this->frame1);
      rai::Frame *object = C.getFrame(this->frame2);

      //is object has as parent gripper, its grasping
      if(object->parent != gripper) return false;
      else return true;
    }
    case OPEN_GRIPPER:{
      rai::Frame *gripper = C.getFrame(this->frame1);
      rai::Frame *object = C.getFrame(this->frame2);

      //is object has as parent gripper, its grasping
      //check if gripper has a child childrren
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
    }


  }
}

bool CtrlSymCommand::run(rai::Configuration& C) {

  switch (this->command) {

    case CLOSE_GRIPPER:{
      rai::Frame* gripper = C.getFrame(this->frame1);
      rai::Frame* object = C.getFrame(this->frame2);
      C.attach(gripper, object);
      return true;
    }
    case OPEN_GRIPPER:{
      rai::Frame* object = C.getFrame(this->frame2);
      C.attach("world", object->name);
      return true;

    }


  }
  // should not happen
  return false;
}

