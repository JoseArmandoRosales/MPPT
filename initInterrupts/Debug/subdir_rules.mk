################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="C:/Users/User/Desktop/C2000 Trabajos/initInterrupts" --include_path="C:/Users/User/Desktop/C2000 Trabajos/MPPT_CODE_USING_LIB/CPU1_RAM" --include_path="C:/Users/User/Desktop/C2000 Trabajos/MPPT_CODE_USING_LIB/CPU1_RAM/syscfg" --include_path="C:/Users/User/Desktop/C2000 Trabajos/MPPT_CODE_USING_LIB/device" --include_path="C:/ti/C2000Ware_4_03_00_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --define=CPU1 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


