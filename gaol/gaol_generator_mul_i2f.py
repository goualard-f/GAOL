#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-

# Generator for the 256 cases based on signs of the operands
# of the multiplication of 2 interval2f 
# (c) 2010 Frederic Goualard, LINA UMR CNRS 6241

# [a,b]x[e,f] | [c,d]x[g,h]
# <d, -c, b, -a> x <h, -g, f, -e>

# [x, y] X [z, t]
# <y, -x> X <t, -z>
# formulas index: XXXX
#                 ^^^^-- sign(x)
#                 ||+--- sign(y)
#                 |+---- sign(z)
#                 +----- sign(t)

import ply.lex as lex
import ply.yacc as yacc

# Position of operands in SSE vectors
pos_operands = {'a':0, 'b':1, 'c':2, 'd':3, 'e':0, 'f':1, 'g':2, 'h':3}

# Instruction to flip signs of the operands of a register
flip_signs =   ("__m128 smask0 = _mm_set_ps(0.0f,0.0f,0.0f,0.0f);\n",
				"__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);\n",			 
				"__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);\n",			 
				"__m128 smask3 = _mm_set_ps(0.0f,0.0f,-0.0f,-0.0f);\n",			 
				"__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);\n",			 
				"__m128 smask5 = _mm_set_ps(0.0f,-0.0f,0.0f,-0.0f);\n",			 
				"__m128 smask6 = _mm_set_ps(0.0f,-0.0f,-0.0f,0.0f);\n",			 
				"__m128 smask7 = _mm_set_ps(0.0f,-0.0f,-0.0f,-0.0f);\n",			 
				"__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);\n",			 
				"__m128 smask9 = _mm_set_ps(-0.0f,0.0f,0.0f,-0.0f);\n",			 
				"__m128 smask10 = _mm_set_ps(-0.0f,0.0f,-0.0f,0.0f);\n",			 
				"__m128 smask11 = _mm_set_ps(-0.0f,0.0f,-0.0f,-0.0f);\n",			 
				"__m128 smask12 = _mm_set_ps(-0.0f,-0.0f,0.0f,0.0f);\n",			 
				"__m128 smask13 = _mm_set_ps(-0.0f,-0.0f,0.0f,-0.0f);\n",			 
				"__m128 smask14 = _mm_set_ps(-0.0f,-0.0f,-0.0f,0.0f);\n",			 
				"__m128 smask15 = _mm_set_ps(-0.0f,-0.0f,-0.0f,-0.0f);\n")

tokens = (
	'SEP',
	'COMMA',
	'MINUS',
	'TIMES',
	'MAX',
	'LPAREN',
	'RPAREN',
	'ZERO',
	'VAR',
	 )
	 
t_SEP = r';'
t_COMMA = r','
t_MINUS	= r'-'
t_TIMES = r'\*'
t_MAX = r'max'
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_ZERO = r'0'
t_VAR = r'[abcdefgh]'

t_ignore  = ' \t'

def t_error(t):
    print "Illegal character '%s'" % t.value[0]
    t.lexer.skip(1)


