/*
        ##########    Copyright (C) 2017 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//--------------------------------------------------------------------------------------------------

ThrobAudioProcessorEditor::ThrobAudioProcessorEditor(ThrobAudioProcessor& p)
  : AudioProcessorEditor(&p), m_processor(p), m_gui(new ThrobGUI(p))
{
  setSize(300, 200);
  addAndMakeVisible(m_gui.get());
}

//--------------------------------------------------------------------------------------------------

void ThrobAudioProcessorEditor::paint(Graphics& g)
{
}

//--------------------------------------------------------------------------------------------------

void ThrobAudioProcessorEditor::resized()
{
}

//--------------------------------------------------------------------------------------------------
