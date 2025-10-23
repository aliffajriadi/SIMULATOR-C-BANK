// main.c
#include "bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACC get_accounts()
#define ACC_COUNT get_account_count()

void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int read_int_safe(const char *prompt) {
    int x;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &x) == 1) {
            flush_stdin();
            return x;
        } else {
            printf("Input invalid. Please enter an integer.\n");
            flush_stdin();
        }
    }
}

double read_double_safe(const char *prompt) {
    double d;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &d) == 1) {
            flush_stdin();
            return d;
        } else {
            printf("Input invalid. Please enter a number.\n");
            flush_stdin();
        }
    }
}

void create_account_flow() {
    if (ACC_COUNT >= MAX_ACCOUNTS) {
        printf("Account capacity reached. Cannot create new account.\n");
        return;
    }

    char name[MAX_NAME_LEN];
    int pin;
    double init_bal;

    printf("Enter full name: ");
    if (!fgets(name, sizeof(name), stdin)) return;
    name[strcspn(name, "\n")] = '\0';

    if (strlen(name) == 0) {
        printf("Name cannot be empty.\n");
        return;
    }

    pin = read_int_safe("Enter 4-digit PIN (numbers only): ");
    init_bal = read_double_safe("Enter initial balance: ");

    if (init_bal < 0) {
        printf("Initial balance cannot be negative.\n");
        return;
    }

    int accNo = create_account(name, pin, init_bal);
    if (accNo < 0) {
        printf("Failed to create account.\n");
    } else {
        printf("Account created successfully! Your Account Number is #%d\n", accNo);
    }
}

void print_receipt(const char *msg) {
    printf(">> %s\n", msg);
}

void deposit_flow(int idx) {
    double amt = read_double_safe("Enter amount to deposit: ");
    if (amt <= 0) { printf("Deposit amount must be positive.\n"); return; }
    ACC[idx].balance += amt;
    char buf[200];
    snprintf(buf, sizeof(buf), "Deposited %.2f to #%d; new balance %.2f",
             amt, ACC[idx].accountNo, ACC[idx].balance);
    print_receipt(buf);
}

void withdraw_flow(int idx) {
    if (ACC[idx].withdrawals_today >= MAX_WITHDRAWALS_PER_DAY) {
        printf("Maximum withdrawals for today reached.\n");
        return;
    }
    double amt = read_double_safe("Enter amount to withdraw: ");
    if (amt <= 0) { printf("Invalid amount.\n"); return; }
    if (amt > MAX_WITHDRAWAL_AMOUNT) {
        printf("Max single withdrawal: %.2f\n", MAX_WITHDRAWAL_AMOUNT);
        return;
    }
    if (amt > ACC[idx].balance) {
        printf("Insufficient balance.\n");
        return;
    }
    ACC[idx].balance -= amt;
    ACC[idx].withdrawals_today++;
    char buf[200];
    snprintf(buf, sizeof(buf), "Withdrew %.2f from #%d; new balance %.2f",
             amt, ACC[idx].accountNo, ACC[idx].balance);
    print_receipt(buf);
}

void transfer_flow(int idx_from) {
    int to_no = read_int_safe("Enter destination account number: ");
    int idx_to = find_account_index_by_no(to_no);
    if (idx_to < 0) { printf("Destination account not found.\n"); return; }
    if (idx_to == idx_from) { printf("Cannot transfer to same account.\n"); return; }

    double amt = read_double_safe("Enter amount to transfer: ");
    if (amt <= 0) { printf("Invalid amount.\n"); return; }
    if (amt > ACC[idx_from].balance) { printf("Insufficient funds.\n"); return; }

    ACC[idx_from].balance -= amt;
    ACC[idx_to].balance += amt;

    char buf[200];
    snprintf(buf, sizeof(buf),
             "Transferred %.2f from #%d to #%d; new balance: %.2f",
             amt, ACC[idx_from].accountNo, ACC[idx_to].accountNo, ACC[idx_from].balance);
    print_receipt(buf);
}

void change_pin_flow(int idx) {
    int oldpin = read_int_safe("Enter current PIN: ");
    if (oldpin != ACC[idx].pin) { printf("PIN incorrect.\n"); return; }
    int newpin = read_int_safe("Enter new PIN: ");
    ACC[idx].pin = newpin;
    printf("PIN changed successfully.\n");
}

void interest_preview_flow(int idx) {
    double rate = read_double_safe("Enter monthly interest rate (%%): ");
    double interest = compute_interest_preview(ACC[idx].balance, rate);
    printf("Interest this month: %.2f\n", interest);
}

void account_menu(int idx) {
    while (1) {
        printf("\n--- Account #%d (%s) ---\n", ACC[idx].accountNo, ACC[idx].name);
        printf("1. Check Balance\n2. Deposit\n3. Withdraw\n4. Transfer\n5. Change PIN\n6. Interest Preview\n7. Logout\n> ");
        int opt;
        if (scanf("%d", &opt) != 1) { flush_stdin(); continue; }
        flush_stdin();
        switch (opt) {
            case 1: printf("Balance: %.2f\n", ACC[idx].balance); break;
            case 2: deposit_flow(idx); break;
            case 3: withdraw_flow(idx); break;
            case 4: transfer_flow(idx); break;
            case 5: change_pin_flow(idx); break;
            case 6: interest_preview_flow(idx); break;
            case 7: return;
            default: printf("Invalid option.\n");
        }
    }
}

int login_prompt() {
    int accNo = read_int_safe("Enter Account No: ");
    int idx = find_account_index_by_no(accNo);
    if (idx < 0) { printf("Account not found.\n"); return -1; }
    if (ACC[idx].locked) { printf("Account locked.\n"); return -1; }

    for (int i = 1; i <= 3; i++) {
        int pin = read_int_safe("Enter PIN: ");
        if (pin == ACC[idx].pin) {
            ACC[idx].failed_logins = 0;
            printf("Login successful.\n");
            return idx;
        } else {
            ACC[idx].failed_logins++;
            printf("Incorrect PIN (%d/3)\n", i);
            if (ACC[idx].failed_logins >= 3) {
                ACC[idx].locked = true;
                printf("Account locked due to too many failed attempts.\n");
                return -1;
            }
        }
    }
    return -1;
}

void main_menu() {
    while (1) {
        printf("\n=== COMMUNITY BANK SIMULATOR ===\n");
        printf("1. Create Account\n2. Login\n3. Simulate New Day\n4. Show Accounts (debug)\n5. Exit\n> ");
        int opt;
        if (scanf("%d", &opt) != 1) { flush_stdin(); continue; }
        flush_stdin();
        switch (opt) {
            case 1: create_account_flow(); break;
            case 2: {
                int idx = login_prompt();
                if (idx >= 0) account_menu(idx);
                break;
            }
            case 3: simulate_new_day(); break;
            case 4: show_accounts_summary(); break;
            case 5: printf("Goodbye!\n"); return;
            default: printf("Invalid choice.\n");
        }
    }
}

int main() {
    init_bank();
    printf("Welcome to Community Bank Simulator (C version)\n");
    main_menu();
    return 0;
}
