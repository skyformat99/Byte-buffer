#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "inttypes.h"
#include "iso646.h"
#include "bytebuffer.h"


struct ByteBuffer *ByteBuffer_New(void)
{
	return calloc(1, sizeof(struct ByteBuffer));
}

void ByteBuffer_Init(struct ByteBuffer *p)
{
	if( !p )
		return;
	p->data = NULL;
	p->size = p->count = 0;
}

size_t ByteBuffer_Len(const struct ByteBuffer *p)
{
	return p ? p->count : 0;
}

void ByteBuffer_Insert(struct ByteBuffer *p, const uint8_t byte)
{
	if( !p )
		return;
	else if( p->count >= p->size )
		ByteBuffer_Resize(p);
	
	p->data[p->count++] = byte;
}

void ByteBuffer_InsertInt(struct ByteBuffer *p, const uint64_t value, const size_t bytes)
{
	if( !p )
		return;
	else if( p->count+bytes >= p->size )
		ByteBuffer_Resize(p);
	
	switch( bytes ) {
		case 2:
			memcpy(p->data+p->count, &value, 2);
			p->count += 2;
			break;
		case 4:
			memcpy(p->data+p->count, &value, 4);
			p->count += 4;
			break;
		default:	// assume default as 8 bytes.
			memcpy(p->data+p->count, &value, 8);
			p->count += 8;
			break;
	}
}

void ByteBuffer_InsertFloat(struct ByteBuffer *p, const float fval)
{
	if( !p )
		return;
	else if( p->count+sizeof(float) >= p->size )
		ByteBuffer_Resize(p);
	
	memcpy(p->data+p->count, &fval, sizeof(float));
	p->count += sizeof(float);
}

void ByteBuffer_InsertDouble(struct ByteBuffer *p, const double fval)
{
	if( !p )
		return;
	else if( p->count+sizeof(double) >= p->size )
		ByteBuffer_Resize(p);
	
	memcpy(p->data+p->count, &fval, sizeof(double));
	p->count += sizeof(double);
}

void ByteBuffer_InsertString(struct ByteBuffer *restrict p, const char *restrict str, const size_t strsize)
{
	if( !p )
		return;
	else if( p->count+strsize+1 >= p->size )
		ByteBuffer_Resize(p);
	
	while( p->count+strsize+1 >= p->size )
		ByteBuffer_Resize(p);
	
	memcpy(p->data+p->count, str, strsize);
	p->count += strsize;
	p->data[p->count++] = 0;	// add null terminator.
}

void ByteBuffer_InsertObject(struct ByteBuffer *restrict p, const void *restrict o, const size_t size)
{
	if( !p )
		return;
	else if( p->count+size >= p->size )
		ByteBuffer_Resize(p);
	
	while( p->count+size >= p->size )
		ByteBuffer_Resize(p);
	
	memcpy(p->data+p->count, o, size);
	p->count += size;
}

void ByteBuffer_Set(struct ByteBuffer *p, const size_t index, const uint8_t byte)
{
	if( !p or index >= p->count )
		return;
	
	p->data[index] = byte;
}

uint8_t ByteBuffer_Get(const struct ByteBuffer *p, const size_t index)
{
	return (!p or index >= p->count) ? 0 : p->data[index];
}

void ByteBuffer_Delete(struct ByteBuffer *p, const size_t index)
{
	if( !p or index >= p->count )
		return;
	
	for( size_t i=index+1, j=index ; i < p->count ; i++ )
		p->data[j++] = p->data[i];
	
	p->count--;
}

void ByteBuffer_Free(struct ByteBuffer *p)
{
	if( !p )
		return;
	
	if( p->data )
		free(p->data);
	ByteBuffer_Init(p);
}

void ByteBuffer_Print(struct ByteBuffer *p)
{
	if( !p )
		return;
	
	printf("ByteBuffer Size: %u\n", p->size);
	printf("ByteBuffer Count: %u\n", p->count);
	for( size_t i=0 ; i<p->count ; i++ )
		printf("ByteBuffer Index: ]%" PRIuPTR "] == \'%u\'\n", i, p->data[i]);
}

void ByteBuffer_Resize(struct ByteBuffer *p)
{
	if( !p )
		return;
	
	// first we get our old size.
	// then resize the actual size.
	size_t oldsize = p->size;
	p->size <<= 1;
	if( p->size==0 )
		p->size=8;
	
	// allocate new table.
	uint8_t *newdata = calloc(p->size, sizeof(uint8_t));
	assert( newdata );
	
	// copy the old table to new then free old table.
	if( p->data ) {
		memcpy(newdata, p->data, oldsize);
		free(p->data);
	}
	p->data = newdata;
}

void ByteBuffer_DumpToFile(struct ByteBuffer *restrict p, const char *restrict filename)
{
	if( !p or !p->data )
		return;
	
	FILE *file = fopen(filename, "wb");
	if( !file )
		return;
	
	fwrite(p->data, sizeof(uint8_t), p->count, file);
}

void ByteBuffer_ReadFromFile(struct ByteBuffer *restrict p, FILE *restrict file)
{
	if( !p or !p->data or !file )
		return;
	
	// get the total file size.
	uint64_t filesize = 0;
	if( !fseek(pFile, 0, SEEK_END) ) {
		filesize = (uint64_t)ftell(pFile);
		rewind(pFile);
	}
	
	// check if buffer can hold it.
	// if not, resize until it can.
	if( p->count+filesize >= p->size )
		ByteBuffer_Resize(p);
	
	while( p->count+filesize >= p->size )
		ByteBuffer_Resize(p);
	
	// read in the data.
	fread(p->data, sizeof(uint8_t), filesize, file);
	p->count += filesize;
}

void ByteBuffer_Append(struct ByteBuffer *restrict p, struct ByteBuffer *o)
{
	if( !p or !o )
		return;
	
	if( p->count+o->count >= p->size )
		ByteBuffer_Resize(p);
	
	while( p->count+o->count >= p->size )
		ByteBuffer_Resize(p);
	
	memcpy(p->data+p->count, o->data, o->count);
	p->count += o->count;
}
