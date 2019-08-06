#include "osfra.h"

void OSFRA_collect(float32_t *control_output, float32_t *feedback)
{
	if(OSFRA_state != 0)
	{
		if(OSFRA_state == 1)
		{
			OSFRA_rSinSum += OSFRA_tableptr[0] * OSFRA_reference;
			OSFRA_rCosSum += OSFRA_tableptr[256] * OSFRA_reference;
			OSFRA_uSinSum += OSFRA_tableptr[0] * (*control_output);
			OSFRA_uCosSum += OSFRA_tableptr[256] * (*control_output);
			OSFRA_ySinSum += OSFRA_tableptr[0] * (*feedback);
			OSFRA_yCosSum += OSFRA_tableptr[256] * (*feedback);
			OSFRA_count--;
			if(OSFRA_count < 0)
			{
				OSFRA_windowSamples++;
				if(abs(OSFRA_tableptr - OSFRA_tableptrStart) == 0)
				{
					OSFRA_state = 0;
				}
			}
		}
		else
		{
			OSFRA_preCount--;
			if(OSFRA_preCount < 0)
			{
				OSFRA_tableptrStart = OSFRA_tableptr;
				OSFRA_state = 1;
			}
		}
	}
}
