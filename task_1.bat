@ECHO OFF
ECHO:
ECHO Task 1 LOG Filtering
ECHO ---------------------------------------------------
cd Executables
ECHO Performing the following commands...
ECHO:
ECHO: pens_rgb
ECHO Task1 ../Data/pens_rgb.bmp ../Data/out_1.bmp 1 1
Task1 ../Data/pens_rgb.bmp ../Data/out_1.bmp 1 1
ECHO:
ECHO Task1 ../Data/pens_rgb.bmp ../Data/out_1_2.bmp 2 1
Task1 ../Data/pens_rgb.bmp ../Data/out_1_2.bmp 2 1
ECHO:
ECHO Task1 ../Data/pens_rgb.bmp ../Data/out_1_3.bmp 1 5
Task1 ../Data/pens_rgb.bmp ../Data/out_1_3.bmp 1 5
ECHO:
ECHO: lenna_mono
ECHO Task1 ../Data/lenna_mono.bmp ../Data/out_1_4.bmp 1 1
Task1 ../Data/lenna_mono.bmp ../Data/out_1_4.bmp 1 1
ECHO:
ECHO Task1 ../Data/lenna_mono.bmp ../Data/out_1_5.bmp 2 1
Task1 ../Data/lenna_mono.bmp ../Data/out_1_5.mp 2 1
ECHO:
ECHO Task1 ../Data/lenna_mono.bmp ../Data/out_1_6.bmp 1 5
Task1 ../Data/lenna_mono.bmp ../Data/out_1_6.bmp 1 5
ECHO:
ECHO TASKS COMPLETE
ECHO To view the output, use mi_viewer 'filepath'
cd ../
