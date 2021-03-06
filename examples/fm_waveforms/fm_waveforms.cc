//========================================================================
// FILE:
//   examples/fm_waveforms/fm_waveforms.cc
//
// AUTHOR:
//   zimzum@github
//
// DESCRIPTION:
//   This file demonstrates how to generate frequency modulated waveforms
//   and save them to WAVE files using the zz-synthesiser. A number of files
//   with different modulator frequency and index of modulation are
//   are generated. There are six hard-coded  parameters that determine
//   the sounds being produced. These are marked with 'Param #:' and
//   can be freely modified to experiment.
//
// License: GNU GPL v2.0
//========================================================================

#include <common/synth_config.h>
#include <common/wave_file.h>
#include <fm_synthesiser/fm_synthesiser.h>
#include <global/global_variables.h>

using namespace std;

//========================================================================
// MAIN
//========================================================================
int main() {
  // Param 1: Carrier's Pitch
  size_t pitch_carrier = 64;
  // Param 2: Modulator's Pitch
  vector<size_t> pitch_modulator = {10, 20, 30, 40,  50,  60,
                                    70, 80, 90, 100, 110, 120};
  // Param 3: Index of modulation
  vector<double> index_of_modulation = {1 << 3, 1 << 5, 1 << 8};
  // Param 4: Volume
  int16_t volume = 1 << 14;
  // Param 5: Duration
  uint32_t duration = 5;
  // Param 6: Initial phase
  double initial_phase = 0;

  uint32_t file_idx = 0;
  char file_name[200];

  // Initialise the synthesiser
  SynthConfig &synthesiser = SynthConfig::getInstance();
  synthesiser.Init();

  for (auto it : pitch_modulator) {
    for (auto it2 : index_of_modulation) {
      // 1. Generate filename. Note that filename contains the
      //    index of modulation the pitch of the modulator.
      sprintf(file_name,
              "examples/fm_waveforms/sounds/fm_wave_pitch_id_%lu_iom_%lu.wav",
              it, static_cast<size_t>(it2));

      // 2. Create the FM synthesiser and generate the samples
      FmSynthesiser fm_synthesiser(synthesiser, volume, initial_phase,
                                   pitch_carrier, it, it2);
      vector<int16_t> samples_output = fm_synthesiser(duration);

      // 3. Save the samples to the file
      WaveFileOut wf_out_sine(duration);
      wf_out_sine.SaveBufferToFile(file_name, samples_output);
    }
  }

  return 0;
}
