@ECHO OFF
cd Executables
ECHO:
ECHO Task 3 Comparing different sigma
ECHO ----------------------------------------------------
ECHO Performing the following commands...
ECHO:
ECHO: Sigma_min = 1, sigma_max = 5, alpha = 1, N = 5
ECHO Task3 ../Data/pens_rgb.bmp ../Data/ 1 5 1 5
Task3 ../Data/pens_rgb.bmp ../Data/ 1 5 1 5
ECHO:
ECHO Playing the 5 BMP files...
ECHO mi_pipe2 :: read_file -f ../Data/out_3_0_1.bmp ../Data/out_3_1_1.49535.bmp ../Data/out_3_2_2.23607.bmp ../Data/out_3_3_3.3437.bmp ../Data/out_3_4_5.bmp :: frame_repeat :: view -play -rate 1
mi_pipe2 :: read_file -f ../Data/out_3_0_1.bmp ../Data/out_3_1_1.49535.bmp ../Data/out_3_2_2.23607.bmp ../Data/out_3_3_3.3437.bmp ../Data/out_3_4_5.bmp :: frame_repeat :: view -play -rate 1
ECHO:
ECHO TASKS COMPLETE
ECHO To view the output, use mi_viewer 'filepath'
cd ../
