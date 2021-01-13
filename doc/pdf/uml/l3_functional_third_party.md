@startuml
left to right direction
artifact l3_functional_third_party as l3

artifact Ringbuffer as rb
artifact Stack as s
artifact Queue as q

artifact Linebuffer as lb

l3 --> rb
l3 --> s
l3 --> q

rb --> lb

@enduml
