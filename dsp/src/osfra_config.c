#include "osfra.h"

void OSFRA_updateInjectionAmplitude(OSFRA *OSFRA_obj, float32_t new_injection_amplitude)
{
	OSFRA_amplitude = new_injection_amplitude;
	OSFRA_obj->amplitude = new_injection_amplitude;
}

void OSFRA_resetFreqRespArray(OSFRA *OSFRA_obj)
{
	int16_t i;
	if(OSFRA_obj->vecLength > 0)
	{
		for(i = 0;i < OSFRA_obj->vecLength;i++)
		{
			if(OSFRA_obj->storeH == 1)
			{
				OSFRA_obj->h_magVect[i] = 0.0;
				OSFRA_obj->h_phaseVect[i] = 0.0;
			}
			if(OSFRA_obj->storeGH == 1)
			{
				OSFRA_obj->gh_magVect[i] = 0.0;
				OSFRA_obj->gh_phaseVect[i] = 0.0;
			}
			if(OSFRA_obj->storeCL == 1)
			{
				OSFRA_obj->cl_magVect[i] = 0.0;
				OSFRA_obj->cl_phaseVect[i] = 0.0;
			}
		}
	}
}

void OSFRA_reset(OSFRA *OSFRA_obj)
{
	OSFRA_obj->start = 0;
	OSFRA_obj->state = 0;
	OSFRA_obj->freqIndex = 0;
	OSFRA_obj->status = 0;
	OSFRA_rSinSum = 0;
	OSFRA_rCosSum = 0;
	OSFRA_uSinSum = 0;
	OSFRA_yCosSum = 0;
	OSFRA_tableptr = 0;
	OSFRA_tableptrStart = 0;
	OSFRA_reference = 0;
	OSFRA_step = 0;
    OSFRA_pointer = 0;
    OSFRA_state = 0;
    OSFRA_preCount = 0;
    OSFRA_count = 0;
    OSFRA_windowSamples = 0;
    OSFRA_stateSlew = 0;
}

void OSFRA_initFreqArrayWithLogSteps(OSFRA *OSFRA_obj, float32_t fra_sweep_start_freq, float32_t freqStep)
{
	int16_t i;
	OSFRA_obj->freqVect[0] = fra_sweep_start_freq;
	OSFRA_obj->freqStart = fra_sweep_start_freq;
	OSFRA_obj->freqStep = freqStep;
	if(OSFRA_obj->vecLength >= 2)
	{
		for(i = 1;i < OSFRA_obj->vecLength;i++)
		{
			OSFRA_obj->freqVect[i] = OSFRA_obj->freqVect[i - 1] + OSFRA_obj->freqStep;
		}
	}
}

void OSFRA_config(OSFRA *OSFRA_obj,
                  float32_t isrFrequency,
                  float32_t injectionAmplitude,
                  int16_t noFreqPoints,
                  float32_t fraSweepStartFreq,
                  float32_t freqStep,
                  float32_t *h_magVect,
                  float32_t *h_phaseVect,
                  float32_t *gh_magVect,
                  float32_t *gh_phaseVect,
                  float32_t *cl_magVect,
                  float32_t *cl_phaseVect,
                  float32_t *freqVect,
                  int16_t speed)
{
	OSFRA_obj->h_magVect = h_magVect;
	OSFRA_obj->h_phaseVect = h_phaseVect;
	OSFRA_obj->gh_magVect = gh_magVect;
	OSFRA_obj->gh_phaseVect = gh_phaseVect;
	OSFRA_obj->cl_magVect = cl_magVect;
	OSFRA_obj->cl_phaseVect = cl_phaseVect;
	OSFRA_obj->freqVect = freqVect;
	OSFRA_obj->isrFreq = isrFrequency;
	OSFRA_obj->amplitude = injectionAmplitude;
	OSFRA_amplitude = injectionAmplitude;
	OSFRA_obj->freqStart = fraSweepStartFreq;
	OSFRA_obj->freqStep = freqStep;
	OSFRA_obj->vecLength = noFreqPoints;
	if((h_magVect != NULL) && (h_phaseVect != NULL))
	{
		OSFRA_obj->storeH = 1;
	}
	if((gh_magVect != NULL) && (gh_phaseVect != NULL))
	{
		OSFRA_obj->storeGH = 1;
	}
	if((cl_magVect != NULL) && (cl_phaseVect != NULL))
	{
		OSFRA_obj->storeCL = 1;
	}
	if(speed <= 0)
	{
		OSFRA_obj->speed = 1;
	}
	else
	{
		OSFRA_obj->speed = speed;
	}
}
