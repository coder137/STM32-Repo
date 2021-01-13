@startuml
left to right direction
artifact l0_lowlevel as l0
rectangle Files {
    artifact "ARM" as arm
    artifact "Device" as device
    artifact "Linker Script" as linker
    artifact "Newlib" as newlib
}

artifact "CMSIS_5" as cmsis

rectangle "Controller Specific Files" {
    file "startup_{controller}.c" as startup
    file "system_{controller}.c/.h" as system
    file "device_header_{controller}.h" as device_header
    file "entry_point.c" as entry_point
}

rectangle "Internal Library" {
    file "syscalls.c" as syscalls
    file "sysbrk.c" as sysbrk
}

file architecture_and_controller_specific.ld as linker_file


l0 --> device
l0 --> linker
l0 --> arm

arm --> cmsis
device --> startup
device --> system
device --> device_header
device --> entry_point

linker --> linker_file

newlib --> syscalls
newlib --> sysbrk

@enduml
