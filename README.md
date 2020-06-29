# EOG-Signal-Analysis

This project was done to perform signal processing for electrooculography (EOG) signals that were used to control a robotic wheelchair as part of a larger project that included circuitry and 3D printing aspects. The project was for the Biomedical Engineering Society of USF with the R&D division.

To acquire the signal, leads are placed on the face around the eyes to track horizontal as well as vertical movement. These are then relayed to an arduino where the signal processing is performed using a running average to filter the signals and derivative calculations to determine up/down and left/right movements. In the extent of my code, the derivative calculations are used to change the color of an RGB LED.

