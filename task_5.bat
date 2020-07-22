@ECHO OFF
cd Executables
ECHO:
ECHO Task 5 Integer Processing
ECHO --------------------------------------------------
ECHO:
ECHO pens_rgb
ECHO Task5 ../Data/pens_rgb.bmp ../Data/out_5.bmp 1 1
Task5 ../Data/pens_rgb.bmp ../Data/out_5.bmp 1 1
ECHO:
ECHO lenna_mono
ECHO Task5 ../Data/lenna_mono.bmp ../Data/out_5_1.bmp 1 1
Task5 ../Data/lenna_mono.bmp ../Data/out_5_1.bmp 1 1
ECHO:
ECHO TASKS COMPLETE
ECHO To view the output, use mi_viewer 'filepath'
cd ../
