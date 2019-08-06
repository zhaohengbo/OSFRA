#include "osfra.h"

/*
 *                       ---------- 
 *                      |          |
 *                 ---->|   H(s)   |--------
 *                |     |          |        |
 *                |      ----------         O<- Noise
 *           (PWM)|                         | (ADC)                Analog
 *= = = = =  = = =|= = = = = = = = = = = = =|= = = = = = = = = = = = = = =
 * Control Output |                         |Feedback              Digital
 *                |      ----------         |
 *                |     |          |  Error |(-)
 *                 -----|   G(z)   |<-------O<- Reference
 *                      |          |         (+)
 *                       ---------- 
 */

void OSFRA_BackgroundTask(OSFRA *OSFRA_obj)
{
	float32_t GH_1dA,GH_Q,GH_I,GH_Mag,GH_Phase;
	float32_t H_1dA,H_Q,H_I,H_Mag,H_Phase;
	float32_t CL_Q,CL_I,CL_Mag,CL_Phase;
	int16_t sample_cycle;
	if(OSFRA_obj->state == 4)
	{
		if(OSFRA_obj->storeGH == 1)
		{
			// GH = e^(theta_feedback) / e^(theta_error) 
			//    = e^(theta_feedback) / (e^(theta_reference) - e^(theta_feedback))
			// NOTE: OSFRA_rCosSum is almost zero!
			GH_1dA = 1.0 / ((OSFRA_yCosSum * OSFRA_yCosSum) + (((-OSFRA_rSinSum) - (-OSFRA_ySinSum)) * ((-OSFRA_rSinSum) - (-OSFRA_ySinSum))));
			GH_Q = -(OSFRA_yCosSum * (-OSFRA_ySinSum)) + (OSFRA_yCosSum * ((-OSFRA_rSinSum) - (-OSFRA_ySinSum)));
			GH_I = -(OSFRA_yCosSum * OSFRA_yCosSum) - ((-OSFRA_ySinSum) * ((-OSFRA_rSinSum) - (-OSFRA_ySinSum)));
			GH_Mag = logf(sqrtf((GH_1dA * GH_Q) * (GH_1dA * GH_Q) + (GH_1dA * GH_I) * (GH_1dA * GH_I)));
			GH_Phase = atan2f(GH_1dA * GH_Q,GH_1dA * GH_I);
			OSFRA_obj->gh_magVect[OSFRA_obj->freqIndex] = 8.68588924 * GH_Mag;
			OSFRA_obj->gh_phaseVect[OSFRA_obj->freqIndex] = 57.2957802 * GH_Phase;
		}
		if(OSFRA_obj->storeH == 1)
		{
			// H = e^(theta_control) / e^(theta_feedback) 
			H_I = ((-OSFRA_ySinSum) * (-OSFRA_uSinSum)) + (OSFRA_uCosSum * OSFRA_yCosSum);
			H_Q = (OSFRA_uCosSum * (-OSFRA_ySinSum)) - ((-OSFRA_uSinSum) * OSFRA_yCosSum);
			H_1dA = 1.0/((OSFRA_uCosSum * OSFRA_uCosSum) + ((-OSFRA_uSinSum) * (-OSFRA_uSinSum)));
			H_Mag = logf(sqrtf((H_I * H_1dA) * (H_I * H_1dA) + (H_Q * H_1dA) * (H_Q * H_1dA)));
			H_Phase = atan2f(H_Q * H_1dA,H_I * H_1dA);
			OSFRA_obj->h_magVect[OSFRA_obj->freqIndex] = 8.68588924 * H_Mag;
			OSFRA_obj->h_phaseVect[OSFRA_obj->freqIndex] = 57.2957802 * H_Phase;
		}
		if(OSFRA_obj->storeCL == 1)
		{
			// CL = e^(theta_reference) / e^(theta_feedback) 
			// NOTE: OSFRA_rCosSum is almost zero!
			CL_Q = -OSFRA_yCosSum/(-OSFRA_rSinSum);
			CL_I = -OSFRA_ySinSum/(-OSFRA_rSinSum);
			CL_Mag = logf(sqrtf(CL_Q * CL_Q + CL_I * CL_I));
			CL_Phase = atan2f(CL_Q,CL_I);
			OSFRA_obj->cl_magVect[OSFRA_obj->freqIndex] = 8.68588924 * CL_Mag;
			OSFRA_obj->cl_phaseVect[OSFRA_obj->freqIndex] = 57.2957802 * CL_Phase;
		}
		OSFRA_obj->state = 5;
		OSFRA_stateSlew = 5;
	}
	else if(OSFRA_obj->state == 5)
	{
		OSFRA_stateSlew--;
		if(OSFRA_stateSlew == 0)
		{
			OSFRA_obj->state = 1;
			OSFRA_obj->freqIndex++;
		}
	}
	else if(OSFRA_obj->state == 3)
	{
		if(OSFRA_state != 1)
		{
			OSFRA_obj->state = 4;
		}
	}
	else if(OSFRA_obj->state == 0)
	{
		if(OSFRA_obj->start == 1)
		{
			OSFRA_obj->start = 0;
			OSFRA_obj->status = 1;
			OSFRA_obj->state = 1;
			OSFRA_obj->freqIndex = 0;
		}
	}
	else if(OSFRA_obj->state == 1)
	{
		if(OSFRA_obj->freqIndex < OSFRA_obj->vecLength)
		{
			OSFRA_step = 2097152.0 * ((512.0 * OSFRA_obj->freqVect[OSFRA_obj->freqIndex]) / OSFRA_obj->isrFreq);
			OSFRA_pointer = 0;
			//OSFRA_preCount = 5 * (OSFRA_obj->isrFreq / OSFRA_obj->freqVect[OSFRA_obj->freqIndex]);
			OSFRA_preCount = (OSFRA_obj->isrFreq / OSFRA_obj->freqVect[OSFRA_obj->freqIndex]);
			if(OSFRA_preCount > 0x7FFF)
			{
				OSFRA_preCount = 0x7FFF;
			}
			if(OSFRA_obj->freqVect[OSFRA_obj->freqIndex] < 10.0)
			{
				sample_cycle = 5;
			}
			else if(OSFRA_obj->freqVect[OSFRA_obj->freqIndex] < 100.0)
			{
				sample_cycle = 10;
			}
			else if(OSFRA_obj->freqVect[OSFRA_obj->freqIndex] < 1000.0)
			{
				sample_cycle = 100;
			}
			else
			{
				sample_cycle = 200;
			}
			//OSFRA_count = OSFRA_preCount * sample_cycle * OSFRA_obj->speed;
			//if(OSFRA_count > 0x7FFF)
			//{
			//	OSFRA_count = 0x7FFF;
			//}
			OSFRA_count = OSFRA_preCount * sample_cycle * OSFRA_obj->speed - 20;
			OSFRA_obj->state = 2;
			OSFRA_rSinSum = 0;
			OSFRA_rCosSum = 0;
			OSFRA_uSinSum = 0;
			OSFRA_uCosSum = 0;
			OSFRA_ySinSum = 0;
			OSFRA_yCosSum = 0;
			OSFRA_windowSamples = 0;
			OSFRA_state = 0x2;
		}
		else
		{
			OSFRA_obj->status = 0;
			OSFRA_obj->state = 0;
		}
	}
	else if(OSFRA_obj->state == 2)
	{
		if(OSFRA_state != 2)
		{
			OSFRA_obj->state = 3;
		}
	}
}
