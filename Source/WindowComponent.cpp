/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  2 May 2012 6:52:33pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "WindowComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
WindowComponent::WindowComponent ()
    : textButton (0),
      textButton2 (0),
      textButton3 (0),
      textButton4 (0),
      textButton5 (0),
      label (0),
      label2 (0),
      label3 (0),
      label4 (0),
      label5 (0)
{
    addAndMakeVisible (textButton = new TextButton (L"new button"));
    textButton->setButtonText (L"Button1");
    textButton->addListener (this);

    addAndMakeVisible (textButton2 = new TextButton (L"new button"));
    textButton2->setButtonText (L"Button2");
    textButton2->addListener (this);

    addAndMakeVisible (textButton3 = new TextButton (L"new button"));
    textButton3->setButtonText (L"Button3");
    textButton3->addListener (this);

    addAndMakeVisible (textButton4 = new TextButton (L"new button"));
    textButton4->setButtonText (L"Button4");
    textButton4->addListener (this);

    addAndMakeVisible (textButton5 = new TextButton (L"new button"));
    textButton5->setButtonText (L"Button5");
    textButton5->addListener (this);

    addAndMakeVisible (label = new Label (L"new label",
                                          L"label1"));
    label->setFont (Font (15.0000f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label2 = new Label (L"new label",
                                           L"label2"));
    label2->setFont (Font (15.0000f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label3 = new Label (L"new label",
                                           L"label3"));
    label3->setFont (Font (15.0000f, Font::plain));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label4 = new Label (L"new label",
                                           L"label4"));
    label4->setFont (Font (15.0000f, Font::plain));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label5 = new Label (L"new label",
                                           L"label5"));
    label5->setFont (Font (15.0000f, Font::plain));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x0));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

WindowComponent::~WindowComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (textButton);
    deleteAndZero (textButton2);
    deleteAndZero (textButton3);
    deleteAndZero (textButton4);
    deleteAndZero (textButton5);
    deleteAndZero (label);
    deleteAndZero (label2);
    deleteAndZero (label3);
    deleteAndZero (label4);
    deleteAndZero (label5);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void WindowComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void WindowComponent::resized()
{
    textButton->setBounds (8, 8, 150, 24);
    textButton2->setBounds (8, 40, 150, 24);
    textButton3->setBounds (8, 72, 150, 24);
    textButton4->setBounds (8, 104, 150, 24);
    textButton5->setBounds (8, 136, 150, 24);
    label->setBounds (176, 8, 200, 24);
    label2->setBounds (176, 40, 200, 24);
    label3->setBounds (176, 72, 200, 24);
    label4->setBounds (176, 104, 200, 24);
    label5->setBounds (176, 136, 200, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void WindowComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        Font f("Nonexistent", 15.0f);
        label->setFont (f);
        label->setText ("Changed Font to nonexistent style", false);
        label3->setText ("Style: " + f.getTypefaceStyle(), false);
        //[/UserButtonCode_textButton]
    }
    else if (buttonThatWasClicked == textButton2)
    {
        //[UserButtonCode_textButton2] -- add your button handler code here..
        // Windows Test Font
        Font f("Tahoma", "Regular", 15.0f);
        // Mac Test Font
        // Font f("Papyrus", "Regular", 15.0f);
        label2->setFont (f);
        label2->setText ("Changed Font to " + f.getTypefaceName() + " " +  f.getTypefaceStyle(), false);
        //[/UserButtonCode_textButton2]
    }
    else if (buttonThatWasClicked == textButton3)
    {
        //[UserButtonCode_textButton3] -- add your button handler code here..
        // Windows Test Font
        Font f("Tahoma", "Regular", 15.0f);
        // Mac Test Font
        // Font f("Papyrus", "Regular", 15.0f);
        Font f2 = f.italicised();
        label2->setFont (f2);
        label2->setText ("Changed Font to " + f.getTypefaceName() + " " +  f.getTypefaceStyle(), false);
        label3->setText ("Style: " + f2.getTypefaceStyle(), false);
        //[/UserButtonCode_textButton3]
    }
    else if (buttonThatWasClicked == textButton4)
    {
        //[UserButtonCode_textButton4] -- add your button handler code here..
        // Windows Test Font
        Font f("Tahoma", "Regular", 15.0f);
        // Mac Test Font
        // Font f("Papyrus", "Regular", 15.0f);
        Font f2 = f.boldened();
        label2->setFont (f2);
        label2->setText ("Changed Font to " + f.getTypefaceName() + " " +  f.getTypefaceStyle(), false);
        label3->setText ("Style: " + f2.getTypefaceStyle(), false);
        //[/UserButtonCode_textButton4]
    }
    else if (buttonThatWasClicked == textButton5)
    {
        //[UserButtonCode_textButton5] -- add your button handler code here..
        label5->setText ("Changed Font to nonexistent style", false);
        //[/UserButtonCode_textButton5]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="WindowComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TEXTBUTTON name="new button" id="c0fe36bccd9217b5" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="8 8 150 24" buttonText="Button1"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="16b558f49dc36186" memberName="textButton2"
              virtualName="" explicitFocusOrder="0" pos="8 40 150 24" buttonText="Button2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="c087fde860b84a2f" memberName="textButton3"
              virtualName="" explicitFocusOrder="0" pos="8 72 150 24" buttonText="Button3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="dfc5a22dc5a546bf" memberName="textButton4"
              virtualName="" explicitFocusOrder="0" pos="8 104 150 24" buttonText="Button4"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="24d322c19bac56f6" memberName="textButton5"
              virtualName="" explicitFocusOrder="0" pos="8 136 150 24" buttonText="Button5"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="4dacfd1b4ddead6f" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="176 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="label1" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="f87f6f1e1a1eed70" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="176 40 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="label2" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="cde0a00111e29627" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="176 72 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="label3" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="effd9b2d80e08395" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="176 104 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="label4" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="cb1f9c1564eb44e4" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="176 136 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="label5" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
