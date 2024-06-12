#ifndef QRCODEC_H
#define QRCODEC_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define QRCODEC_MIN_VERSION 1
#define QRCODEC_MAX_VERSION 40
#define QRCODEC_VERSION_BUFFER_SIZE(v) ((((v) * 4 + 17) * ((v) * 4 + 17) + 7) / 8 + 1)
#define QRCODEC_MAX_BUFFER_SIZE QRCODEC_VERSION_BUFFER_SIZE(QRCODEC_MAX_VERSION)

typedef enum
{
    QRCODEC_STATUS_SUCCESS,
    QRCODEC_STATUS_INVALID_HANDLE,
} qrcodec_status;

typedef enum
{
    QRCODEC_ECC_LOW,
    QRCODEC_ECC_MEDIUM,
    QRCODEC_ECC_QUARTILE,
    QRCODEC_ECC_HIGH,
} qrcodec_ecc;

typedef enum
{
    QRCODEC_MASK_AUTO = -1,
    QRCODEC_MASK_0 = 0,
    QRCODEC_MASK_1,
    QRCODEC_MASK_2,
    QRCODEC_MASK_3,
    QRCODEC_MASK_4,
    QRCODEC_MASK_5,
    QRCODEC_MASK_6,
    QRCODEC_MASK_7,
} qrcodec_mask;

typedef enum
{
    QRCODEC_TYPE_NUMERIC,
    QRCODEC_TYPE_ALPHANUMERIC,
    QRCODEC_TYPE_BYTE,
    QRCODEC_TYPE_KANJI,
} qrcodec_type;

typedef struct
{
    qrcodec_ecc ecc;
    qrcodec_mask mask;
    qrcodec_type type;
    int min_version;
    int max_version;
    int version;
    uint8_t *payload_data;
    size_t payload_size;
} qrcodec_data;

typedef struct
{
    uint8_t *bitmap_data;
    size_t bitmap_size;
} qrcodec_qr;

qrcodec_status qrcodec_encode(const qrcodec_data *data, qrcodec_qr *qr);

qrcodec_status qrcodec_decode(const qrcodec_qr *qr, qrcodec_data *data);

qrcodec_status qrcodec_qr_get_size(const qrcodec_qr *qr, size_t *size);

qrcodec_status qrcodec_qr_get_value(const qrcodec_qr *qr, size_t x, size_t y, bool *value);

#endif