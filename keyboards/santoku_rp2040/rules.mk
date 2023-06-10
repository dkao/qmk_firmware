#MCU = atmega32u4

#BOARD = GENERIC_RP_RP2040

# Bootloader selection
#BOOTLOADER = rp2040         # W25Q080 selected by default for RP2040
#BOARD = GENERIC_RP_RP2040

BOOTMAGIC_ENABLE = yes  # Enable Bootmagic Lite
EXTRAKEY_ENABLE = yes   # Audio control and System control
CONSOLE_ENABLE = no     # Console for debug
COMMAND_ENABLE = no     # Commands for debug and configuration
NKRO_ENABLE = yes       # USB Nkey Rollover

# Options that are specific to current build of Santoku
ENCODER_ENABLE = yes
LTO_ENABLE = yes
OLED_ENABLE = yes
OLED_DRIVER = SSD1306

#PS2_ENABLE = yes
#PS2_MOUSE_ENABLE = yes
#PS2_USE_USART = yes
#PS2_USE_BUSYWAIT = yes
#MOUSEKEY_ENABLE = yes
#OPT_DEFS += -DHAL_USE_I2C=TRUE
#POINTING_DEVICE_ENABLE = yes # used for the scroll wheel
