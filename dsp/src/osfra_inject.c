#include "osfra.h"

float32_t OSFRA_inject(float32_t ref)
{
	if(OSFRA_state != 0)
	{
		OSFRA_pointer = (OSFRA_pointer + OSFRA_step) & 0x3FFFFFFF;
		OSFRA_tableptr = &FPUsinTable[(OSFRA_pointer + 10) >> 21];
		OSFRA_reference = OSFRA_amplitude * (*OSFRA_tableptr) + ref;
		return OSFRA_reference;
	}
	else
	{
		return ref;
	}
}