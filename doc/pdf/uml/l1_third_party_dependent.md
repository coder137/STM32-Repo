@startuml
left to right direction
artifact l1_third_party_device_dependent as l1

artifact "RTOS" as rtos

artifact "FreeRTOS" as freertos
artifact "ThreadX" as threadx

artifact "External Libraries (device specific)" as external_libs

l1 --> rtos
l1 --> external_libs

rtos --> freertos
rtos --> threadx

@enduml
