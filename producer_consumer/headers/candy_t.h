#ifndef CANDY_T
#define CANDY_T

typedef struct  {
    int factory_number;
    double time_stamp_in_ms;
} candy_t;

candy_t* candy_create(const int f_number);

double current_time_in_ms(void);

#endif
