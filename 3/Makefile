#--------------------------------------------------------------
# 
#  4190.308 Computer Architecture (Fall 2018)
#
#  Project #3: Image Pixelization
#
#  November 6, 2018
#
#  Jin-Soo Kim (jinsoo.kim@snu.ac.kr)
#  Systems Software & Architecture Laboratory
#  Dept. of Computer Science and Engineering
#  Seoul National University
#  http://csl.snu.ac.kr
#
#--------------------------------------------------------------

CC	= gcc
AS	= as
CFLAGS	= -g -O -Wall
ASFLAGS	= -g
LDFLAGS	= 
RM	= rm

CSRCS	= bmp.c
ASRCS	= bmpmosaic.s
TARGET	= bmpmosaic
OBJECTS	= $(CSRCS:.c=.o) $(ASRCS:.s=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.s.o:
	$(AS) $(ASFLAGS) $< -o $@

clean:
	$(RM) -f $(OBJECTS) $(TARGET) *~
