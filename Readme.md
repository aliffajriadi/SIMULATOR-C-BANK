Community Bank Simulator (C) - Run Guide

Files:
- main.c
- bank.c
- bank.h
- Makefile

Build (Ubuntu):
1. Open terminal in project folder.
2. Run: make
   (or: gcc *.c -o bank)

Run:
./bank

Menu:
1. Create Account
2. Login
3. Simulate New Day (resets withdrawal counters)
4. Show accounts summary (for testing only)
5. Exit

Notes:
- Account numbers start at 1001 and increment.
- Login requires Account No and PIN.
- After 3 failed PIN attempts in the session, account is locked until program restart.
- Max 3 withdrawals per account per day.
- Max single withdrawal amount: 500.0
- Deposit must be positive.
- Transfer verifies destination account exists and sufficient balance.
- Use option 3 to simulate a new day (resets withdrawals_today counters).
