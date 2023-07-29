# TripleSaw

This example shows how to make a complex oscillator voice using three bandlimited sine waves.

## Controls

| Pin Name | Pin Location | Function | Comment |
| --- | --- | --- | --- |
| CV_1 | C5 | Coarse Tuning | Tunes the base frequency of all three oscillators |

| CV_2 | C4 | Fine Tuning | Fine tune adjustment of base freq. for all three oscillators |
| CV_3 | C3 | Detune Amount | Adjusts the amount of detuning between all three voices |
| CV_5 | C6 | 1V/Octave Input | Musical pitch tracking input |
| CV_6 | ?  | 1V/Octave Input 2 | Musical pitch tracking input |


The _comments.cpp file is identical to the main file with detailed comments of each piece of the source code.

## install
```sh
make clean && make && make program-dfu

dfu-util -w -a 0 -s 0x08000000:leave -D build/Merlin.bin -d ,0483:df11
```