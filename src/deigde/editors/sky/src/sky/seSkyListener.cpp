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

#include "seSkyListener.h"



// Class seSkyListener
////////////////////////

// Constructor, destructor
////////////////////////////

seSkyListener::seSkyListener(){
}

seSkyListener::~seSkyListener(){
}



// Notifications
//////////////////

void seSkyListener::StateChanged( seSky*  ){
}

void seSkyListener::UndoChanged( seSky*  ){
}

void seSkyListener::SkyChanged( seSky*  ){
}

void seSkyListener::EnvObjectChanged( seSky*  ){
}

void seSkyListener::ViewChanged( seSky*  ){
}

void seSkyListener::CameraChanged( seSky*  ){
}



void seSkyListener::ControllerStructureChanged( seSky*  ){
}

void seSkyListener::ControllerChanged( seSky* , seController* ){
}

void seSkyListener::ControllerNameChanged( seSky*, seController* ){
}

void seSkyListener::ControllerValueChanged( seSky* , seController* ){
}

void seSkyListener::ControllerSelectionChanged( seSky*  ){
}

void seSkyListener::ActiveControllerChanged( seSky*  ){
}



void seSkyListener::LinkStructureChanged( seSky*  ){
}

void seSkyListener::LinkChanged( seSky* , seLink* ){
}

void seSkyListener::LinkNameChanged( seSky*, seLink* ){
}

void seSkyListener::LinkSelectionChanged( seSky*  ){
}

void seSkyListener::ActiveLinkChanged( seSky*  ){
}



void seSkyListener::LayerStructureChanged( seSky*  ){
}

void seSkyListener::LayerChanged( seSky* , seLayer* ){
}

void seSkyListener::LayerNameChanged( seSky*, seLayer* ){
}

void seSkyListener::LayerSelectionChanged( seSky*  ){
}

void seSkyListener::ActiveLayerChanged( seSky*  ){
}



void seSkyListener::BodyStructureChanged( seSky* , seLayer* ){
}

void seSkyListener::BodyChanged( seSky* , seLayer*, seBody* ){
}

void seSkyListener::BodySelectionChanged( seSky* , seLayer* ){
}

void seSkyListener::ActiveBodyChanged( seSky* , seLayer* ){
}



void seSkyListener::TargetChanged( seSky* , seLayer*, deSkyLayer::eTargets ){
}

void seSkyListener::ActiveTargetChanged( seSky* , seLayer* ){
}
