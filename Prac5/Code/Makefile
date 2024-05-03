# Makefile for Verilog program

# Compiler used is iverilog
CC=iverilog 

# Source files to be compiled
ADC_SRC=adc.v adc_tb.v
TSC_SRC=tsc.v tsc_tb.v adc.v

# Compile Flag
FLAGS=-g2009 # To add support for system verilog

# Run command used
RC=vvp 

tsc: $(TSC_SRC)
	$(CC) $(FLAGS) $(TSC_SRC) -o tsc

runtsc:
	$(RC) tsc

adc: $(ADC_SRC)
	$(CC) $(FLAGS) $(ADC_SRC) -o adc

runadc:
	$(RC) adc

clean:
	rm -f tsc adc

cleantsc:
	rm -f tsc

cleanadc:
	rm -f adc
