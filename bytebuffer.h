#ifndef BYTEBUFFER_H_INCLUDED
#define BYTEBUFFER_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

typedef struct ByteBuffer {
	size_t		size, count;
	uint8_t		*data;
} ByteBuffer;


struct ByteBuffer	*ByteBuffer_New(void);
void		ByteBuffer_Init		(struct ByteBuffer *);
size_t		ByteBuffer_Len		(const struct ByteBuffer *);
void		ByteBuffer_Insert	(struct ByteBuffer *, const uint8_t);
void		ByteBuffer_InsertInt	(struct ByteBuffer *, const uint64_t, const size_t);
void		ByteBuffer_InsertFloat	(struct ByteBuffer *, const float);
void		ByteBuffer_InsertDouble	(struct ByteBuffer *, const double);
void		ByteBuffer_InsertString	(struct ByteBuffer *, const char *, const size_t);
void		ByteBuffer_InsertObject	(struct ByteBuffer *, const void *, const size_t);
void		ByteBuffer_Set		(struct ByteBuffer *, const size_t, const uint8_t);
uint8_t		ByteBuffer_Get		(const struct ByteBuffer *, const size_t);
void		ByteBuffer_Delete	(struct ByteBuffer *, const size_t);
void		ByteBuffer_Free		(struct ByteBuffer *);
void		ByteBuffer_Print	(struct ByteBuffer *);
void		ByteBuffer_Resize	(struct ByteBuffer *);
void		ByteBuffer_DumpToFile(struct ByteBuffer *, const char *);
void		ByteBuffer_ReadFromFile(struct ByteBuffer *, FILE *);
void		ByteBuffer_Append	(struct ByteBuffer *, struct ByteBuffer *);

#ifdef __cplusplus
}
#endif

#endif	// BYTEBUFFER_H_INCLUDED
