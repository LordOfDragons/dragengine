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

#ifdef IGDE_TOOLKIT_FOX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "foxtoolkit.h"
#include "../../igdeWidget.h"
#include "../../igdeContainer.h"
#include "../../resources/igdeHotKey.h"

#include <dragengine/common/exceptions.h>



// Structs
////////////

igdeUIFoxHelper::sChildLayoutFlags::sChildLayoutFlags() :
flags(0),
canResizeVertical(false),
canResizeHorizontal(false){
}



// Class igdeWidget
/////////////////////

igdeUIFoxHelper::igdeUIFoxHelper(){}



// Functions
//////////////

FXString igdeUIFoxHelper::AccelString(const igdeHotKey &hotKey){
	const deInputEvent::eKeyCodes keyCode = hotKey.GetKeyCode();
	if(keyCode == deInputEvent::ekcUndefined){
		return "";
	}
	
	const bool shift = (hotKey.GetModifiers() & deInputEvent::esmShift) == deInputEvent::esmShift;
	const bool control = (hotKey.GetModifiers() & deInputEvent::esmControl) == deInputEvent::esmControl;
	const bool alt = (hotKey.GetModifiers() & deInputEvent::esmAlt) == deInputEvent::esmAlt;
	
	FXString string;
	
	if(alt){
		string += "Alt-";
	}
	if(control){
		string += "Ctl-";
	}
	if(shift){
		string += "Shift-";
	}
	
	switch(keyCode){
	case deInputEvent::ekcSpace:
		string += "Space";
		break;
		
	case deInputEvent::ekcBackSpace:
		string += "BackSpace";
		break;
		
	case deInputEvent::ekcTab:
		string += "Tab";
		break;
		
	case deInputEvent::ekcReturn:
		string += "Return";
		break;
		
	case deInputEvent::ekcPause:
		string += "Pause";
		break;
		
	case deInputEvent::ekcEscape:
		string += "Escape";
		break;
		
	case deInputEvent::ekcInsert:
		string += "Insert";
		break;
		
	case deInputEvent::ekcDelete:
		string += "Delete";
		break;
		
	case deInputEvent::ekcArrowLeft:
		string += "Left";
		break;
		
	case deInputEvent::ekcArrowUp:
		string += "Up";
		break;
		
	case deInputEvent::ekcArrowRight:
		string += "Right";
		break;
		
	case deInputEvent::ekcArrowDown:
		string += "Down";
		break;
		
	case deInputEvent::ekcPageUp:
		string += "PgUp";
		break;
		
	case deInputEvent::ekcPageDown:
		string += "PgDn";
		break;
		
	case deInputEvent::ekcHome:
		string += "Home";
		break;
		
	case deInputEvent::ekcEnd:
		string += "End";
		break;
		
	default:{
		FXString key;
		
		if(keyCode >= deInputEvent::ekcA && keyCode <= deInputEvent::ekcZ){
			key.format("%c", 'A' + (keyCode - deInputEvent::ekcA));
			
		}else if(keyCode >= deInputEvent::ekc0 && keyCode <= deInputEvent::ekc9){
			key.format("%d", keyCode - deInputEvent::ekc0);
			
		}else if(keyCode >= deInputEvent::ekcF1 && keyCode <= deInputEvent::ekcF12){
			key.format("F%d", keyCode - deInputEvent::ekcF1 + 1);
			
		}else{
			key = "?";
		}
		
		string += key;
		}
	};
	
	return string;
}

FXuchar igdeUIFoxHelper::MnemonicKey(deInputEvent::eKeyCodes keyCode){
	if(keyCode >= deInputEvent::ekcA && keyCode <= deInputEvent::ekcZ){
		return 'a' + (keyCode - deInputEvent::ekcA);
		
	}else if(keyCode >= deInputEvent::ekc0 && keyCode <= deInputEvent::ekc9){
		return '0' + (keyCode - deInputEvent::ekc0);
		
	}else{
		return 0;
	}
}

FXString igdeUIFoxHelper::MnemonizeString(const char* string, deInputEvent::eKeyCodes keyCode){
	const FXuchar key = igdeUIFoxHelper::MnemonicKey(keyCode);
	if(key == 0){
		return FXString(string);
	}
	
	FXString lowercased(string);
	lowercased.lower();
	
	const int index = lowercased.find(key);
	if(index == -1){
		return FXString(string);
	}
	
	return FXString(string, index) + "&" + (string + index);
}

