@startuml
file linker_script.ld as linker_script
package l0_lowlevel {
    file "startup_{controller}.c" as startup
    file "system_{controller}.c" as system
    file "entry_point.c" as entry_point
}

package l2_board {
    file "peripherals.c" as peripheral_file
}

package l5_application {
    file "main.c" as main_file
}

linker_script --> startup
startup --> system
system --> entry_point
entry_point --> peripheral_file
peripheral_file --> main_file

file "startup_{controller}.c" {
    card "Interrupt Vector Table" as ivt
    card "Reset Handler" as reset_handler
}

file "system_{controller}.c" as system_controller_file {
    card "System Initialization" as sys_init
    card "BSS Initialization" as bss_init
    card "DATA Initialization" as data_init
}

file "entry_point.c" {
    card "Clock Update" as clk_update
    card "extern Peripheral Init" as extern_periph_init
    card "extern main" as extern_main

    clk_update --> extern_periph_init
    extern_periph_init --> extern_main
}

ivt --> reset_handler
reset_handler --> system_controller_file
system_controller_file --> clk_update

@enduml
