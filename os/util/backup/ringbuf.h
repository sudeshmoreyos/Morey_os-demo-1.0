#ifndef RINGBUF_H
#define RINGBUF_H

#include "../../platform.h" /* provides mos_uint8_t, mos_uint16_t, and (optionally) interrupt macros */
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ringbuf_struct {
    mos_uint8_t *buffer;       /* user-provided array */
    mos_uint16_t buffer_size;  /* total slots in buffer */
    mos_uint16_t write_index;  /* next write position */
    mos_uint16_t read_index;   /* next read position */
    mos_uint16_t counter;      /* number of bytes currently stored (0..buffer_size) */
} ringbuf_struct_t;

/* Initialize ringbuf with a user-provided buffer (no allocation). */
void ringbuf_init(ringbuf_struct_t *rb, mos_uint8_t *data_buffer, mos_uint16_t data_buffer_size);

/* Write one byte. Returns true on success, false if buffer is full. */
bool ringbuf_write(ringbuf_struct_t *rb, mos_uint8_t data);

/* Read one byte. Returns true and stores byte in *out if buffer not empty; false if empty. */
bool ringbuf_read(ringbuf_struct_t *rb, mos_uint8_t *out);

/* Return number of bytes currently in buffer (0..buffer_size). */
mos_uint16_t ringbuf_count(const ringbuf_struct_t *rb);

/* Return total capacity (buffer_size). */
mos_uint16_t ringbuf_capacity(const ringbuf_struct_t *rb);

/* Return true if buffer is full. */
bool ringbuf_is_full(const ringbuf_struct_t *rb);

/* Flush/reset the buffer (empties it). */
void ringbuf_flush(ringbuf_struct_t *rb);

#ifdef __cplusplus
}
#endif

#endif /* RINGBUF_H */
