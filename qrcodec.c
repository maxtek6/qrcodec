#include "qrcodec.h"
#include "qrcodegen.h"
#include "quirc.h"

qrcodec_status qrcodec_encode(const qrcodec_data *data, qrcodec_qr *qr)
{
    qrcodegen_encodeBinary(
        data->payload_data,
        data->payload_size,
        qr->bitmap_data,
        data->ecc,
        data->min_version,
        data->max_version,
        data->mask,
        false);
}

qrcodec_status qrcodec_decode(const qrcodec_qr *qr, qrcodec_data *data)
{
    struct quirc *decoder = quirc_new();
    size_t size;
    uint8_t *buffer;
    struct quirc_code decoder_input;
    struct quirc_data decoder_output;

    quirc_resize(decoder, size, size);
    buffer = quirc_begin(decoder, NULL, NULL);
    memcpy(qr->bitmap_data, buffer, qr->bitmap_size);
    quirc_end(decoder);
    quirc_extract(decoder, 0, &decoder_input);
    quirc_decode(&decoder_input, &decoder_output);
    quirc_destroy(decoder);
}

qrcodec_status qrcodec_qr_get_size(const qrcodec_qr *qr, size_t *size)
{
    *size = qrcodegen_getSize(qr->bitmap_data);
}

qrcodec_status qrcodec_qr_get_value(const qrcodec_qr *qr, size_t x, size_t y, bool *value)
{
    *value = qrcodegen_getModule(qr->bitmap_data, x, y);
}