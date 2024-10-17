
primate_pgm.o:	file format elf32-littleprimate

Disassembly of section .text:

00000048 <_Z12primate_mainv>:
2 --------
      48: 13 00 00 00  	nop
      4c: 13 00 00 00  	nop
      50: 13 00 00 00  	nop
      54: 13 00 00 00  	nop
      58: 13 00 00 00  	nop
      5c: 13 00 00 00  	nop
      60: 8b 32 80 00  	inputread	p5, x0, 8
      64: 13 00 00 00  	nop
      68: 13 00 00 00  	nop
3 --------
      6c: 2b 80 12 00  	extract	x0, p5, 1
      70: 2b 80 02 00  	extract	x0, p5, 0
      74: 33 04 4c 00  	add	x2, x3, x4
      78: ab 92 02 00  	insert	p5, p5, 0
      7c: 13 00 00 00  	nop
      80: 13 00 00 00  	nop
      84: 13 00 00 00  	nop
      88: 13 00 00 00  	nop
      8c: 13 00 00 00  	nop
4 --------
      90: 13 00 00 00  	nop
      94: 13 00 00 00  	nop
      98: 13 00 00 00  	nop
      9c: 13 00 00 00  	nop
      a0: 13 00 00 00  	nop
      a4: 13 00 00 00  	nop
      a8: 6b 90 82 00  	outputwrite	p5, 8
      ac: 13 00 00 00  	nop
      b0: 6f f0 ff ff  	j	0xae <_Z12primate_mainv+0x66>
