# name of the program (Minix service)
PROG=proj

.PATH:	${.CURDIR}/drivers/
.PATH:	${.CURDIR}/drivers/timer
.PATH:	${.CURDIR}/drivers/keyboard_and_mouse
.PATH:	${.CURDIR}/drivers/video
.PATH:	${.CURDIR}/game_logic
.PATH:  ${.CURDIR}/sprites
.PATH:  ${.CURDIR}/drivers/rtc
.PATH:  ${.CURDIR}/drivers/serial_port


# source code files to be compiled  serial_pPOrt.Port.c
SRCS =	proj.c timer.c utils.c KBC.c keyboard.c mouse.c video.c sprites.c menu.c instructions.c rtc.c serial_port.c queue.c accept.c wait.c game.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>