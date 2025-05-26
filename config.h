// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define MASTER_LEFT
#define SPLIT_OLED_ENABLE
#define NO_ACTION_MACRO
#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT
#define IGNORE_MOD_TAP_INTERRUPT
#define NO_USB_STARTUP_CHECK

#define VIAL_KEYBOARD_UID {0xAD, 0x3C, 0x92, 0xF7, 0x48, 0x1E, 0xB0, 0x65}

/* RP2040- and hardware-specific config */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET // Activates the double-tap behavior
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U
#define PICO_XOSC_STARTUP_DELAY_MULTIPLIER 64
