/*
        ##########    Copyright (C) 2017 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "ThrobGUI.h"

//--------------------------------------------------------------------------------------------------

class ThrobAudioProcessorEditor final : public AudioProcessorEditor
{
public:
  ThrobAudioProcessorEditor(ThrobAudioProcessor&);
  virtual ~ThrobAudioProcessorEditor() = default;

  void paint(Graphics&) override;
  void resized() override;

private:
  ThrobAudioProcessor& m_processor;
  std::unique_ptr<ThrobGUI> m_gui;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThrobAudioProcessorEditor)
};

//--------------------------------------------------------------------------------------------------
