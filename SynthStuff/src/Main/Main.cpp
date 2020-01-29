#include "pch.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "DSP/SynthDefs.h"
#include "DSP/WavFile/WaveFile.h"

int main(int argc, char** argv)
{
    std::cout << "Hello World!\n"; 
	int pitch = 68;  // Middle C
	FrqValue duration = 5;
	AmpValue volume = 0.5;

	InitSynthesizer();
	FrqValue frequency = synthParams.GetFrequency(pitch);

	PhsAccum phaseIncr = synthParams.frqRad * frequency;
	PhsAccum phase = 0;

	long totalSamples = (long) ((synthParams.sampleRate * duration) + 0.5);

	WaveFile wf;
	if (wf.OpenWaveFile("Output/Test/example01.wav", 1))
	{
		printf("Cannot open wavefile for output\n");
		exit(1);
	}

	for (long n = 0; n < totalSamples; n++)
	{
		wf.Output1(volume * sinv(phase));
		if ((phase += phaseIncr) >= twoPI)
			phase -= twoPI;
	}

	wf.CloseWaveFile();

	return 0;
}
