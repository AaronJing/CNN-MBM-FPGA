# CNN-MBM-FPGA
4601
CNN with MBM on FPGA(ZEDBOARD)
HLS Build
Windows:
1. Open Start menu, search Vivado HLS 2017.2 Command Prompt, open it. Change directory to the hls, run command "vivado_hls ¨Cf script.tcl". It should build the whole hls project for you.
2. run command "vivado_hls -p hls_lenet" it should open the project in GUI
3. click run c synthesis
4. Export RTL.(Pack into IP catalog, but you still need to add this repositry in Vivado)

C simulation:
After you run c sunthesis, click run C simulation, you will see the result output in the console.

Cosimulation:
I have not done this. Basically, it compare the output from both software(C simulation) and RTL(hardware simulation).

Vivado:
Directly open folder, run the .xpr.

Current issue:
1. HLS synthesis generated a few warnings. They are inside hls folder called issue.
2. PL logic is running at 75 mhz not 100mhz, there are a few setup timing violation inside my code.(Hard to fix)
3. The parameter is smaller for simple testing
