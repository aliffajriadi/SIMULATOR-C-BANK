// bank.h
#ifndef BANK_H
#define BANK_H

#include <stdbool.h>

#define MAX_ACCOUNTS 50
#define START_ACCOUNT_NO 1001
#define MAX_NAME_LEN 40
#define MAX_WITHDRAWALS_PER_DAY 3
#define MAX_WITHDRAWAL_AMOUNT 500.0

typedef struct {
    int accountNo;
    int pin;
    double balance;
    char name[MAX_NAME_LEN];
    int failed_logins;
    int withdrawals_today;
    bool locked;
    bool used;
} Account;

void init_bank();
int create_account(const char *name, int pin, double initial_balance);
int find_account_index_by_no(int accountNo);
void simulate_new_day();
void show_accounts_summary();
double compute_interest_preview(double balance, double monthly_rate_percent);

int get_account_count();
Account* get_accounts();

#endif
