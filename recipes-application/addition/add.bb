DESCRIPTION = "Addition Application"
LICENSE = "CLOSED"

SRC_URI = "file://add.c"


S = "${WORKDIR}"
do_compile(){
	${CC} add.c ${LDFLAGS} -o add
}
do_install(){
	install -d ${D}${bindir}
	install -m 0755 ${WORKDIR}/add ${D}${bindir}
}

