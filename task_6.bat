@ECHO OFF
cd Executables
ECHO:
ECHO Task 6 Morphology
ECHO ---------------------------------------------------------
ECHO: pens_rgb
ECHO Task6 ../Data/pens_rgb.bmp ../Data/out_6_1.bmp 5 1
Task6 ../Data/pens_rgb.bmp ../Data/out_6_1.bmp 5 1
ECHO Task4 ../Data/pens_rgb.bmp ../Data/out_4_1.bmp 5 1
Task4 ../Data/pens_rgb.bmp ../Data/out_4_1.bmp 5 1
ECHO:
ECHO lenna_mono
ECHO Task6 ../Data/lenna_mono.bmp ../Data/out_6_2.bmp 5 1
Task6 ../Data/lenna_mono.bmp ../Data/out_6_2.bmp 5 1
ECHO Task4 ../Data/lenna_mono.bmp ../Data/out_4_2.bmp 5 1
Task4 ../Data/lenna_mono.bmp ../Data/out_4_2.bmp 5 1
ECHO:
ECHO Playing back pens_rgb...
mi_pipe2 :: read_file -f ../Data/out_6_1.bmp ../Data/out_4_1.bmp :: frame_repeat :: view -play -rate 1
ECHO:
ECHO Playing back lenna_mono...
mi_pipe2 :: read_file -f ../Data/out_6_2.bmp ../Data/out_4_2.bmp :: frame_repeat :: view -play -rate 1
ECHO:
ECHO TASKS COMPLETE
ECHO To view the output, use mi_viewer 'filepath'
cd ../
