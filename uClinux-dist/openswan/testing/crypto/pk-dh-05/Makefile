OPENSWANSRCDIR=$(shell cd ../../..; pwd)

UNITTEST=pk-dh
CFLAGS=-DUSE_3DES -I${OPENSWANSRCDIR}/programs/pluto

SOURCES=pk-dh.c ${OPENSWANSRCDIR}/programs/pluto/hmac.c
SOURCES+=${OPENSWANSRCDIR}/programs/pluto/vulcan/crypto_dh_vulcanpk.c
SOURCES+=${OPENSWANSRCDIR}/programs/pluto/vulcan/vulcanpk_funcs.c
SOURCES+=${OPENSWANSRCDIR}/programs/pluto/crypto.c
SOURCES+=${OPENSWANSRCDIR}/programs/pluto/ike_alg.c
SOURCES+=${OPENSWANSRCDIR}/programs/pluto/crypt_utils.c
SOURCES+=${OPENSWANSRCDIR}/programs/pluto/crypt_dh.c

pk-dh: ${SOURCES}

programs checkprograms: ${UNITTEST}
	@true

include ${OPENSWANSRCDIR}/programs/Makefile.program


