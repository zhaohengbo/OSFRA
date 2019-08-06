# OSFRA
 An Open-Source System Frequency Response Analyzer
## DSP_LIB
 DSP LIB is used to estimate the parameters of the system.
 
 The system, as shown in the figure, can be divided into analog domain and digital domain.
 
 ![Control System](https://github.com/zhaohengbo/OSFRA/blob/master/image/1.png)
 
 It is very easy to get:
 
 (1)GH = e^(theta_feedback) / e^(theta_error) = e^(theta_feedback) / (e^(theta_reference) - e^(theta_feedback))
 
 (2)H = e^(theta_control) / e^(theta_feedback)
 
 (3)GH = e^(theta_reference) / e^(theta_feedback)
 
 **As e^(theta_reference).I is almost zero, some simplifications can be made.**
 
 This is the main algorithm in DSP LIB, besides there are some supporting codes.
 ## HOST_APP
 HOST APP is used to displaying the frequency response and compute the best control parameters.
 
 Recently, it is not finished yet.