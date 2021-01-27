@startuml

participant linker_script.ld as linker_script
participant "startup_{controller}.c" as startup_controller
collections "FreeRTOS" as freertos
participant "system_{controller}.c" as system_controller
collections CMSIS_5 as cmsis

participant "entry_point.c" as entry_point
participant "peripherals.c" as peripherals
participant "main.c" as main

group Interrupt Vector Table
linker_script -> startup_controller : stack_top
startup_controller -> startup_controller : Reset_Handler();
startup_controller -> freertos : vPortSVCHandler();
startup_controller -> freertos : xPortPendSVHandler();
startup_controller -> freertos : xPortSysTickHandler();
end


group Reset_Handler();
startup_controller -> system_controller : SystemInit();
startup_controller -> cmsis : __cmsis_start();
end


group __cmsis_start();
linker_script -> cmsis : copy_table_start
linker_script -> cmsis : copy_table_end
linker_script -> cmsis : zero_table_start
linker_script -> cmsis : zero_table_end

cmsis -> cmsis : __data_copy();
cmsis -> cmsis : __bss_zero();
cmsis -> entry_point : extern _start() __NO_RETURN; 
end

group _start();
entry_point -> entry_point : clock_update(80_mhz);
entry_point -> entry_point : feature semihosting
entry_point -> peripherals : extern peripheral_init();
entry_point -> main : extern int main();
end

@enduml
