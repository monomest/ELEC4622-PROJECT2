@ECHO OFF
cd Executables
ECHO:
ECHO Task 2 Separable vs. No Separable LOG Filtering
ECHO ----------------------------------------------------
ECHO Performing the following commands...
ECHO:
ECHO Task2 ../Data/pens_rgb.bmp ../Data/out_2.bmp 1 1
Task2 ../Data/pens_rgb.bmp ../Data/out_2.bmp 1 1
ECHO:
ECHO Task1 ../Data/pens_rgb.bmp ../Data/out_1.bmp 1 1
Task1 ../Data/pens_rgb.bmp ../Data/out_1.bmp 1 1
ECHO:
ECHO Task2 ../Data/pens_rgb.bmp ../Data/out_2_1.bmp 3 5
Task2 ../Data/pens_rgb.bmp ../Data/out_2_1.bmp 3 5
ECHO:
ECHO Task1 ../Data/pens_rgb.bmp ../Data/out_1_2.bmp 3 5
Task1 ../Data/pens_rgb.bmp ../Data/out_1_2.bmp 3 5
ECHO:
ECHO TASKS COMPLETE
ECHO To view the output, use mi_viewer 'filepath'
cd ../
