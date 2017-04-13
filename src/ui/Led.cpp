/*
        ##########    Copyright (C) 2017 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "Led.h"

//--------------------------------------------------------------------------------------------------

#include "resources/Led.cpp"

//--------------------------------------------------------------------------------------------------

Led::Led()
{
  m_image = ImageCache::getFromMemory(::led_off_png, ::led_off_pngSize);
  setSize(20, 20);
}

//--------------------------------------------------------------------------------------------------

void Led::paint(Graphics& g)
{
  g.drawImageWithin(m_image, 0, 0, 20, 20, RectanglePlacement::centred, false);
}

//--------------------------------------------------------------------------------------------------

void Led::resized()
{
}

//--------------------------------------------------------------------------------------------------

void Led::setState(State state_, const unsigned timeoutMs_)
{
  stopTimer();

  switch (state_)
  {
    case State::blue:
      m_image = ImageCache::getFromMemory(led_blue_png, led_blue_pngSize);
      break;
    case State::green:
      m_image = ImageCache::getFromMemory(led_green_png, led_green_pngSize);
      break;
    case State::orange:
      m_image = ImageCache::getFromMemory(led_orange_png, led_orange_pngSize);
      break;
    case State::red:
      m_image = ImageCache::getFromMemory(led_red_png, led_red_pngSize);
      break;
    case State::off:
    default:
      m_image = ImageCache::getFromMemory(led_off_png, led_off_pngSize);
      break;
  }

  if (timeoutMs_ > 0 && state_ != State::off)
  {
    startTimer(timeoutMs_);
  }

  repaint();
}

//--------------------------------------------------------------------------------------------------
