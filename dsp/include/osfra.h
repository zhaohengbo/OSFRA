#ifndef OSFRA_H
#define OSFRA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <typedef>
#include <math.h>

typedef float float32_t;

typedef struct{
    float32_t *h_magVect;     //!< Plant Mag SFRA Vector
    float32_t *h_phaseVect;   //!< Plant Phase SFRA Vector
    float32_t *gh_magVect;    //!< Open Loop Mag SFRA Vector
    float32_t *gh_phaseVect;  //!< Open Loop Phase SFRA Vector
    float32_t *cl_magVect;    //!< Closed Loop Mag SFRA Vector
    float32_t *cl_phaseVect;  //!< Closed Loop Phase SFRA Vector
    float32_t *freqVect;      //!< Frequency Vector
    float32_t amplitude;      //!< Injection Amplitude
    float32_t isrFreq;        //!< SFRA ISR frequency
    float32_t freqStart;      //!< Start frequency of SFRA sweep
    float32_t freqStep;       //!< Log space between frequency points (optional)
    int16_t start;            //!< Command to start SFRA
    int16_t state;            //!< State of SFRA
    int16_t status;           //!< Status of SFRA
    int16_t vecLength;        //!< No. of Points in the SFRA
    int16_t freqIndex;        //!< Index of the frequency vector
    int16_t storeH;           //!< Flag to indicate if H vector is stored
    int16_t storeGH;          //!< Flag to indicate if GH vector is stored
    int16_t storeCL;          //!< Flag to indicate if CL vector is stored
    int16_t speed;            //!< variable to change the speed of the sweep
}OSFRA;

//! \brief               Resets internal data of OSFRA module
//! \param OSFRA_obj    Pointer to the OSFRA structure
//! \return None
//!
extern void OSFRA_reset(OSFRA *OSFRA_obj);

//! \brief Configures the OSFRA module
//! \param *OSFRA_obj  Pointer to the OSFRA structure
//! \param isrFrequency  Frequency at which SFRA routine in called,
//!                      typically the control ISR rate
//! \param injectionAmplitude  Per Unit (PU) injection amplitude
//! \param noFreqPoints  Number of frequency points sweeped
//! \param fraSweepStartFreq  Start frequency of SFRA sweep
//! \param freqStep  Multiplier used to keep frequency points log step apart
//! \param *h_magVect  Pointer to array that stores plant FRA magnitude data
//! \param *h_phaseVect  Pointer to array that stores plant FRA phase data
//! \param *gh_magVect  Pointer to array that stores OL FRA magnitude data
//! \param *gh_phaseVect  Pointer to array that stores OL FRA phase data
//! \param *cl_magVect  Pointer to array that stores OL FRA magnitude data
//! \param *cl_phaseVect  Pointer to array that stores OL FRA phase data
//! \param *freqVect Pointer to array that stores the freq points for the sweep
//! \param speed indiactes the speed of the sweep
//! \return None
//!
extern void OSFRA_config(OSFRA *OSFRA_obj,
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
                     int16_t speed);

//! \brief Initailizes the freq vectors with points that are log step apart
//! \param *OSFRA_obj  Pointer to the OSFRA structure
//! \param fra_sweep_start_freq  Start frequency of SFRA sweep
//! \param freqStep  Multiplier used to keep frequency points log step apart
//! \return None
//!
extern void OSFRA_initFreqArrayWithLogSteps(OSFRA *OSFRA_obj,
                                        float32_t fra_sweep_start_freq,
                                        float32_t freqStep);

//! \brief Resets the response data stored in the ol and plant
//!        phase and mag vector
//! \param *OSFRA_obj  Pointer to the OSFRA structure pointer
//! \return None
//!
extern void OSFRA_resetFreqRespArray(OSFRA *OSFRA_obj);

//! \brief Updates injection amplitude
//! \param *OSFRA_obj  Pointer to the OSFRA structure
//! \param new_injection_amplitude Injection amplitude
//! \return None
//!
extern void OSFRA_updateInjectionAmplitude(OSFRA *OSFRA_obj,
                                       float32_t new_injection_amplitude);

//! \brief Injects small signal disturbance into the control loop
//! \param ref refernce value on which the injection is added
//! \return Routine returns the reference plus the injection when SFRA sweep
//!         is active, when SFRA sweep is not active that is if SFRA state is 0
//!         it returns the the refernce without any change
//!
extern float32_t OSFRA_inject(float32_t ref);

//! \brief Collects the response of the loop because of small signal disturbance
//!        injected
//! \param *control_output pointer to the variable where control output is saved
//!                        note though the parameter is passed by reference
//!                        it is unchanged by the module
//! \param *feedback pointer to the variable where control output is saved
//!                        note though the parameter is passed by reference
//!                        it is unchanged by the module
//! \return None
//!
extern void OSFRA_collect(float32_t *control_output, float32_t *feedback);

//! \brief Runs the background task, this routine executes the state machine
//!        when a frequency sweep is started and is responsible for changing
//!        the frequency points and saving the measured results in an array
//! \param *OSFRA_obj  Pointer to the OSFRA structure pointer
//! \return None
//!
extern void OSFRA_runBackgroundTask(OSFRA *OSFRA_obj);

#ifdef __cplusplus
}
#endif // extern "C"

#endif
