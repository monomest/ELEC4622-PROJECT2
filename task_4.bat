@ECHO OFF
cd Executables
ECHO:
ECHO Task 4 Edge detection
ECHO -----------------------------------------------------------
ECHO Performing the following commands...
ECHO:
ECHO: pens_rgb
ECHO Task4 ../Data/pens_rgb.bmp ../Data/out_4_1_1.bmp 1 1
Task4 ../Data/pens_rgb.bmp ../Data/out_4_1_1.bmp 1 1
ECHO Task4 ../Data/pens_rgb.bmp ../Data/out_4_1_2.bmp 3 1
Task4 ../Data/pens_rgb.bmp ../Data/out_4_1_2.bmp 3 1
ECHO Task4 ../Data/pens_rgb.bmp ../Data/out_4_1_3.bmp 5 1
Task4 ../Data/pens_rgb.bmp ../Data/out_4_1_3.bmp 5 1
ECHO:
ECHO lenna_mono
ECHO Task4 ../Data/lenna_mono.bmp ../Data/out_4_2_1.bmp 1 1
Task4 ../Data/lenna_mono.bmp ../Data/out_4_2_1.bmp 1 1
ECHO Task4 ../Data/lenna_mono.bmp ../Data/out_4_2_2.bmp 3 1
Task4 ../Data/lenna_mono.bmp ../Data/out_4_2_2.bmp 3 1
ECHO Task4 ../Data/lenna_mono.bmp ../Data/out_4_2_3.bmp 5 1
Task4 ../Data/lenna_mono.bmp ../Data/out_4_2_3.bmp 5 1
ECHO:
ECHO Playing all the BMP files...
mi_pipe2 :: read_file -f ../Data/out_4_1_1.bmp ../Data/out_4_1_2.bmp ../Data/out_4_1_3.bmp :: frame_repeat :: view -play -rate 1
mi_pipe2 :: read_file -f ../Data/out_4_2_1.bmp ../Data/out_4_2_2.bmp ../Data/out_4_2_3.bmp :: frame_repeat :: view -play -rate 1
ECHO:
ECHO TASKS COMPLETE
ECHO To view the output, use mi_viewer 'filepath'
cd ../
