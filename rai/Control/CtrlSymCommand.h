//
// Created by jason on 07.12.20.
//

#pragma once

#include "../Kin/kin.h"
#pragma once

#include "CtrlObjective.h"
#include "CtrlSymCommand.h"
#include <Kin/feature.h>

enum CtrlSymCommandType {SC_CLOSE_GRIPPER, SC_OPEN_GRIPPER};

struct CtrlSymCommand {

    bool isCondition;
    CtrlSymCommandType command;
    StringA frames;

    CtrlSymCommand();//CtrlSymCommandType command, bool isImmediate, rai::String gripper_name, rai::String object_name);

    bool isConverged(const rai::Configuration& C);

    bool run(rai::Configuration& C);

};

