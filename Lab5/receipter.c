#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "receipter.h"

#define MAX_ITEM_COUNT (10)
#define MAX_ITEM_PRICE (999.99)
#define MAX_ITEM_NAME_LENGTH (25)

#define MAX_RECEIPT_COUNT (99999)
#define MAX_RECEIPT_MESSAGE_LENGTH (75)

#define RESTAURANT_NAME ("Charles' Seafood")
#define RESTAURANT_TAX_NUMBER ("Tax#-51234")

#define TAX_RATE (0.05)


static size_t s_item_count = 0;
static char s_item_names[MAX_ITEM_COUNT][MAX_ITEM_NAME_LENGTH + 1] = { 0, };
static double s_item_prices[MAX_ITEM_COUNT] = { 0, };

static receipt_flag_t s_receipt_flag = RECEIPT_FLAG_EMPTY;
static double s_tip_price = 0.;
static char s_receipt_message[MAX_RECEIPT_MESSAGE_LENGTH + 1];


/* required */
int add_item(const char* name, double price)
{
    assert(*name != '\0');
    assert(price <= MAX_ITEM_PRICE);

    if (s_item_count < MAX_ITEM_COUNT) {
        strncpy(s_item_names[s_item_count], name, sizeof(s_item_names[0]));
        s_item_names[s_item_count][sizeof(s_item_names[0]) - 1] = '\0';

        s_item_prices[s_item_count] = price;

        ++s_item_count;
        return TRUE;
    }

    return FALSE;
}

void set_tip(double tip)
{
    assert(tip >= 0.0);

    s_tip_price = tip;

    if (s_tip_price == 0.0) {
        clear_receipt_flag(&s_receipt_flag, RECEIPT_FLAG_TIP);
        return;
    }

    set_receipt_flag(&s_receipt_flag, RECEIPT_FLAG_TIP);
    return;
}

void set_message(const char* message)
{
    assert(message != NULL);

    if (*message == '\0') {
        clear_receipt_flag(&s_receipt_flag, RECEIPT_FLAG_MESSAGE);
        return;
    }

    strncpy(s_receipt_message, message, sizeof(s_receipt_message));
    s_receipt_message[sizeof(s_receipt_message) - 1] = '\0';

    set_receipt_flag(&s_receipt_flag, RECEIPT_FLAG_MESSAGE);
    return;
}

int print_receipt(const char* filename, time_t timestamp)
{
    const size_t MAX_LINE_LENGTH = 50;

    static unsigned int s_receipt_count = 0;

    int b_out;
    FILE* stream;

    if (s_item_count == 0) {
        b_out = FALSE;
        goto out;
    }

    assert(s_receipt_count <= MAX_RECEIPT_COUNT);

    stream = fopen(filename, "wb");
    if (stream == NULL) {
        b_out = FALSE;
        goto out;
    }

    {
        fprintf(stream, "%s\n", RESTAURANT_NAME);
        fprintf(stream, "--------------------------------------------------\n");

        {
            struct tm tm = *gmtime(&timestamp);
            fprintf(stream, "%04d-%02d-%02d %02d:%02d:%02d                          %05u\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, s_receipt_count);
            fprintf(stream, "--------------------------------------------------\n");
        }

        {
            double subtotal = 0.;
            double total = 0.;

            {
                size_t i;

                for (i = 0; i < s_item_count; ++i) {
                    fprintf(stream, "%33s%17.2f\n", s_item_names[i], s_item_prices[i]);
                    subtotal += s_item_prices[i];
                }
            }

            fprintf(stream, "\n%33s%17.2f\n", "Subtotal", subtotal);
            total += subtotal;

            if (is_include_receipt_flag(s_receipt_flag, RECEIPT_FLAG_TIP)) {
                fprintf(stream, "%33s%17.2f\n", "Tip", s_tip_price);
                total += s_tip_price;
            }

            {
                const double tax = subtotal * TAX_RATE;
                fprintf(stream, "%33s%17.2f\n", "Tax", tax);
                total += tax;
            }

            fprintf(stream, "%33s%17.2f\n\n", "Total", total);
        }

        if (is_include_receipt_flag(s_receipt_flag, RECEIPT_FLAG_MESSAGE)) {
            const char* p_receipt_message = s_receipt_message;

            while (*p_receipt_message != '\0') {
                if (p_receipt_message != s_receipt_message && ((p_receipt_message - s_receipt_message) % MAX_LINE_LENGTH) == 0) {
                    fprintf(stream, "\n");
                }

                fprintf(stream, "%c", *p_receipt_message);
                ++p_receipt_message;
            }

            fprintf(stream, "\n");
        }

        fprintf(stream, "==================================================\n");
        fprintf(stream, "%50s", RESTAURANT_TAX_NUMBER);
    }

    fclose(stream);
    ++s_receipt_count;
    b_out = TRUE;

out:
    s_item_count = 0;
    s_receipt_flag = RECEIPT_FLAG_EMPTY;

    return b_out;
}


/* my function */
void set_receipt_flag(receipt_flag_t* const out_flag, const receipt_flag_t set_flag)
{
    *out_flag |= set_flag;
}

void clear_receipt_flag(receipt_flag_t* const out_flag, const receipt_flag_t clear_flag)
{
    *out_flag &= ~clear_flag;
}

int is_include_receipt_flag(const receipt_flag_t flag, const receipt_flag_t is_include_flag)
{
    return flag & is_include_flag ? TRUE : FALSE;
}