#  Grammar
def p_vector(p):
	"""vector : expr SEP expr SEP expr SEP expr"""
	if p[1]["iszero"] and p[5]["iszero"]: # all 4 operands are 0
		p[0] = """xmmbounds = interval2f::m128_zero();"""
		return
	# Necessary shuffle necessary for the operands of the first (and only, if no max) product 
	shuffle0L = p[1]["psmask"][0][0] << 6 | p[3]["psmask"][0][0] << 4 | p[5]["psmask"][0][0] << 2 | p[7]["psmask"][0][0]
	shuffle0R = p[1]["psmask"][0][2] << 6 | p[3]["psmask"][0][2] << 4 | p[5]["psmask"][0][2] << 2 | p[7]["psmask"][0][2]
	# Necessary shuffle for the second product (makes sense only if max() is used at least once in any of the four expr)
	shuffle1L = p[1]["psmask"][1][0] << 6 | p[3]["psmask"][1][0] << 4 | p[5]["psmask"][1][0] << 2 | p[7]["psmask"][1][0]
	shuffle1R = p[1]["psmask"][1][2] << 6 | p[3]["psmask"][1][2] << 4 | p[5]["psmask"][1][2] << 2 | p[7]["psmask"][1][2]
	
	# Sign mask for the operands of the first product
	invertvec0L = p[1]["psmask"][0][1] << 3 | p[3]["psmask"][0][1] << 2 | p[5]["psmask"][0][1] << 1 | p[7]["psmask"][0][1]
	invertvec0R = p[1]["psmask"][0][3] << 3 | p[3]["psmask"][0][3] << 2 | p[5]["psmask"][0][3] << 1 | p[7]["psmask"][0][3]
	# Sign mask for the operands of the second product
	invertvec1L = p[1]["psmask"][1][1] << 3 | p[3]["psmask"][1][1] << 2 | p[5]["psmask"][1][1] << 1 | p[7]["psmask"][1][1]
	invertvec1R = p[1]["psmask"][1][3] << 3 | p[3]["psmask"][1][3] << 2 | p[5]["psmask"][1][3] << 1 | p[7]["psmask"][1][3]
		
	code = ""
	
	# Emitting shuffles =========================
	if shuffle0L == 0xE4: # No shuffle necessary?
		code += "__m128 r0 = xmmbounds;\n"
	else:
		code += "__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,{0});\n".format(shuffle0L)
	if shuffle0R == 0xE4: # No shuffle necessary?
		code += "__m128 r1 = I.xmmbounds;\n"
	else:
		code += "__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,{0});\n".format(shuffle0R)
	
	use_max = p[1]["max"] or p[5]["max"] # At least one max is used => max() operation for all bounds
	
	if use_max:
		if shuffle1L == 0xE4: # No shuffle necessary?
			code += "__m128 r2 = xmmbounds;\n"
		else:
			code += "__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,{0});\n".format(shuffle1L)
		if shuffle1R == 0xE4: # No shuffle necessary?
			code += "__m128 r3 = I.xmmbounds;\n"
		else:
			code += "__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,{0});\n".format(shuffle1R)
		
	# Emitting sign flipping ==============================
	i = 0
	if invertvec0L != 0: # Some sign flipping necessary?
		code += flip_signs[invertvec0L]
		code += "r0 = _mm_xor_ps(r0,smask{0});\n".format(invertvec0L)
		i += 1
	if invertvec0R != 0: # Some sign flipping necessary?
		if invertvec0R != invertvec0L: # smask not already defined?
			code += flip_signs[invertvec0R]			
		code += "r1 = _mm_xor_ps(r1,smask{0});\n".format(invertvec0R)
	
	if use_max:
		if invertvec1L != 0: # Some sign flipping necessary?
			if invertvec1L != invertvec0L and invertvec1L != invertvec0R: # smask not already defined?
				code += flip_signs[invertvec1L]			
			code += "r2 = _mm_xor_ps(r2,smask{0});\n".format(invertvec1L)
		if invertvec1R != 0: # Some sign flipping necessary?
			if invertvec1R != invertvec0L and invertvec1R != invertvec0R and invertvec1R != invertvec1L: # smask not already defined?
				code += flip_signs[invertvec1R]						
			code += "r3 = _mm_xor_ps(r3,smask{0});\n".format(invertvec1R)
	
	# Emitting multiplications
	code += "__m128 r4 = _mm_mul_ps(r0,r1);\n"
	code += "__m128 ord4 = _mm_cmpord_ps(r4,r4);\n"
	code += "xmmbounds = _mm_and_ps(r4,ord4);\n"
	if use_max:
		code += "__m128 r5 = _mm_mul_ps(r2,r3);\n"
		code += "__m128 ord5 = _mm_cmpord_ps(r5,r5);\n"
		code += "r5 = _mm_and_ps(r5,ord5);\n"
		code += "xmmbounds = _mm_max_ps(xmmbounds, r5);\n"
	
	# Forcing some bounds to 0 if necessary
	if p[1]["iszero"]: # Left interval is 0
		code += "xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());\n"
	elif p[5]["iszero"]: # Right interval is 0
		code += "xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());\n"
		
	p[0] = code
		
		
	