deInputEvent::eKeyCodes igdeUIFoxHelper::KeyCodeFromEvent(const FXEvent &event){
	if(event.code >= KEY_A && event.code <= KEY_Z){
		return (deInputEvent::eKeyCodes)(deInputEvent::ekcA + (event.code - KEY_A));
		
	}else if(event.code >= KEY_a && event.code <= KEY_z){
		return (deInputEvent::eKeyCodes)(deInputEvent::ekcA + (event.code - KEY_a));
		
	}else if(event.code >= KEY_0 && event.code <= KEY_9){
		return (deInputEvent::eKeyCodes)(deInputEvent::ekc0 + (event.code - KEY_0));
		
	}else if(event.code >= KEY_KP_0 && event.code <= KEY_KP_9){
		return (deInputEvent::eKeyCodes)(deInputEvent::ekc0 + (event.code - KEY_KP_0));
		
	}else if(event.code >= KEY_F1 && event.code <= KEY_F12){
		return (deInputEvent::eKeyCodes)(deInputEvent::ekcF1 + (event.code - KEY_F1));
	}
	
	switch(event.code){
	case KEY_space:
		return deInputEvent::ekcSpace;
		
	case KEY_BackSpace:
		return deInputEvent::ekcBackSpace;
		
	case KEY_Tab:
		return deInputEvent::ekcTab;
		
	case KEY_Return:
		return deInputEvent::ekcReturn;
		
	case KEY_Pause:
		return deInputEvent::ekcPause;
		
	case KEY_Escape:
		return deInputEvent::ekcEscape;
		
	case KEY_Insert:
		return deInputEvent::ekcInsert;
		
	case KEY_Delete:
		return deInputEvent::ekcDelete;
		
	case KEY_Shift_L:
	case KEY_Shift_R:
		return deInputEvent::ekcShift;
		
	case KEY_Control_L:
	case KEY_Control_R:
		return deInputEvent::ekcControl;
		
	case KEY_Alt_L:
	case KEY_Alt_R:
		return deInputEvent::ekcAlt;
		
	case KEY_Meta_L:
	case KEY_Meta_R:
		return deInputEvent::ekcMeta;
		
	case KEY_Super_L:
	case KEY_Super_R:
		return deInputEvent::ekcSuper;
		
	case KEY_Hyper_L:
	case KEY_Hyper_R:
		return deInputEvent::ekcHyper;
		
	case KEY_Left:
	case KEY_KP_Left:
		return deInputEvent::ekcArrowLeft;
		
	case KEY_Up:
	case KEY_KP_Up:
		return deInputEvent::ekcArrowUp;
		
	case KEY_Right:
	case KEY_KP_Right:
		return deInputEvent::ekcArrowRight;
		
	case KEY_Down:
	case KEY_KP_Down:
		return deInputEvent::ekcArrowDown;
		
	case KEY_Page_Up:
	case KEY_KP_Page_Up:
		return deInputEvent::ekcPageUp;
		
	case KEY_Page_Down:
	case KEY_KP_Page_Down:
		return deInputEvent::ekcPageDown;
		
	case KEY_Home:
	case KEY_KP_Home:
		return deInputEvent::ekcHome;
		
	case KEY_End:
	case KEY_KP_End:
		return deInputEvent::ekcEnd;
		
	default:
		return deInputEvent::ekcUndefined;
	}
}

int igdeUIFoxHelper::ModifiersFromEvent(const FXEvent &event){
	int modifiers = 0;
	
	if((event.state & SHIFTMASK) == SHIFTMASK){
		modifiers |= deInputEvent::esmShift;
	}
	if((event.state & CONTROLMASK) == CONTROLMASK){
		modifiers |= deInputEvent::esmControl;
	}
	if((event.state & ALTMASK) == ALTMASK){
		modifiers |= deInputEvent::esmAlt;
	}
	if((event.state & METAMASK) == METAMASK){
		modifiers |= deInputEvent::esmMeta;
	}
	
	return modifiers;
}

FXString igdeUIFoxHelper::FilePatternListToFOX(const igdeFilePattern::List &filePatterns){
	decString foxString;
	
	filePatterns.Visit([&](const igdeFilePattern &fp){
		if(!foxString.IsEmpty()){
			foxString.AppendCharacter('\n');
		}
		
		foxString.AppendFormat("%s (%s)", fp.GetName().GetString(), fp.GetPattern().GetString());
	});
	
	//foxString.Append( "\nAll Files (*)" );
	return FXString(foxString.GetString());
}



int igdeUIFoxHelper::GetChildLayoutFlags(igdeWidget *widget, int defaultFlags){
	return GetChildLayoutFlagsAll(widget, defaultFlags).flags;
}

