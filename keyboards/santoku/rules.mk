MCU = atmega32u4

# Bootloader selection
BOOTLOADER = atmel-dfu

BOOTMAGIC_ENABLE = no  # Enable Bootmagic Lite
EXTRAKEY_ENABLE = no   # Audio control and System control
CONSOLE_ENABLE = no     # Console for debug
COMMAND_ENABLE = no     # Commands for debug and configuration
NKRO_ENABLE = yes       # USB Nkey Rollover

# Options that are specific to current build of Santoku
PS2_MOUSE_ENABLE = yes
PS2_USE_USART = yes
ENCODER_ENABLE = yes
LTO_ENABLE = yes
OLED_ENABLE = yes
