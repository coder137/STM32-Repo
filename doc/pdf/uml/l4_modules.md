@startuml
left to right direction
artifact l4_modules as l4

artifact Sensor as s
artifact Technology as t
artifact Protocol as p

rectangle "Sensor" {
    artifact "LIDAR" as lidar
    artifact "Medical Sensors" as medical_sensors
    artifact "Environmental Sensors" as environmental_sensors
}

rectangle Technology {
    artifact "GPS" as gps
    artifact "BT/BLE" as ble
}

rectangle "Protocol" { 
    artifact "TCP/UDP" as socket
    artifact "MQTT" as mqtt
    artifact "OpenThread" as openthread
}

l4 --> s
l4 --> t
l4 --> p

s --> lidar
s --> medical_sensors
s --> environmental_sensors

t --> gps
t --> ble

p --> socket
p --> mqtt
p --> openthread

@enduml
