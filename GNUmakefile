# GNUmakefile for APLC1 - APL interpreter (macOS/Linux)
# Uses K&R C compatibility flags; original code is circa 1986.

CC     = cc
CFLAGS = -std=gnu89 -w -g -I.

OBJS = clr.o d.o df.o dt.o k.o m.o ma.o n.o p.o p0.o s.o u.o e.o th.o \
       dth.o fd.o ind.o m1.o t.o m2.o m3.o thenc.o q1.o pk.o c.o q0.o \
       fmt.o fmtp.o rs.o o.o uc.o mf.o unixos.o st.o sv.o i.o cq.o cqu.o \
       f3.o unix_stubs.o unix_sy.o

all: apl

apl: $(OBJS)
	$(CC) -o apl $(OBJS) -lm

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) apl
