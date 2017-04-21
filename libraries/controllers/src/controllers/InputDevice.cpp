//
//  InputDevice.cpp
//  input-plugins/src/input-plugins
//
//  Created by Sam Gondelman on 7/15/2015
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//
#include "InputDevice.h"

#include "Input.h"
#include "impl/endpoints/InputEndpoint.h"

namespace controller {

    bool InputDevice::_lowVelocityFilter = false;

    const float DEFAULT_HAND_RETICLE_MOVE_SPEED = 37.5f;
    float InputDevice::_reticleMoveSpeed = DEFAULT_HAND_RETICLE_MOVE_SPEED;

    //Constants for getCursorPixelRangeMultiplier()
    const float MIN_PIXEL_RANGE_MULT = 0.4f;
    const float MAX_PIXEL_RANGE_MULT = 2.0f;
    const float RANGE_MULT = (MAX_PIXEL_RANGE_MULT - MIN_PIXEL_RANGE_MULT) * 0.01f;

    //Returns a multiplier to be applied to the cursor range for the controllers
    float InputDevice::getCursorPixelRangeMult() {
        //scales (0,100) to (MINIMUM_PIXEL_RANGE_MULT, MAXIMUM_PIXEL_RANGE_MULT)
        return InputDevice::_reticleMoveSpeed * RANGE_MULT + MIN_PIXEL_RANGE_MULT;
    }

    float InputDevice::getButton(int channel) const {
        if (!_buttonPressedMap.empty()) {
            if (_buttonPressedMap.find(channel) != _buttonPressedMap.end()) {
                return 1.0f;
            } else {
                return 0.0f;
            }
        }
        return 0.0f;
    }

    float InputDevice::getAxis(int channel) const {
        auto axis = _axisStateMap.find(channel);
        if (axis != _axisStateMap.end()) {
            return (*axis).second;
        } else {
            return 0.0f;
        }
    }

    Pose InputDevice::getPose(int channel) const {
        auto pose = _poseStateMap.find(channel);
        if (pose != _poseStateMap.end()) {
            auto pose2 = (*pose).second;
            //qDebug() << "InputDevice::getPose there is a second one valid? " << pose2.isValid();
            return pose2;
        } else {
            //qDebug() << "InputDevice::getPose return empty invalid";
            return Pose();
        }
    }

    Input InputDevice::makeInput(controller::StandardButtonChannel button) const {
        return Input(_deviceID, button, ChannelType::BUTTON);
    }

    Input InputDevice::makeInput(controller::StandardAxisChannel axis) const {
        return Input(_deviceID, axis, ChannelType::AXIS);
    }

    Input InputDevice::makeInput(controller::StandardPoseChannel pose) const {
        return Input(_deviceID, pose, ChannelType::POSE);
    }

    Input::NamedPair InputDevice::makePair(controller::StandardButtonChannel button, const QString& name) const {
        return Input::NamedPair(makeInput(button), name);
    }

    Input::NamedPair InputDevice::makePair(controller::StandardAxisChannel axis, const QString& name) const {
        return Input::NamedPair(makeInput(axis), name);
    }

    Input::NamedPair InputDevice::makePair(controller::StandardPoseChannel pose, const QString& name) const {
        return Input::NamedPair(makeInput(pose), name);
    }

    float InputDevice::getValue(ChannelType channelType, uint16_t channel) const {
        switch (channelType) {
            case ChannelType::AXIS:
                return getAxis(channel);

            case ChannelType::BUTTON:
                return getButton(channel);

            case ChannelType::POSE:
                return getPose(channel).valid ? 1.0f : 0.0f;

            default:
                break;
        }

        return 0.0f;
    }


    float InputDevice::getValue(const Input& input) const {
        return getValue(input.getType(), input.channel);
    }

    EndpointPointer InputDevice::createEndpoint(const Input& input) const {
        return std::make_shared<InputEndpoint>(input);
    }

}
