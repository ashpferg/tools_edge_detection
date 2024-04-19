## Description
A pixel manipulation / filter tool  

This is designed to take an input image in .raw format and detect edges.  
Edge detection is performed using manual implementation of the Sobel operator.  
It outputs a .raw file, which can be converted to PNG using apps like Photoshop.  
Examples of input and output are included in the image folder.  

## Thresholds
This program gives the option of using a predetermined threshold value.  
Any pixel above the value are turned pure white. Pixels below the threshold are turned pure black.

Not using thresholds results in a higher quality image with a broader range of white, gray, and black.  
See /images/unesco/ for examples. Nth = no threshold, th = threshold. 
