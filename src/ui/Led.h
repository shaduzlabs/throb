/*
        ##########    Copyright (C) 2017 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "JuceHeader.h"

//--------------------------------------------------------------------------------------------------

class Led : public Component, public Timer
{
public:
  enum class State
  {
    off,
    blue,
    green,
    orange,
    red,
  };

  Led();
  virtual ~Led() = default;

  void setState(State state_, unsigned timoutMs_ = 0);

  virtual void timerCallback()
  {
    setState(State::off);
  }

  void paint(Graphics& g);
  void resized();

private:
  Image m_image;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Led)
};

//--------------------------------------------------------------------------------------------------
