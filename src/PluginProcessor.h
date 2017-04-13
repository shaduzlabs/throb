/*
        ##########    Copyright (C) 2017 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <atomic>
#include <chrono>

#include "JuceHeader.h"

//--------------------------------------------------------------------------------------------------

class ThrobAudioProcessor final : public AudioProcessor
{
public:
  enum class ClockResolution
  {
    ppqn1 = 1,
    ppqn8 = 8,
    ppqn24 = 24,
  };

  ThrobAudioProcessor();
  ~ThrobAudioProcessor() = default;

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool setPreferredBusArrangement(
    bool isInput, int bus, const AudioChannelSet& preferredSet) override;
#endif

  void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  const String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  double getTailLengthSeconds() const override;

  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const String getProgramName(int index) override;
  void changeProgramName(int index, const String& newName) override;

  void getStateInformation(MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  void setClockResolution(ClockResolution cr_)
  {
    m_clockResolution = cr_;
  }
  ClockResolution clockResolution() const
  {
    return m_clockResolution;
  }

  bool playing() const
  {
    return m_playing.load();
  }
  bool looping() const
  {
    return m_looping.load();
  }

  size_t numClockMultipliers() const
  {
    return m_clockMultipliers.size();
  }

  String clockMultiplierText() const
  {
    return m_clockMultipliers[m_currentClockMultiplier].label;
  }

  int getNumParameters() override
  {
    return 1;
  }
  const String getParameterName(int parameterIndex) override
  {
    return "Multiplier";
  }
  float getParameter(int parameterIndex) override
  {
    return static_cast<unsigned>(m_currentClockMultiplier)
           / static_cast<float>(m_clockMultipliers.size());
  }
  const String getParameterText(int parameterIndex) override
  {
    return m_clockMultipliers[m_currentClockMultiplier].label;
  }
  int getParameterNumSteps(int parameterIndex) override
  {
    return m_clockMultipliers.size();
  }
  float getParameterDefaultValue(int parameterIndex) override
  {
    return static_cast<float>(m_defaultClockMultiplier);
  }
  bool isParameterAutomatable(int index) const override
  {
    return true;
  }
  void setParameter(int parameterIndex, float newValue) override
  {
    unsigned multiplierIndex = static_cast<unsigned>(newValue * m_clockMultipliers.size());
    if (parameterIndex != 0 || multiplierIndex >= m_clockMultipliers.size())
    {
      return;
    }
    m_currentClockMultiplier = multiplierIndex;
  }

private:
  struct ClockMultiplier
  {
    ClockMultiplier(double v, std::string l) : value(v), label(std::move(l))
    {
    }

    double value;
    std::string label;
  };

  using ClockMultipliers = std::vector<ClockMultiplier>;

  void initClockMultipliers();

  AudioPlayHead::CurrentPositionInfo m_currentPositionInfo;

  ClockResolution m_clockResolution;
  double m_lastPpqPosition{0.};
  std::chrono::nanoseconds m_lastClick{};
  std::chrono::nanoseconds m_lastReset{};

  std::atomic_bool m_playing;
  std::atomic_bool m_looping;

  ClockMultipliers m_clockMultipliers;
  unsigned m_defaultClockMultiplier;
  unsigned m_currentClockMultiplier;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThrobAudioProcessor)
};

//--------------------------------------------------------------------------------------------------
