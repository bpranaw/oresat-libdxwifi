/**
 *  radiotap.h
 * 
 *  DESCRIPTION: Headers for Radiotap.c
 * 
 *  https://github.com/oresat/oresat-dxwifi-software
 * 
 */

#ifndef LIBDXWIFI_RADIOTAP_H
#define LIBDXWIFI_RADIOTAP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <asm/types.h>

#include <linux/types.h>
#include <linux/errno.h>

#include <libdxwifi/details/ieee80211.h>

typedef __u32 __bitwise _le32;
typedef __u16 __bitwise _le16;

struct __una_u16 { uint16_t x __attribute__((packed));};
struct __una_u32 { uint32_t x __attribute__((packed));};

struct radiotap_override {
    uint8_t field;
    uint8_t align:4, size:4;
};

struct radiotap_align_size {
    uint8_t align:4, size:4;
};

struct radiotap_namespace {
    const struct radiotap_align_size * align_size;
    int n_bits;
    uint32_t oui;
    uint8_t subns;
};

struct radiotap_vendor_namespace{
    const struct radiotap_namespace * ns;
    int n_ns;
};

//See linux/blob/master/include/net/cfg80211.h for desc.
//Ref: ieee80211_radiotap_iterator
struct ieee80211_radiotap_iterator{
    struct ieee80211_radiotap_header * _rtheader;
    const struct radiotap_vendor_namespace * _vns;
    const struct radiotap_namespace * current_namespace;
    u_char *_arg;  //Pointer to next argument
    u_char *_next_ns_data; //beginning of next namespace's data.
    __le32 *_next_bitmap; //Internal pointer to next present u32
    u_char *_this_arg; // Current Arg.
    int _this_arg_index; // Index of current arg
    int _this_arg_size; // length of current arg.
    int is_radiotap_ns; //Is the current namespace the default radiotap namespace?
    uint64_t _max_length; //Length of radiotap header in CPU Byte Order
    int _arg_index; //Next argument index
    uint32_t _bitmap_shifter; //Internal shifter for current u32 butmap, bit 0 set, arg present.
    int _reset_on_ext;  //Reset arg index to 0 when advancing to next bitmap word

};

/*
 *
 * Radiotap header contains captured flags from the radiotap header
 * 
 * 
 */
 typedef struct radiotap_header_data {
    //Capture Flags, Derived from IE80211.h, Transmitter.h, & Radiotap.org
   
    uint32_t TSFT[2]; //Unit: Microseconds
    uint8_t  Flags; //Unit: Bitmap. See: /radiotap/fields/flags
    uint16_t ChannelFreq; //Unit: MHz. (TX/RX Frequency)
    uint16_t ChannelFlags; //Unit: Bitmap.  See: /radiotap/fields/Channel
    int8_t   dBm_AntSignal;
    uint16_t Rx_Flags; //Bitmap
    uint8_t  MCS_Known;
    uint8_t  MCS_Flags;
    uint8_t  MCS_MCS; //Rate Index (IEEE 802.11N-2009)
} radiotap_header_data;

int radiotap_iterator_init(struct ieee80211_radiotap_iterator *iterator, struct ieee80211_radiotap_header *radiotap_header, long unsigned int max_length, const struct radiotap_vendor_namespace *vns);

int radiotap_iterator_next(struct ieee80211_radiotap_iterator *iterator);

int run_parser(struct radiotap_header_data *output_data, const struct ieee80211_radiotap_header *actual_header, int length_data);

#endif //LIBDXWIFI_RADIOTAP_H