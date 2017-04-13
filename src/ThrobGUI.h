/*
        ##########    Copyright (C) 2017 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"

#include "UI/GraphicKnob.h"
#include "UI/Led.h"

//--------------------------------------------------------------------------------------------------

class ThrobGUI final : public Component, public SliderListener, public ButtonListener
{
public:
  ThrobGUI(ThrobAudioProcessor& ap_);
  ~ThrobGUI();

  void sliderValueChanged(Slider* sliderThatWasMoved) override;

  void paint(Graphics& g) override;
  void resized() override;
  void buttonClicked(Button* buttonThatWasClicked) override;

private:
  ThrobAudioProcessor& m_audioProcessor;
  Image m_knobStrip;
  Image m_knobStripHover;

  ScopedPointer<ImageButton> m_btnPpqn1;
  ScopedPointer<ImageButton> m_btnPpqn8;
  ScopedPointer<ImageButton> m_btnPpqn24;
  ScopedPointer<Led> m_ledActive;
  ScopedPointer<Led> m_ledLooping;
  ScopedPointer<GraphicKnob> m_knob;
  ScopedPointer<Label> m_lblMultiplier;
  Image m_backgroundImage;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThrobGUI)
};

//--------------------------------------------------------------------------------------------------
