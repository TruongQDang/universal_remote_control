#ifndef _HELPER_FUNC_H_
#define _HELPER_FUNC_H_

#include <stdint.h>

static inline void helper_set_bit(volatile uint8_t *regs, uint8_t bit_position)
{
        *regs |= (1 << bit_position);
}

static inline void helper_clear_bit(volatile uint8_t *regs, uint8_t bit_position)
{
        *regs &= ~(1 << bit_position);
}


static inline uint8_t helper_check_bit(volatile uint8_t *regs, uint8_t bit_position)
{
        return (*regs & (1 << bit_position));
}

static inline void helper_clear_flag(volatile uint8_t *regs, uint8_t flag_position) 
{
        *regs = (1 << flag_position);
}
#endif