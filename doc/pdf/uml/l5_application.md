@startuml
left to right direction
artifact l5_application as l5
file main.c as main

artifact "Application modules" as application_modules

l5 --> main
l5 --> application_modules

@enduml
