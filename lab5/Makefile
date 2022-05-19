# name of the program (Minix service)
PROG=lab5

# source code files to be compiled
SRCS = lab5.c video_gr.c kbc.c utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 5, only LCF library)
DPADD += ${LIBLCf}
DPADD += ${LIBLM}
DPADD += ${LIBXPM}
LDADD += -llcf -llm -lxpm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
