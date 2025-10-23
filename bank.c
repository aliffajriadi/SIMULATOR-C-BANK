// bank.c
#include "bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Account accounts[MAX_ACCOUNTS];
static int account_count = 0;
static int next_account_no = START_ACCOUNT_NO;

void init_bank() {
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        accounts[i].used = false;
        accounts[i].locked = false;
    }
    account_count = 0;
    next_account_no = START_ACCOUNT_NO;
}

int create_account(const char *name, int pin, double initial_balance) {
    if (account_count >= MAX_ACCOUNTS) return -1;
    int idx = -1;
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        if (!accounts[i].used) { idx = i; break; }
    }
    if (idx == -1) return -1;

    accounts[idx].used = true;
    accounts[idx].accountNo = next_account_no++;
    accounts[idx].pin = pin;
    accounts[idx].balance = initial_balance;
    strncpy(accounts[idx].name, name, MAX_NAME_LEN - 1);
    accounts[idx].name[MAX_NAME_LEN - 1] = '\0';
    accounts[idx].failed_logins = 0;
    accounts[idx].withdrawals_today = 0;
    accounts[idx].locked = false;

    account_count++;
    return accounts[idx].accountNo;
}

int find_account_index_by_no(int accountNo) {
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        if (accounts[i].used && accounts[i].accountNo == accountNo)
            return i;
    }
    return -1;
}

void simulate_new_day() {
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        if (accounts[i].used)
            accounts[i].withdrawals_today = 0;
    }
    printf("A new day has been simulated. Withdrawal counters reset.\n");
}

void show_accounts_summary() {
    printf("\n---- ACCOUNTS SUMMARY ----\n");
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        if (accounts[i].used) {
            printf("#%d | %-20s | Balance: %.2f | Locked: %s | Withdrawals today: %d\n",
                accounts[i].accountNo, accounts[i].name,
                accounts[i].balance,
                accounts[i].locked ? "YES" : "NO",
                accounts[i].withdrawals_today);
        }
    }
    printf("---------------------------\n");
}

double compute_interest_preview(double balance, double monthly_rate_percent) {
    return balance * (monthly_rate_percent / 100.0);
}

int get_account_count() {
    return account_count;
}

Account* get_accounts() {
    return accounts;
}
