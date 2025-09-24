*23/Sep/2025*
The agenda for the day was to breakdown the antenna design phase into smaller manageable tasks. 
The phase should include:
  1. Link Budget Analysis: i) This should proof the feasibility of the downlink (Accounting for the gains and all the losses)
                          ii) Gain an  understanding of the concepts in a link budget such as the EIRP, FSPL etc.
                         iii) Most parameters are already known such as the receiver's sensisitivity. So using the link budget equation you
                              can actually determine the gain you need for the Yagi-Uda antenna. ( This should also include a link margin)

    
  2. Design and Simulation with 4nec2 software: i) Learning the Yagi-Uda Basics e.g. the reflector elements, director elements and driven
                                                   elements etc.
                                               ii) Familiarizing with the 4nec2 software (Through tutorials)
                                              iii) Modelling the Antenna.
                                               iv) Analysing the results.
                                                v) Optimization.
 3. Antenna assembly.
 4. Field testing and performance evaluation.

I have gained a better understanding of the Link Budget analysis. My next step should be to document the calculation steps while noting all
the necessary parameters needed to perform the link budget. (Should also include any assumptions made). There are also online calculators to 
aid in the calculations. These should also be cited properly in the document. Once I have the minimum gain needed I should jump into the
antenna modelling and simulation.

*24/Sep/2025*
For today I was able to document the step-wise approach when performing the link budget to determine the required receive power. With the minimum required power you can determine the gain of the antenna to be designed. However, my calculations resulted in a negative value of gain. From what I have gathered this means the input parameters in the formula might need to be adjusted. The negative gain means that the signal is too strong at the distance I had used in the calculations i.e. 20km. I'll try implementing this considering a LEO satellite at a distance of 500-550km. Another adjustment that should be made is the receiver's sensitivity. The RFM95 has a sensitivity of -148dBm which is achieved at the highest spreading factor and narrow bandwidth. These parameters are adjustable based on our data rates and time-on-air considerations so it might be better to use a slightly less sensitve setting. There's not much I can say conclusively at the time until I have done more research on this.

