#ifndef RECEIPTER_H
#define RECEIPTER_H

#include <time.h>

#define TRUE (1)
#define FALSE (0)

typedef enum receipt_flag {
    RECEIPT_FLAG_EMPTY = 0,
    RECEIPT_FLAG_TIP = 1 << 0,
    RECEIPT_FLAG_MESSAGE = 1 << 1
} receipt_flag_t;


/* required */
int add_item(const char* name, double price);

void set_tip(double tip);

void set_message(const char* message);

int print_receipt(const char* filename, time_t timestamp);


/* my function */
void set_receipt_flag(receipt_flag_t* const out_flag, const receipt_flag_t set_flag);

void clear_receipt_flag(receipt_flag_t* const out_flag, const receipt_flag_t clear_flag);

int is_include_receipt_flag(const receipt_flag_t flag, const receipt_flag_t is_include_flag);


#endif /* RECEIPTER_H */
