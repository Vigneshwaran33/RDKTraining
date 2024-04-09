DESCRIPTION = "Gstreamer Application"
LICENSE = "CLOSED"
SRC_URI += "file://gstrecipe.c"


S = "${WORKDIR}"

DEPENDS += "${@bb.utils.contains('DISTRO_FEATURES', 'gstreamer1', 'gstreamer1.0  gstreamer1.0-plugins-base  gstreamer1.0-plugins-good  gstreamer1.0-plugins-bad  gstreamer1.0-libav', 'gstreamer gst-plugins-base gst-plugins-good gst-plugins-bad gst-libav', d)} "



inherit pkgconfig
do_compile(){
        ${CC} gstrecipe.c ${LDFLAGS} -o gstrecipe $(pkg-config --cflags --libs gstreamer-1.0)
}

do_install(){
        install -d ${D}${bindir}
        install -m 0755 ${WORKDIR}/gstrecipe ${D}${bindir}
}



