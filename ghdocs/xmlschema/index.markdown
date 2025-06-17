---
layout: page
title: XML Schema
permalink: /xmlschema/
nav_order: 3
has_children: false
---

# XML Schema

Here you can find the XML Schema for XML based Drag[en]gine file formats.
These are suitable for editors like Visual Studio Code to provide verification and auto completion.

| Type | Pattern | Schema | Documentation |
| --- | --- | --- | --- |
| Behavior Tree | .debt | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/behaviorTree.xsd) | [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:behaviortrees) |
| Border | .debor | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/border.xsd) | [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:xmlguitheme:borderfactory) |
| Canvas Creator | .decc | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/canvasCreator.xsd) | [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:canvascreators) |
| Camera | .decamera | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/camera.xsd) | [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:behavior_camera?s[]=ecbehaviorcamera) |
| Decoration | .dedeco | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/decoration.xsd) | [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:xmlguitheme:decoration) |
| Element Class | .deeclass | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/elementClass.xsd) | [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:behavior_elements#xml_element_classes) |
| Gui Theme | .degt | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/guitheme.xsd) | [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:xmlguitheme:guitheme) |
| Mouse Pointer | .demp | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/mousePointer.xsd) | [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:xmlguitheme:mousepointer) |
| Parameter Tree | .dept | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/parameterTree.xsd) | |
| Simple Conversation | .desconvo | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/simpleConversation.xsd) | [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:simpleconversation) |
| Skin Annotations | .deskinann | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/skinAnnotations.xsd) |
| State Machine | .desm | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/stateMachine.xsd) | [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:statemachines) |
| Widget Designer | .dedes | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/designer.xsd) | [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:xmlguitheme:widgetdesigner) |

These are helper schemas included in the above schemas containing common types. They contain only type definitions. You can use them in your own schemas like this:
```xml
<xs:include schemaLocation='https://lordofdragons.github.io/dragengine/artifacts/xmlschema/dragengine/latest/dragengineTypes.xsd'/>
```

| Type | Schema |
| --- | --- |
| Drag[en]gine Data Types | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/dragengineTypes.xsd) |
| Drag[en]gine Parameter Types | [XSD]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/parameterTypes.xsd) |
