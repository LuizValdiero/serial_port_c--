#include <data_structure/serie.h>

int serie_mount(buffer_t * out, int *displacement, void * data) {
    serie_t * serie = (serie_t *) data;
    uint32_t size_serie = sizeof(serie_t);
    if (out->buffer_size < *displacement + size_serie)
        return CODE_ERROR_SHORT_BUFFER;
    
    memcpy(out->buffer + *displacement, serie, size_serie);
    *displacement += size_serie;
    return CODE_SUCCESS;
}