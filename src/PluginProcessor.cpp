/*
        ##########    Copyright (C) 2017 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <string>

//--------------------------------------------------------------------------------------------------

ThrobAudioProcessor::ThrobAudioProcessor()
  : m_clockResolution(ClockResolution::ppqn1), m_playing(false), m_looping(false)
{
  initClockMultipliers();
}

//--------------------------------------------------------------------------------------------------

const String ThrobAudioProcessor::getName() const
{
  return "Throb";
}

//--------------------------------------------------------------------------------------------------

bool ThrobAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

//--------------------------------------------------------------------------------------------------

bool ThrobAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

//--------------------------------------------------------------------------------------------------

double ThrobAudioProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

//--------------------------------------------------------------------------------------------------

int ThrobAudioProcessor::getNumPrograms()
{
  return 1;
}

//--------------------------------------------------------------------------------------------------

int ThrobAudioProcessor::getCurrentProgram()
{
  return 0;
}

//--------------------------------------------------------------------------------------------------

void ThrobAudioProcessor::setCurrentProgram(int index)
{
}

//--------------------------------------------------------------------------------------------------

const String ThrobAudioProcessor::getProgramName(int index)
{
  return String();
}

//--------------------------------------------------------------------------------------------------

void ThrobAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//--------------------------------------------------------------------------------------------------

void ThrobAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
}

//--------------------------------------------------------------------------------------------------

void ThrobAudioProcessor::releaseResources()
{
}

//--------------------------------------------------------------------------------------------------

#ifndef JucePlugin_PreferredChannelConfigurations
bool ThrobAudioProcessor::setPreferredBusArrangement(
  bool isInput, int bus, const AudioChannelSet& preferredSet)
{
  const int numChannels = preferredSet.size();
  if (isInput || (numChannels != 1 && numChannels != 2))
    return false;

  return AudioProcessor::setPreferredBusArrangement(isInput, bus, preferredSet);
}
#endif

//--------------------------------------------------------------------------------------------------

void ThrobAudioProcessor::processBlock(AudioSampleBuffer& buffer_, MidiBuffer& /*mm_*/)
{
  using namespace std::chrono;
  AudioPlayHead::CurrentPositionInfo result;
  auto playHead = getPlayHead();
  if (playHead == nullptr)
  {
    m_playing = false;
    m_looping = false;
    return;
  }
  playHead->getCurrentPosition(result);

  static const auto resetDuration = duration<double>{0.02};

  const auto microsPerSample = 1e6 / getSampleRate();
  if (!result.isPlaying)
  {
    m_playing = false;
    m_looping = false;
    m_lastClick.zero();
    m_lastReset.zero();
    return;
  }

  m_playing = true;
  m_looping = result.isLooping;

  double pulseWrapFactor
    = 2.0 / (result.timeSigDenominator * static_cast<unsigned>(m_clockResolution));
  pulseWrapFactor /= m_clockMultipliers[m_currentClockMultiplier].value;

  auto clickDuration = duration<double>{(30.0 / result.bpm) * pulseWrapFactor};

  for (int i = 0; i < getBlockSize(); ++i)
  {
    float clockAmplitude = -1.;
    float resetAmplitude = -1.;

    auto now = std::chrono::nanoseconds(
      (unsigned long long)(((result.timeInSamples + i) * microsPerSample) * 1000));

    if (now - m_lastClick >= clickDuration * 2 || now < m_lastClick)
    {
      m_lastClick = now;
    }

    if (now - m_lastClick < clickDuration)
    {
      clockAmplitude = 1.0;
    }
    if (m_lastPpqPosition > result.ppqPosition)
    {
      m_lastReset = now;
    }
    m_lastPpqPosition = result.ppqPosition;

    if (now - m_lastReset < resetDuration)
    {
      resetAmplitude = 1.0;
    }

    buffer_.getWritePointer(0)[i] = clockAmplitude;
    if(buffer_.getNumChannels() > 1)
    {
      buffer_.getWritePointer(1)[i] = resetAmplitude;
    }
  }
}

//--------------------------------------------------------------------------------------------------

bool ThrobAudioProcessor::hasEditor() const
{
  return true;
}

//--------------------------------------------------------------------------------------------------

AudioProcessorEditor* ThrobAudioProcessor::createEditor()
{
  return new ThrobAudioProcessorEditor(*this);
}

//--------------------------------------------------------------------------------------------------

void ThrobAudioProcessor::getStateInformation(MemoryBlock& destData)
{
  XmlElement xml("Throb");
  xml.setAttribute("ppqn", std::to_string(static_cast<unsigned>(m_clockResolution)));
  xml.setAttribute("multiplier", std::to_string(static_cast<unsigned>(m_currentClockMultiplier)));
  xml.setAttribute("version", "1.0");
  copyXmlToBinary(xml, destData);
}

//--------------------------------------------------------------------------------------------------

void ThrobAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
  // You should use this method to restore your parameters from this memory
  // block,
  // whose contents will have been created by the getStateInformation() call.
  ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
  if (xmlState != 0)
  {
    if (xmlState->hasTagName("Throb"))
    {
      m_clockResolution = static_cast<ClockResolution>(xmlState->getIntAttribute("ppqn", 0));
      m_currentClockMultiplier
        = static_cast<unsigned>(xmlState->getIntAttribute("multiplier", m_defaultClockMultiplier));
    }
  }
}

//--------------------------------------------------------------------------------------------------

void ThrobAudioProcessor::initClockMultipliers()
{
  m_clockMultipliers.clear();

  m_clockMultipliers.emplace_back(1 / 64., "1/64");
  m_clockMultipliers.emplace_back(1 / 32., "1/32");
  m_clockMultipliers.emplace_back(1 / 16., "1/16");
  m_clockMultipliers.emplace_back(1 / 8., "1/8");
  m_clockMultipliers.emplace_back(1 / 4., "1/4");
  m_clockMultipliers.emplace_back(1 / 3., "1/3");
  m_clockMultipliers.emplace_back(1 / 2., "1/2");
  m_clockMultipliers.emplace_back(1., "1");
  m_clockMultipliers.emplace_back(2., "2");
  m_clockMultipliers.emplace_back(3., "3");
  m_clockMultipliers.emplace_back(4., "4");
  m_clockMultipliers.emplace_back(5., "5");
  m_clockMultipliers.emplace_back(6., "6");
  m_clockMultipliers.emplace_back(7., "7");
  m_clockMultipliers.emplace_back(8., "8");
  m_clockMultipliers.emplace_back(9., "9");
  m_clockMultipliers.emplace_back(10., "10");
  m_clockMultipliers.emplace_back(11., "11");
  m_clockMultipliers.emplace_back(12., "12");
  m_clockMultipliers.emplace_back(13., "13");
  m_clockMultipliers.emplace_back(14., "14");
  m_clockMultipliers.emplace_back(15., "15");
  m_clockMultipliers.emplace_back(16., "16");
  m_clockMultipliers.emplace_back(32., "32");
  m_clockMultipliers.emplace_back(64., "64");
  m_clockMultipliers.emplace_back(128., "128");

  for (unsigned i = 0; i < m_clockMultipliers.size(); ++i)
  {
    if (m_clockMultipliers[i].value == 1.)
    {
      m_defaultClockMultiplier = i;
    }
  }

  m_currentClockMultiplier = m_defaultClockMultiplier;
}

//--------------------------------------------------------------------------------------------------

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
  return new ThrobAudioProcessor();
}

//--------------------------------------------------------------------------------------------------
