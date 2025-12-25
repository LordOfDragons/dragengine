/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "skyeSkyListener.h"



// Class skyeSkyListener
////////////////////////

// Constructor, destructor
////////////////////////////

skyeSkyListener::skyeSkyListener(){
}

skyeSkyListener::~skyeSkyListener(){
}



// Notifications
//////////////////

void skyeSkyListener::StateChanged(skyeSky*){
}

void skyeSkyListener::UndoChanged(skyeSky*){
}

void skyeSkyListener::SkyChanged(skyeSky*){
}

void skyeSkyListener::EnvObjectChanged(skyeSky*){
}

void skyeSkyListener::ViewChanged(skyeSky*){
}

void skyeSkyListener::CameraChanged(skyeSky*){
}



void skyeSkyListener::ControllerStructureChanged(skyeSky*){
}

void skyeSkyListener::ControllerChanged(skyeSky* , skyeController*){
}

void skyeSkyListener::ControllerNameChanged(skyeSky*, skyeController*){
}

void skyeSkyListener::ControllerValueChanged(skyeSky* , skyeController*){
}

void skyeSkyListener::ControllerSelectionChanged(skyeSky*){
}

void skyeSkyListener::ActiveControllerChanged(skyeSky*){
}



void skyeSkyListener::LinkStructureChanged(skyeSky*){
}

void skyeSkyListener::LinkChanged(skyeSky* , skyeLink*){
}

void skyeSkyListener::LinkNameChanged(skyeSky*, skyeLink*){
}

void skyeSkyListener::LinkSelectionChanged(skyeSky*){
}

void skyeSkyListener::ActiveLinkChanged(skyeSky*){
}



void skyeSkyListener::LayerStructureChanged(skyeSky*){
}

void skyeSkyListener::LayerChanged(skyeSky* , skyeLayer*){
}

void skyeSkyListener::LayerNameChanged(skyeSky*, skyeLayer*){
}

void skyeSkyListener::LayerSelectionChanged(skyeSky*){
}

void skyeSkyListener::ActiveLayerChanged(skyeSky*){
}



void skyeSkyListener::BodyStructureChanged(skyeSky* , skyeLayer*){
}

void skyeSkyListener::BodyChanged(skyeSky* , skyeLayer*, skyeBody*){
}

void skyeSkyListener::BodySelectionChanged(skyeSky* , skyeLayer*){
}

void skyeSkyListener::ActiveBodyChanged(skyeSky* , skyeLayer*){
}



void skyeSkyListener::TargetChanged(skyeSky* , skyeLayer*, deSkyLayer::eTargets){
}

void skyeSkyListener::ActiveTargetChanged(skyeSky* , skyeLayer*){
}
