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

- [Behavior Tree (*.debt)]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/behaviorTree.xsd): Documentation [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:behaviortrees)

- [Border (*.deborder)]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/border.xsd): Documentation [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:xmlguitheme:borderfactory)

- [Canvas Creator (*.deccreator)]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/canvasCreator.xsd): Documentation [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:canvascreators)

- [Decoration (*.dedecoration)]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/decoration.xsd): Documentation [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:xmlguitheme:decoration)

- [Element Class (*.deeclass)]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/elementClass.xsd): Documentation [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:behavior_elements#xml_element_classes)

- [Gui Theme (*.deguitheme)]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/guitheme.xsd): Documentation [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:xmlguitheme:guitheme)

- [Mouse Pointer (*.dempointer)]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/mousePointer.xsd): Documentation [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:xmlguitheme:mousepointer)

- [Parameter Tree (*.dept)]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/parameterTree.xsd)

- [Simple Conversation (*.desconvo)]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/simpleConversation.xsd): Documentation [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:simpleconversation)

- [State Machine (*.desm)]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/stateMachine.xsd): Documentation [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:statemachines)

- [Widget Designer (*.designer)]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/designer.xsd): Documentation [Wiki](https://developer.dragondreams.ch/wiki/doku.php/dragengine:modules:dragonscript:xmlguitheme:widgetdesigner)

These are helper schemas included in the above schemas containing common types. They contain only type definitions. You can use them in your own schemas like this:
```xml
<xs:include schemaLocation='{{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/dragengineTypes.xsd'/>
```

- [Drag[en]gine Data Types]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/dragengineTypes.xsd)
- [Drag[en]gine Parameter Types]({{ site.baseurl }}/artifacts/xmlschema/dragengine/latest/parameterTypes.xsd)
