
SRCS := $(wildcard *.c)
OBJS := $(subst .c,.o,$(SRCS))
PROGRAM := nw

############################################################
################Settings for Compile########################
############################################################

CXX := gcc
CXXFLAGS := -g -Wall
LDFLAGS := -lm

################# For OpenGL Compile #######################
# OPENGL_CFLAGS  = -I.
# OPENGL_LDFLAGS = -lglut -lGLU -lGL -lm

################# For OpenCV Compile #######################
# OPENCV_CFLAGS = `pkg-config opencv --cflags`
# OPENCV_LIBS   = `pkg-config opencv --libs`
# GLUT_CFLAGS = `pkg-config gl --cflags` `pkg-config glu --cflags`
# GLUT_LIBS   = `pkg-config gl --libs` `pkg-config glu --libs` -lglut
# INCLUDE = -I./

############################################################
############################################################



all:$(PROGRAM)


#make executable files
$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(OPENGL_CFLAGS) $(OPENGL_LDFLAGS) $(OPENCV_CFLAGS) $(OPENCV_LIBS) $(LDFLAGS)


#make object files
.c.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(OPENGL_CFLAGS) $(OPENGL_LDFLAGS) $(OPENCV_CFLAGS) $(OPENCV_LIBS) $(LDFLAGS)


.PHONY: clean depend
clean:
	rm $(OBJS) $(PROGRAM)

depend:
	makedepend -- $(CXXFLAGS) $(OPENGL_CFLAGS) $(OPENGL_LDFLAGS) $(OPENCV_CFLAGS) $(OPENCV_LIBS) $(LDFLAGS) -- $(SRCS)


# DO NOT DELETE

nw.o: /usr/include/stdio.h /usr/include/features.h
nw.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
nw.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
nw.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
nw.o: /usr/include/bits/typesizes.h /usr/include/libio.h
nw.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h
nw.o: /usr/include/xlocale.h /usr/include/bits/stdio_lim.h
nw.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
nw.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
nw.o: /usr/include/endian.h /usr/include/bits/endian.h
nw.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
nw.o: /usr/include/time.h /usr/include/sys/select.h
nw.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
nw.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
nw.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
nw.o: /usr/include/math.h /usr/include/bits/huge_val.h
nw.o: /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h
nw.o: /usr/include/bits/inf.h /usr/include/bits/nan.h
nw.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
