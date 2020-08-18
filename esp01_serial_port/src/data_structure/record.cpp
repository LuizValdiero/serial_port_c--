#include <data_structure/record.h>

int record_mount(buffer_t * out, int *displacement, void * data) {
    record_t * record = (record_t *) data;
    uint32_t size_record = sizeof(record_t);
    if (out->buffer_size < *displacement + size_record)
        return CODE_ERROR_SHORT_BUFFER;
    
    memcpy(out->buffer + *displacement, record, size_record);
    *displacement += size_record;
    return CODE_SUCCESS;
}