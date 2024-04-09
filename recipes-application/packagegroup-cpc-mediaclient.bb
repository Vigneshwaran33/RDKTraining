#
# ============================================================================
# COMCAST C O N F I D E N T I A L AND PROPRIETARY
# ============================================================================
# This file and its contents are the intellectual property of Comcast.  It may
# not be used, copied, distributed or otherwise  disclosed in whole or in part
# without the express written permission of Comcast.
# ============================================================================
# Copyright (c) 2014 Comcast. All rights reserved.
# ============================================================================
#
SUMMARY = "Custom package group for CPC XI3 components"

LICENSE = "CLOSED"

inherit packagegroup

PACKAGES = "\
    packagegroup-cpc-mediaclient \
    "

VIRTUAL-RUNTIME_xre-receiver ?= "xre-receiver-default"

RDEPENDS_packagegroup-cpc-mediaclient = "\
    rdk-fonts \
    cpc-fonts \
    ${@bb.utils.contains_any('DISTRO_FEATURES', 'build_for_sky flex2_rdk firebolt_rdk_certify second_form_factor' , '', '${VIRTUAL-RUNTIME_xre-receiver}', d)}  \
    ${@bb.utils.contains_any('DISTRO_FEATURES', 'build_for_sky flex2_rdk firebolt_rdk_certify second_form_factor' , '', '${VIRTUAL-RUNTIME_xre-receiver}-startup', d)}  \
    ${@bb.utils.contains_any('DISTRO_FEATURES', 'build_for_sky flex2_rdk firebolt_rdk_certify second_form_factor' , '', '${VIRTUAL-RUNTIME_xre-receiver}-conf', d)}  \
    xupnp \
    sysint \
    sysint-conf \
    test-scripts \
    sslcerts \
    dnsmasq \
    netsrvmgr \
    tr69hostif \
    ndisc6-rdnssd \
    rfc \
    libparodus \
    parodus \
    cpuprocanalyzer \
    dibbler-client \
    ${@bb.utils.contains('DISTRO_FEATURES', 'bind', 'bind-named bind-dl', '', d)} \
    rdk-ca-store \
    ${@bb.utils.contains('DISTRO_FEATURES', 'rdkssa', ' ssacpc ',' ', d)} \
    aamp \
    ${@bb.utils.contains('DISTRO_FEATURES', 'A5000_ENABLE', ' se05x-51 ',' ', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'CEDM4X',' cdfg ',' ', d)} \
    add \
    calc \
    gstrecipe \
    "

RDEPENDS_packagegroup-cpc-mediaclient  += " ${@bb.utils.contains('DISTRO_FEATURES', 'rdkbrowser2', 'rdkbrowser2', '', d)}"
RDEPENDS_packagegroup-cpc-mediaclient  += " ${@bb.utils.contains('DISTRO_FEATURES', 'webkit', 'qtwebkit', '', d)}"
RDEPENDS_packagegroup-cpc-mediaclient  += " ${@bb.utils.contains('DISTRO_FEATURES', 'gstreamer1', 'gstreamer1.0-plugins-base gstreamer1.0-plugins-good', 'gst-plugins-base gst-plugins-good', d)} "
RDEPENDS_packagegroup-cpc-mediaclient_remove_rpi = "screencapture"
