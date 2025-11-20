#include "ringbuf.h"

/*
 * Atomic helpers:
 *
 * We try to use existing macros if provided by platform.h:
 *  - If IS_ATOMIC_OFF / ATOMIC_ON / ATOMIC_OFF are defined, we use them and preserve prior state.
 *  - Else if GLOBAL_INTERRUPT_DISABLE / GLOBAL_INTERRUPT_ENABLE exist, we disable/enable around
 *    critical sections (best-effort; we cannot detect prior state in that fallback).
 *
 * If your platform provides a different saved-state API, replace these helpers accordingly.
 */

static inline int atomic_start(void)
{
#ifdef IS_ATOMIC_OFF
    /* IS_ATOMIC_OFF returns non-zero when interrupts are OFF in your original code.
       The original code's use was confusing; we assume:
         IS_ATOMIC_OFF() == 0 -> interrupts enabled
         IS_ATOMIC_OFF() != 0 -> interrupts disabled
       To preserve prior state we enable interrupts only if they were enabled previously.
       However original macros naming may differ on your platform; adapt if needed. */
    int was_off = IS_ATOMIC_OFF();
    if (!was_off) {
        /* interrupts were enabled -> disable them for critical section */
#ifdef ATOMIC_OFF
        ATOMIC_OFF(); /* naming differs per platform; some use ATOMIC_OFF to disable */
#else
#ifdef ATOMIC_ON
        /* if platform uses ATOMIC_ON to enable, try to use ATOMIC_OFF if available */
        /* no-op if not available */
#endif
#endif
    }
    return was_off ? 1 : 0;
#elif defined(GLOBAL_INTERRUPT_DISABLE)
    GLOBAL_INTERRUPT_DISABLE();
    return 1; /* we disabled interrupts (best-effort) */
#else
    /* No atomic support available; return 0 (no change). */
    return 0;
#endif
}

static inline void atomic_end(int token)
{
#ifdef IS_ATOMIC_OFF
    if (!token) {
        /* token==0 means interrupts were enabled before atomic_start, so re-enable them */
#ifdef ATOMIC_ON
        ATOMIC_ON();
#else
#ifdef GLOBAL_INTERRUPT_ENABLE
        GLOBAL_INTERRUPT_ENABLE();
#endif
#endif
    }
#elif defined(GLOBAL_INTERRUPT_ENABLE)
    /* If we used GLOBAL_INTERRUPT_DISABLE earlier, re-enable unconditionally */
    (void) token;
    GLOBAL_INTERRUPT_ENABLE();
#else
    (void) token;
    /* nothing to do */
#endif
}

/* Initialize ring buffer */
void ringbuf_init(ringbuf_struct_t *rb, mos_uint8_t *data_buffer, mos_uint16_t data_buffer_size)
{
    if (!rb || !data_buffer || data_buffer_size == 0) return;
    rb->buffer = data_buffer;
    rb->buffer_size = data_buffer_size;
    /* reset state in an atomic-safe way */
    int tok = atomic_start();
    rb->write_index = 0;
    rb->read_index = 0;
    rb->counter = 0;
    atomic_end(tok);
}

/* Write a byte. Returns true on success, false if buffer full. */
bool ringbuf_write(ringbuf_struct_t *rb, mos_uint8_t data)
{
    if (!rb || !rb->buffer || rb->buffer_size == 0) return false;

    int tok = atomic_start();

    if (rb->counter == rb->buffer_size) {
        /* full */
        atomic_end(tok);
        return false;
    }

    rb->buffer[rb->write_index] = data;
    rb->write_index++;
    if (rb->write_index >= rb->buffer_size)
        rb->write_index = 0;

    rb->counter++;

    atomic_end(tok);
    return true;
}

/* Read a byte. Returns true and writes to *out if not empty. */
bool ringbuf_read(ringbuf_struct_t *rb, mos_uint8_t *out)
{
    if (!rb || !rb->buffer || rb->buffer_size == 0 || !out) return false;

    int tok = atomic_start();

    if (rb->counter == 0) {
        /* empty */
        atomic_end(tok);
        return false;
    }

    *out = rb->buffer[rb->read_index];
    rb->read_index++;
    if (rb->read_index >= rb->buffer_size)
        rb->read_index = 0;

    rb->counter--;

    atomic_end(tok);
    return true;
}

mos_uint16_t ringbuf_count(const ringbuf_struct_t *rb)
{
    if (!rb) return 0;
    /* reading counter is atomic on most platforms for uint16; if not, caller should call under atomic context */
    return rb->counter;
}

mos_uint16_t ringbuf_capacity(const ringbuf_struct_t *rb)
{
    if (!rb) return 0;
    return rb->buffer_size;
}

bool ringbuf_is_full(const ringbuf_struct_t *rb)
{
    if (!rb) return false;
    return (rb->counter == rb->buffer_size);
}

void ringbuf_flush(ringbuf_struct_t *rb)
{
    if (!rb) return;
    int tok = atomic_start();
    rb->write_index = 0;
    rb->read_index = 0;
    rb->counter = 0;
    atomic_end(tok);
}
