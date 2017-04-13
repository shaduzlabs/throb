/*
        ##########    Copyright (C) 2017 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "ThrobGUI.h"

//--------------------------------------------------------------------------------------------------

#include "resources/ThrobGUI.cpp"

//--------------------------------------------------------------------------------------------------

ThrobGUI::ThrobGUI(ThrobAudioProcessor& ap_) : m_audioProcessor(ap_)
{
  m_knobStrip = ImageCache::getFromMemory(::knobstrip_png, ::knobstrip_pngSize);
  m_knobStripHover = ImageCache::getFromMemory(::knobhover_png, ::knobhover_pngSize);

  addAndMakeVisible(m_btnPpqn1 = new ImageButton("4 ppqn"));
  m_btnPpqn1->setTooltip(TRANS("4 ppqn"));
  m_btnPpqn1->setRadioGroupId(1);
  m_btnPpqn1->addListener(this);

  m_btnPpqn1->setImages(false,
    true,
    true,
    ImageCache::getFromMemory(squarebtnnormal_png, squarebtnnormal_pngSize),
    0.800f,
    Colour(0x00000000),
    ImageCache::getFromMemory(squarebtnnormal_png, squarebtnnormal_pngSize),
    0.800f,
    Colour(0x00000000),
    ImageCache::getFromMemory(squarebtndown_png, squarebtndown_pngSize),
    1.000f,
    Colour(0x00000000));
  addAndMakeVisible(m_btnPpqn8 = new ImageButton("32 ppqn"));
  m_btnPpqn8->setTooltip(TRANS("32 ppqn"));
  m_btnPpqn8->setRadioGroupId(1);
  m_btnPpqn8->addListener(this);

  m_btnPpqn8->setImages(false,
    true,
    true,
    ImageCache::getFromMemory(squarebtnnormal_png, squarebtnnormal_pngSize),
    0.800f,
    Colour(0x00000000),
    ImageCache::getFromMemory(squarebtnnormal_png, squarebtnnormal_pngSize),
    0.800f,
    Colour(0x00000000),
    ImageCache::getFromMemory(squarebtndown_png, squarebtndown_pngSize),
    1.000f,
    Colour(0x00000000));
  addAndMakeVisible(m_btnPpqn24 = new ImageButton("96 ppqn"));
  m_btnPpqn24->setRadioGroupId(1);
  m_btnPpqn24->addListener(this);

  m_btnPpqn24->setImages(false,
    true,
    true,
    ImageCache::getFromMemory(squarebtnnormal_png, squarebtnnormal_pngSize),
    0.800f,
    Colour(0x00000000),
    ImageCache::getFromMemory(squarebtnnormal_png, squarebtnnormal_pngSize),
    0.800f,
    Colour(0x00000000),
    ImageCache::getFromMemory(squarebtndown_png, squarebtndown_pngSize),
    1.000f,
    Colour(0x00000000));
  addAndMakeVisible(m_ledActive = new Led());
  addAndMakeVisible(m_ledLooping = new Led());
  addAndMakeVisible(m_knob = new GraphicKnob(m_knobStrip, 128, false));
  addAndMakeVisible(m_lblMultiplier = new Label("new label", TRANS("1")));
  m_lblMultiplier->setFont(Font(11.20f, Font::plain));
  m_lblMultiplier->setJustificationType(Justification::centred);
  m_lblMultiplier->setEditable(false, false, false);
  m_lblMultiplier->setColour(Label::textColourId, Colour(0x8dffffff));
  m_lblMultiplier->setColour(TextEditor::textColourId, Colours::black);
  m_lblMultiplier->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

  m_backgroundImage = ImageCache::getFromMemory(throb_png, throb_pngSize);

  m_knob->setRange(0., 1.0, 1. / m_audioProcessor.numClockMultipliers());
  m_knob->setValue(static_cast<double>(m_audioProcessor.getParameter(0)));
  m_knob->setImageBackgroundHover(&m_knobStripHover);
  m_knob->addListener(this);

  m_btnPpqn1->setClickingTogglesState(true);
  m_btnPpqn8->setClickingTogglesState(true);
  m_btnPpqn24->setClickingTogglesState(true);

  switch (m_audioProcessor.clockResolution())
  {
    case ThrobAudioProcessor::ClockResolution::ppqn24:
    {
      m_btnPpqn24->setToggleState(true, NotificationType::sendNotification);
      break;
    }
    case ThrobAudioProcessor::ClockResolution::ppqn8:
    {
      m_btnPpqn8->setToggleState(true, NotificationType::sendNotification);
      break;
    }
    case ThrobAudioProcessor::ClockResolution::ppqn1:
    default:
    {
      m_btnPpqn1->setToggleState(true, NotificationType::sendNotification);
    }
  }
  setSize(300, 200);
}

ThrobGUI::~ThrobGUI()
{
  m_btnPpqn1 = nullptr;
  m_btnPpqn8 = nullptr;
  m_btnPpqn24 = nullptr;
  m_ledActive = nullptr;
  m_ledLooping = nullptr;
  m_knob = nullptr;
  m_lblMultiplier = nullptr;
}

//--------------------------------------------------------------------------------------------------

void ThrobGUI::paint(Graphics& g)
{
  m_ledActive->setState(m_audioProcessor.playing() ? Led::State::green : Led::State::off);
  m_ledLooping->setState(m_audioProcessor.looping() ? Led::State::orange : Led::State::off);
  auto cm = m_audioProcessor.getParameter(0);
  if (static_cast<float>(cm) != m_knob->getValue())
  {
    m_knob->setValue(static_cast<float>(cm), NotificationType::dontSendNotification);
    m_lblMultiplier->setText(
      m_audioProcessor.clockMultiplierText(), NotificationType::dontSendNotification);
  }

  g.fillAll(Colours::white);

  g.setColour(Colours::black);
  g.drawImageWithin(m_backgroundImage, 0, 0, 300, 200, RectanglePlacement::centred, false);
}

//--------------------------------------------------------------------------------------------------

void ThrobGUI::resized()
{
  m_btnPpqn1->setBounds(213, 64, 34, 34);
  m_btnPpqn8->setBounds(213, 94, 34, 34);
  m_btnPpqn24->setBounds(212, 124, 34, 34);
  m_ledActive->setBounds(18, 94, 20, 20);
  m_ledLooping->setBounds(18, 121, 20, 20);
  m_knob->setBounds(110, 73, 80, 80);
  m_lblMultiplier->setBounds(110, 150, 80, 24);
}

//--------------------------------------------------------------------------------------------------

void ThrobGUI::buttonClicked(Button* buttonThatWasClicked)
{
  if (buttonThatWasClicked == m_btnPpqn1)
  {
    m_btnPpqn1->setState(juce::Button::ButtonState::buttonDown);
    m_btnPpqn8->setState(juce::Button::ButtonState::buttonNormal);
    m_btnPpqn24->setState(juce::Button::ButtonState::buttonNormal);
    m_audioProcessor.setClockResolution(ThrobAudioProcessor::ClockResolution::ppqn1);
  }
  else if (buttonThatWasClicked == m_btnPpqn8)
  {
    m_btnPpqn1->setState(juce::Button::ButtonState::buttonNormal);
    m_btnPpqn8->setState(juce::Button::ButtonState::buttonDown);
    m_btnPpqn24->setState(juce::Button::ButtonState::buttonNormal);
    m_audioProcessor.setClockResolution(ThrobAudioProcessor::ClockResolution::ppqn8);
  }
  else if (buttonThatWasClicked == m_btnPpqn24)
  {
    m_btnPpqn1->setState(juce::Button::ButtonState::buttonNormal);
    m_btnPpqn8->setState(juce::Button::ButtonState::buttonNormal);
    m_btnPpqn24->setState(juce::Button::ButtonState::buttonDown);
    m_audioProcessor.setClockResolution(ThrobAudioProcessor::ClockResolution::ppqn24);
  }
}

//--------------------------------------------------------------------------------------------------

void ThrobGUI::sliderValueChanged(Slider* sliderThatWasMoved)
{
  if (sliderThatWasMoved == m_knob)
  {
    m_audioProcessor.setParameterNotifyingHost(0, sliderThatWasMoved->getValue());
    m_lblMultiplier->setText(
      m_audioProcessor.clockMultiplierText(), NotificationType::dontSendNotification);
  }
}

//--------------------------------------------------------------------------------------------------
