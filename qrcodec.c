#include "qrcodec.h"
#include "qrcodegen.h"
#include "quirc.h"
#include <stdio.h>

qrcodec_status qrcodec_encode(const qrcodec_data *data, qrcodec_qr *qr)
{
    static const enum qrcodegen_Mode mode_conversion[] = {
        qrcodegen_Mode_NUMERIC,
        qrcodegen_Mode_ALPHANUMERIC,
        qrcodegen_Mode_BYTE,
        qrcodegen_Mode_KANJI,
    };
    const struct qrcodegen_Segment segment = {
        .mode = mode_conversion[data->type],
        .data = data->payload_data,
        .numChars = data->payload_size,
        .bitLength = data->payload_size * 8,
    };
    
    if(!qrcodegen_encodeSegments(&segment, 1, data->ecc, data->payload_data, qr->bitmap_data))
    {
        puts("error");
    }
    return QRCODEC_STATUS_SUCCESS;
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
    return QRCODEC_STATUS_SUCCESS;
}

qrcodec_status qrcodec_qr_get_size(const qrcodec_qr *qr, size_t *size)
{
    *size = qrcodegen_getSize(qr->bitmap_data);
    return QRCODEC_STATUS_SUCCESS;
}

qrcodec_status qrcodec_qr_get_value(const qrcodec_qr *qr, size_t x, size_t y, bool *value)
{
    *value = qrcodegen_getModule(qr->bitmap_data, x, y);
    return QRCODEC_STATUS_SUCCESS;
}