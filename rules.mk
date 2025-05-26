# MCU and architecture
MCU = RP2040

# Build options
BOOTLOADER = rp2040
SERIAL_DRIVER = vendor
CONVERT_TO = rp2040_ce

# Enabled features
BOOTMAGIC_ENABLE = yes
EXTRAKEY_ENABLE = yes
MOUSEKEY_ENABLE = yes
NKRO_ENABLE = yes
OLED_ENABLE = yes
WPM_ENABLE = yes
COMBO_ENABLE = no
VIA_ENABLE = yes
VIAL_ENABLE = yes
CONSOLE_ENABLE = yes

# Split support
SPLIT_KEYBOARD = yes
SPLIT_TRANSPORT = vendor
SPLIT_USB_DETECT = yes
SPLIT_LAYER_STATE_ENABLE = yes
SPLIT_OLED_ENABLE = yes
SPLIT_WPM_ENABLE = yes

# OLED Driver (SSD1306 assumed from your photos)
OLED_DRIVER = ssd1306
OLED_DISPLAY_128X64 = yes

# RGB settings (if used)
# RGB_MATRIX_ENABLE = yes
# RGB_MATRIX_DRIVER = vendor
# RGB_MATRIX_SUPPORTED = yes
# RGB_MATRIX_SPLIT = yes

# Encoder support (optional, remove if not needed)
# ENCODER_ENABLE = yes

# Disable unused features to save space
COMMAND_ENABLE = no
CONSOLE_ENABLE = no
AUDIO_ENABLE = no
MUSIC_ENABLE = no
LTO_ENABLE = yes
