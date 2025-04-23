/*
   atomic.h -- support for atomic action (compare and swap)
 */

/* Atomically store NEWVAL in *MEM if *MEM is equal to OLDVAL and set eqz;
   o/w clear eqz. Always return the old *MEM value.
   The conditional atomic swap is performed if the return value == OLDVAL. */

#ifdef i386  // si la arquitectura es de 32 bits (x86)
# define __arch_compare_and_exchange_val_32_acq(mem, newval, oldval)	\
  ({ __typeof (*mem) ret; \
    /* instruccion ensamblador para hacer cmpxchg en 32 bits */ \
    __asm __volatile ("lock\n" "cmpxchgl %2, %1\n"		 \
		      : "=a" (ret), "=m" (*mem)			 \
		      : "r" (newval), "m" (*mem), "0" (oldval)); \
    ret; })

// define alias para que el usuario use una sola macro generica
# define compare_and_swap(mem, newval, oldval)			 \
  __arch_compare_and_exchange_val_32_acq(mem, newval, oldval)

#else  // si la arquitectura es de 64 bits (amd64)
# define __arch_compare_and_exchange_val_64_acq(mem, newval, oldval)	\
  ({ __typeof (*mem) ret; \
    /* instruccion ensamblador para hacer cmpxchg en 64 bits */ \
    __asm __volatile ("lock\n" "cmpxchgq %q2, %1\n"		 \
		       : "=a" (ret), "=m" (*mem)		 \
		       : "r" ((long int) (newval)), "m" (*mem),	 \
			 "0" ((long int) (oldval)));		 \
     ret; })

// define alias para que el usuario use una sola macro generica
# define compare_and_swap(mem, newval, oldval)			 \
  __arch_compare_and_exchange_val_64_acq(mem, newval, oldval)

#endif


// sacado de internet no se dnd lo perdi . 