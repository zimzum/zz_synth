//=============================================================
// FILE:
//   src/unit_tests/read_write_wav/testbench.c
//
// AUTHOR:
//   zimzum@github 
//
// DESCRIPTION:
//   Tests reading and writing WAVE files.
//
// License: GNU GPL v2.0 
//=============================================================

//=============================================================
// Includes
//=============================================================
#include <cmath>
#include <iostream>
#include <string>

#include "common/zz_global.h"
#include "common/wave_file.h"
#include "common/synth_config.h"
#include <vector>

#include <gtest/gtest.h>

//=============================================================
// Tests
//=============================================================
TEST(ReadWriteWaveFileTest, HandleDifferentPitches )
{
    int pitch = 48;  // Middle C
    int32_t volume = 1 << 15;
    uint32_t total_number_of_samples;
    double frequency, phase_increment, phase;
    int8_t number_of_seconds = 5;
    const std::string file_name("example01.wav");

    // 1. Initialise the synthesiser
    zz_SynthConfig &synthesiser  = zz_SynthConfig::getInstance();
    synthesiser.Init();	

    // 0. General test setting
    frequency = synthesiser.frequency_table(pitch);

    phase_increment = synthesiser.phase_increment_per_sample() * frequency;
    phase = 0;
    total_number_of_samples = 
        (std::size_t) ((synthesiser.sampling_rate() * number_of_seconds) + 0.5);


    // 2. The output buffer and the output file
    WaveFileOut wf_out(number_of_seconds);

    std::vector<int16_t> samples_out(total_number_of_samples);
    for (std::size_t n = 0; n < total_number_of_samples; n++)
    {
        samples_out[n] = volume * sin(phase);

        if ((phase += phase_increment) >= kTwoPi)
            phase -= kTwoPi;
    }

    wf_out.SaveBufferToFile(file_name, samples_out);
    
    // 3. The input buffer and the input file
    std::vector<int16_t> samples_in;
    WaveFileIn wf_in;
    samples_in = wf_in.ReadBufferFromFile(file_name);

    // 4. The input and output files ought to be indentical. Verify that.
    EXPECT_EQ(wf_out.chunk_id(), wf_in.chunk_id());
    EXPECT_EQ(wf_out.chunk_size(), wf_in.chunk_size());
    EXPECT_EQ(wf_out.format(), wf_in.format());
    EXPECT_EQ(wf_out.subchunk_1_id(), wf_in.subchunk_1_id());
    EXPECT_EQ(wf_out.subchunk_1_size(), wf_in.subchunk_1_size());
    EXPECT_EQ(wf_out.audio_format(), wf_in.audio_format());
    EXPECT_EQ(wf_out.num_channels(), wf_in.num_channels());
    EXPECT_EQ(wf_out.sample_rate(), wf_in.sample_rate());
    EXPECT_EQ(wf_out.byte_rate(), wf_in.byte_rate());
    EXPECT_EQ(wf_out.block_align(), wf_in.block_align());
    EXPECT_EQ(wf_out.bits_per_sample(), wf_in.bits_per_sample());
    EXPECT_EQ(wf_out.subchunk_2_id(), wf_in.subchunk_2_id());
    EXPECT_EQ(wf_out.subchunk_2_size(), wf_in.subchunk_2_size());
    EXPECT_EQ(wf_out.subchunk_2_size(), wf_in.subchunk_2_size());
    //EXPECT_THAT(samples_in, ::testing::ContainerEq(samples_out));

}

//=============================================================
// Main 
//=============================================================
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