igdeUIFoxHelper::sChildLayoutFlags igdeUIFoxHelper::GetChildLayoutFlagsAll(
igdeWidget *widget, int defaultFlags){
	if(!widget){
		DETHROW(deeInvalidParam);
	}
	
	igdeContainer * const pparent = widget->GetParent();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const foxContainer = (FXComposite*) pparent->GetNativeContainer();
	if(!foxContainer){
		DETHROW(deeInvalidParam);
	}
	
	sChildLayoutFlags clflags;
	clflags.widget = widget;
	clflags.flags = defaultFlags;
	
	foxContainer->handle(foxContainer, FXSEL(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0), &clflags);
	
	return clflags;
}

void igdeUIFoxHelper::UpdateLayoutFlags(igdeWidget *widget){
	if(!widget){
		DETHROW(deeInvalidParam);
	}
	
	igdeContainer * const pparent = widget->GetParent();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const foxContainer = (FXComposite*) pparent->GetNativeContainer();
	if(!foxContainer){
		DETHROW(deeInvalidParam);
	}
	
	sChildLayoutFlags clflags;
	
	clflags.widget = widget;
	FXWindow * const foxWidget = (FXWindow*)widget->GetNativeWidget();
	if(!foxWidget){
		DETHROW(deeInvalidParam);
	}
	
	foxContainer->handle(foxContainer, FXSEL(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0), &clflags);
	
	foxWidget->setLayoutHints(clflags.flags & 0xfff);
}

void igdeUIFoxHelper::UpdateLayoutFlagsChildren(igdeContainer *container){
	if(!container){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const foxContainer = (FXComposite*)container->GetNativeContainer();
	if(!foxContainer){
		DETHROW(deeInvalidParam);
	}
	
	sChildLayoutFlags clflags;
	
	container->GetChildren().Visit([&](igdeWidget *child){
		clflags.widget = child;
		FXWindow * const foxWidget = static_cast<FXWindow*>(clflags.widget->GetNativeWidget());
		if(!foxWidget){
			DETHROW(deeInvalidParam);
		}
		
		foxContainer->handle(foxContainer, FXSEL(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0), &clflags);
		
		foxWidget->setLayoutHints(clflags.flags & 0xfff);
	});
}

FXColor igdeUIFoxHelper::BlendColor(FXColor color1, FXColor color2, float factor){
	const float c1r = (float)FXREDVAL(color1) / 255.f;
	const float c1g = (float)FXGREENVAL(color1) / 255.f;
	const float c1b = (float)FXBLUEVAL(color1) / 255.f;
	
	const float c2r = (float)FXREDVAL(color2) / 255.f;
	const float c2g = (float)FXGREENVAL(color2) / 255.f;
	const float c2b = (float)FXBLUEVAL(color2) / 255.f;
	
	const int c3r = decMath::clamp((int)(decMath::mix(c1r, c2r, factor) * 255.0f), 0, 255);
	const int c3g = decMath::clamp((int)(decMath::mix(c1g, c2g, factor) * 255.0f), 0, 255);
	const int c3b = decMath::clamp((int)(decMath::mix(c1b, c2b, factor) * 255.0f), 0, 255);
	
	return FXRGB(c3r, c3g, c3b);
}

FXColor igdeUIFoxHelper::ConvertColor(const decColor &color){
	const int r = decMath::clamp((int)(color.r * 255.0f), 0, 255);
	const int g = decMath::clamp((int)(color.g * 255.0f), 0, 255);
	const int b = decMath::clamp((int)(color.b * 255.0f), 0, 255);
	return FXRGB(r, g, b);
}

decColor igdeUIFoxHelper::ConvertColor(FX::FXColor color){
	return decColor((float)FXREDVAL(color) / 255.f,
		(float)FXGREENVAL(color) / 255.f, (float)FXBLUEVAL(color) / 255.f);
}

FXColor igdeUIFoxHelper::BrighterColor(FXColor color, int amount){
	const int r = decMath::min(FXREDVAL(color) + amount, 255);
	const int g = decMath::min(FXGREENVAL(color) + amount, 255);
	const int b = decMath::min(FXBLUEVAL(color) + amount, 255);
	return FXRGB(r, g, b);
}

int igdeUIFoxHelper::DebugCountWindows(FXWindow *rootWindow){
	if(!rootWindow){
		rootWindow = FXApp::instance()->getRootWindow();
	}
	
	FXWindow *w = rootWindow->getFirst();
	int count = 0;
	while(w){
		count += 1 + DebugCountWindows(w);
		w = w->getNext();
	}
	return count;
}

#endif