def p_expr(p):
	"""expr : product
			| MAX LPAREN product COMMA product RPAREN
			| ZERO"""
	if p[1] == "max":
		p[0] = {"max": True, "iszero": False, "psmask": (p[3],p[5])}
	elif p[1] == "0":
		p[0] = {"max": False, "iszero": True, "psmask": ((0,0,0,0),(0,0,0,0))}
	else:
		p[0] = {"max": False, "iszero": False, "psmask": (p[1],p[1])}
		
def p_product(p):
	"""product : operand TIMES operand"""
	p[0] = p[1] + p[3] # Returning tuple (pos1, smask1, pos2, smask2)
	
def p_operand(p):
	"""operand : VAR
			   | LPAREN MINUS VAR RPAREN"""
	if p[1] == "(":
		pos = pos_operands[p[3]]
		invert = int((pos % 2) != 0) # Operands at even position in SSE reg are already stored negated
	else:
		pos = pos_operands[p[1]]	
		invert = int((pos % 2) == 0) # Operands at even position in SSE reg are stored negated
	p[0] = (pos, invert)
	
	
def p_error(p):
    print "Syntax error in input!"

lexer = lex.lex()
parser = yacc.yacc()

	 
formulas =dict()
formulas["0000"] = ("{y}*{t}","{x}*(-{z})")
formulas["0001"] = ("{y}*{t}","(-{x})*{t}")
formulas["0010"] = ("0","0")
formulas["0011"] = ("{y}*{z}","(-{x})*{t}")
formulas["0100"] = ("{y}*{t}","{y}*(-{z})")
formulas["0101"] = ("max((-{x})*(-{z}),{y}*{t})","max((-{x})*{t},{y}*(-{z}))")
formulas["0110"] = ("0","0")
formulas["0111"] = ("(-{x})*(-{z})","(-{x})*{t}")
formulas["1000"] = ("0","0")
formulas["1001"] = ("0","0")
formulas["1010"] = ("0","0")
formulas["1011"] = ("0","0")
formulas["1100"] = ("{x}*{t}","{y}*(-{z})")
formulas["1101"] = ("(-{x})*(-{z})","{y}*(-{z})")
formulas["1110"] = ("0","0")
formulas["1111"] = ("(-{x})*(-{z})","(-{y})*{t}")



#for x in range(18,19):
for x in range(0,256):
	# x^85: every other bit starting from lsb is flipped
	selector = bin(x)[2:].zfill(8) # Removing binary prefix "0b" and completing to 8 bits
	signbits = bin(x^85)[2:].zfill(8) # Removing binary prefix "0b" and completing to 8 bits
	signsHG = signbits[0:2]
	signsFE = signbits[2:4]
	signsDC = signbits[4:6]
	signsBA = signbits[6:8]
	signsHGDC = signsHG+signsDC
	signsFEBA = signsFE+signsBA
	
	SSEformula  = formulas[signsHGDC][0].format(x="c",y="d",z="g",t="h")
	SSEformula += " ; " + formulas[signsHGDC][1].format(x="c",y="d",z="g",t="h")
	SSEformula += " ; " + formulas[signsFEBA][0].format(x="a",y="b",z="e",t="f")
	SSEformula += " ; " + formulas[signsFEBA][1].format(x="a",y="b",z="e",t="f")
	
#	print SSEformula
	print "case {0}: // {1} => {2}\n{{".format(x,selector,signbits)
	print parser.parse(SSEformula),
	print "}\nbreak;"
