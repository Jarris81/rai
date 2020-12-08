//
// Created by jason on 07.12.20.
//

#include "CtrlSymCommand.h"


CtrlSymCommand::CtrlSymCommand(CommandType command, bool isCondition, rai::String gripper_name, rai::String object_name) {
  this->command = command;
  this->isCondition = isCondition;

  this->frame1 = gripper_name;
  this->frame2 = object_name;
}

bool CtrlSymCommand::isConverged(const rai::Configuration& C) {

  if(this->command == CLOSE_GRIPPER) {
    rai::Frame *gripper = C.getFrame(this->frame1);
    rai::Frame *object = C.getFrame(this->frame2);
    //is object has as parent gripper, its grasping

    if(object->parent != gripper) return false;
  }

}

bool CtrlSymCommand::run(rai::Configuration& C) {

  if(this->command == CLOSE_GRIPPER){
    rai::Frame* gripper = C.getFrame(this->frame1);
    rai::Frame* object = C.getFrame(this->frame2);
    cout<<"Im grasping!!"<<endl;
    C.attach(gripper, object);
  }
  return true;
}

