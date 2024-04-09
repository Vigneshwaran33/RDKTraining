DESCRIPTION = "Calculator Application"
LICENSE = "CLOSED"

SRC_URI = "file://calc.c"


S = "${WORKDIR}"
do_compile(){
	${CC} calc.c ${LDFLAGS} -o calc
}
do_install(){
	install -d ${D}${bindir}
	install -m 0755 ${WORKDIR}/calc ${D}${bindir}
}

