//
// Created by jason on 07.12.20.
//

#pragma once

#include "../Kin/kin.h"
#pragma once

#include "CtrlObjective.h"
#include "CtrlSymCommand.h"
#include <Kin/feature.h>

enum CommandType {CLOSE_GRIPPER, OPEN_GRIPPER};

struct CtrlSymCommand {

    bool isCondition;
    CommandType command;

    rai::String frame1;
    rai::String frame2;

    CtrlSymCommand(CommandType command, bool isImmediate, rai::String gripper_name, rai::String object_name);

    bool isConverged(const rai::Configuration& C);

    bool run(rai::Configuration& C);

};

