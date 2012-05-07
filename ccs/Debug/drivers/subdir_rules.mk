################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
drivers/rit128x96x4.obj: C:/Luminary/StellarisWare/boards/ek-lm3s6965/drivers/rit128x96x4.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: TMS470 Compiler'
	"C:/EC_FR/Code Composer Studio/ccsv5/tools/compiler/tms470/bin/cl470" -mv7M3 -g -O2 --gcc --define=ccs --define=PART_LM3S6965 --include_path="C:/EC_FR/Code Composer Studio/ccsv5/tools/compiler/tms470/include" --include_path="C:/Luminary/StellarisWare/boards/ek-lm3s6965/bt_classd/ccs/../.." --include_path="C:/Luminary/StellarisWare/boards/ek-lm3s6965/bt_classd/ccs/../../../.." --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="drivers/rit128x96x4.pp" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


