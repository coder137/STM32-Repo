@startuml
left to right direction
artifact "l2_driver" as l2_driver
artifact "l2_board" as l2_board

rectangle "Controller specific" {
    artifact "GPIO" as gpio
    artifact "UART" as uart
}

rectangle "Board Startup" {
    artifact "UART Port" as board_uart
    artifact "LEDs" as leds
    artifact "Buttons" as buttons
}

l2_driver --> gpio
l2_driver --> uart

gpio --> l2_board
uart --> l2_board

l2_board --> board_uart
l2_board --> leds
l2_board --> buttons

@enduml
