#include <sys/regdef.h>

#include <sys/syscall.h>

	.rdata
	.align 2
.eqv STRING_HASH_INIT, 1
.eqv STRING_HASH_MORE, 2
.equ STRING_HASH_DONE, 3

	.text

	.abicalls
	.align 2

	.globl	string_hash_more
	.ent	string_hash_more
string_hash_more:
	subu	sp, sp, 40	## 4 (SRA) + 2 (LTA) + 4 (ABA)
	.cprestore 24		## sw gp, 24(sp)
	sw	fp, 28(sp)
	sw	ra, 32(sp)
	move	fp, sp		## fp = sp
	
	##salvo 1er arg (siempre)
	sw  a0, 40(fp)	## n: a0, sp+40 ##hash
	sw  a1, 44(fp)  ##str*
	sw  a2, 48(fp)  ##len

	## assert(sh->flag == STRING_HASH_INIT || sh->flag == STRING_HASH_MORE);
	
	lw t8, 40(fp) 			## cargamos la direccion del struct
	lb t2, 0(t8) 			## accedo al valor que esta dentro de la direccion que tiene t8 (el flag)
	beq t2, STRING_HASH_INIT, _assert_true
	beq t2, STRING_HASH_MORE, _assert_true
	j _error

	## if (sh->flag == STRING_HASH_INIT)

_assert_true:
	beq t2, STRING_HASH_INIT, move_str

	##while ((*str) != 0 && len--)

while:
	lb t1,0(a1)			## cargamos en t1 el valor del str
	bne t1, zero, _cond2		## chequeamos condicion 1 (*str != 0)
_cond2: bne a2,zero, loop		## chequeamos condicion 2 (len != 0)
	j _return

move_str:
	##sh->flag = STRING_HASH_MORE;
	addu t2, zero, STRING_HASH_MORE
	sb t2,0(t8) 			## cargamos el valor de la constante en el flag del struct
	##sh->hash = (*str) << 7;
	lb t7, 0(a1)			## cargamos en t7 *str (un char)
	sll t7, t7, 7			## hacemos sll en 7 para t7
	sw t7,4(t8)			## guardamos el valor del hash en el struct 
	j while
loop: 
	subu a2, a2, 1                  ## decrementamos el len en 1 (len--)

	##sh->hash = (1000003 * sh->hash) ^ *str++    
	
	lw t3,4(t8)			## cargamos en t3 el valor del hash 
	li t6,983040			## cargamos en t6 0xf0000
	ori t6,t6,0x4243		## OR con 0x4243 para que en t6 quede 1000003
	mul t3 ,t3,t6			## hacemos la multiplicacion
	
	
	addu a1,a1,1			## incrementamos en 1 al str
	xor t3, t1,t3 			## XOR
	sw t3,4(t8)			## guardamos en memoria el valor de hash

	##sh->size++;			
        lw t4,8(t8)			## cargamos el size en t4	
	addu t4,t4,1			## aumentamos en 1 el size
	sw t4, 8(t8)			## guardamos el size en memoria
	j while				## vuelta a la condicion del while




_error:
	li	v0,-1			
	j	_return

_return:
	## return;
	lw	gp, 24(sp)
	lw	fp, 28(sp)
	lw	ra, 32(sp)
	addu	sp, sp, 40
	jr	ra

	.end string_hash_more
	.size	string_hash_more,.-string_hash_more
