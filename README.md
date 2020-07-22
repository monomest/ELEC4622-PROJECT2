## ELEC4622 Project 2
Renee Lu, z5160268 | July, 2020

## Tasks Overview
1. **Task 1** LOG filtering
2. **Task 2** Separable LOG filtering
3. **Task 3** Comparison of different sigmas in LOG filtering
4. **Task 4** Edge detection using LOG filtering
5. **Task 5 Bonus** Integer processing for separable LOG filtering
6. **Task 6 Bonus** Simple Morphology Dilation after LOG filtering

## Code Location
The solution file is found in:
- *Project2/Project2.sln*

The code for Task 1, 2 and 3 are in, respectively:
- *Project2/Task1/main1.cpp*
- *Project2/Task2/main2.cpp*
- *Project2/Task3/main3_sep.cpp*
- *Project2/Task3/main3_no_sep.cpp* - this is an inefficient implementation
- *Project2/Task4/main4.cpp*
- *Project2/Task5/main5.cpp*
- *Project2/Task6/main6.cpp*

## Project Properties
In Visual Studio, the non-default property configuration for the projects are:
- **Configuration Properties > General > Output Directory = [executables output file path]**
e.g. "*../../Executables*"
- **Configuration Properties > Advanced > Character Set = Use Multi-Bye Character Set**
- **Configuration Properties > Debugging > Command Arguments = [input bmp file path] [output bmp file path] [sigma] [a]**
e.g. "*../Data/pens_rgb ../Data/out.bmp 1 1*"
- **C/C++ > Preprocessor > Preprocessor Definitions = WIN32; _CRT_SECURE_NO_DEPRECATE**

## Steps to Run
1. Open Visual Studio 
2. Open the Project2/Project2.sln
3. Configure the Project Properties as above
3. Run the desired tasks using the command line arguments below:

To run Task 1 give the following command line arguments:
[input bmp file path] [output bmp file path] [sigma] [a]
