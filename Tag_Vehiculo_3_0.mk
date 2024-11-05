###############################################################################
#
# Copyright (C) 2018-2021 LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
# All rights reserved.
#
# This document is the property of LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
# It is considered confidential and proprietary.
#
# This document may not be reproduced or transmitted in any form,
# in whole or in part, without the express written permission of
# LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
#
################################################################################

NAME := apps_SmartFlow_SmartTorch_tag_vh4

$(NAME)_SOURCES := tag_vh4.c
#$(NAME)_SOURCES    += i2c.c

WIFI_CONFIG_DCT_H := wifi_config_dct.h

APPLICATION_DCT := DCT/dct_data_write.c

LWIP_NETIF_HOSTNAME==1


$(NAME)_COMPONENTS := graphics/u8g \
    					protocols/HTTP_client \
    					protocols/MQTT
    					
    					
##GLOBAL_DEFINES += WICED_DISABLE_STDIO
## NEED IT FOR WATCHDOG ##
##GLOBAL_DEFINES := APPLICATION_WATCHDOG_TIMEOUT_SECONDS=9

####### NEED IT FOR SD CARD###############################

# for SD/MMC card support
WICED_SDMMC_SUPPORT := yes

# default stack size was WICED_DEFAULT_APPLICATION_STACK_SIZE, 6K
# needs big stack size for deep directory processing
GLOBAL_DEFINES += APPLICATION_STACK_SIZE=1024*1024

# For exFAT support, use filesystems/FileX/exFAT (for licensees only)
$(NAME)_COMPONENTS += filesystems/FileX/default

# Crypto driver for 43907/54907 platform doesnt support some of AES modes such as XTS and OFB
# Therefore disabling hw crypto since this app uses XTS
GLOBAL_DEFINES += WICED_CONFIG_DONOT_USE_HW_CRYPTO
#==============================================================================
# following is for SD Card Debug strings
#==============================================================================
ifeq ($(BUILD_TYPE),debug)
GLOBAL_DEFINES += BCMDBG
GLOBAL_DEFINES += WPRINT_ENABLE_WICED_DEBUG WPRINT_ENABLE_WICED_ERROR
endif

VALID_OSNS_COMBOS := ThreadX-NetX ThreadX-NetX_Duo
VALID_PLATFORMS := BCM943907AEVAL1F* CYW943907AEVAL1F CYW943907WAE4 BCM943907WCD2 CYW954907AEVAL1F
