# Car-Black-Box
A CBB prototype developed for PIC microcontroller, with state-machine architecture. The application essentially provides a dashboard displaying the rela time events triggred in the car, data logging along with the login fetaure.

# Microcontroller used
PIC16F877A (comes with a inbuilt CLCD screen that is used as dashboard)

# Simulation environment (Alternative to the hardware)
PICSIMLAB

# Usage
1. The dashboard displays the time along with the current gear and speed.
2. Use RB2 to down-gear, RB1 to up-gear and RB0 for reverse.
3. Press RB4 or RB5 to enter the login page
4. The default system password is RB4 RB5 RB4 RB5 - Enter this to complete the login
5. Use RB4 & RB5 to scroll up and down the listed menus and RB6 to select a menu
6. Menus supported - View log, Clear log, Change Password

