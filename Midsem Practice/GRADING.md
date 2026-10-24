# Graders

Each grader compiles your file, runs every test, and prints a report card out of 100. The marks follow each paper's own weighting.

| Practice | Run from | Command | Marks |
|---|---|---|---|
| 2025 GIE | `2025 GIE/` | `sh ./local_tester/grade.sh` (grades `main.cpp`) | Q1 10, Q2 10, Q3 15, Q4 20, Q5 25, Q6 20 (same as the paper) |
| 2026 Lab Quiz | `2026 Lab Quiz/Problem/` | `sh ./local_tester_full/grade.sh` (grades `skeleton.cpp`) | Wallet 10, Account 5, addMoney 15, sendMoney 20, registerUser 15, login 10, menu wiring 10, 3 end-to-end runs 15 |
| Mock 2 | `Mock 2 - Order Matching/Problem/` | `sh ./local_tester/grade.sh` | UML 15, Exception+Logger 8, Orders 15, Trade 5, OrderBook 35, Exchange 22 |

To grade a different file, pass it as an argument: `sh ./local_tester/grade.sh my_attempt.cpp`

## How the score works

- **Compile check first.** If your file doesn't compile on its own, FINAL is 0, the same as on DOMjudge. The report still shows what you would have scored.
- **Each section** earns marks × (checks passed / checks total). A test file that doesn't compile against your code (a missing method or a wrong signature) scores 0 for that section.
- **Each test file gets 10 seconds.** An infinite loop or a crash forfeits the checks it didn't reach. The report shows `[CRASH/TIMEOUT]` when this happens.
- `bits/stdc++.h` halves the score, as in the exam rules.

## Notes per paper

**2025 GIE.** The tests open private members (`#define private public`) so they can check `ht`, `buckets` and `hash()` directly. Use the names the paper gives. Q6 runs your whole `main()` with typed input and compares the output exactly, so don't add extra `cout`s. The read-only code uses `find_if` and `runtime_error`. If the compile check fails on those, add `#include <algorithm>` and `#include <stdexcept>` at the top of the file, which is allowed.

**2026 Lab Quiz.** This is much stricter than the official `local_tester/`, which only checks 11 substrings. This one checks whole log lines and the end-to-end log files from Sample_Runs.pdf. Where the question and Sample_Runs disagree, both spellings pass: `’` or `'`, "Add money failed" or "Add-money failed", trailing zeros like `Rs.500.000000` or `Rs.500`, and `uusseerr .` or `uusseerr.`. One trap is worth knowing: logout only works if `doUserAction` takes `Account*& currentUser`. With a plain pointer, the program never returns to the main menu, and the end-to-end runs time out.

**Mock 2.** Same 28 tests as before, now weighted.

## References

Open these only after an attempt: `2025 GIE/Reference/reference_main.cpp`, `2026 Lab Quiz/Reference/reference_solution.cpp`, `Mock 2 - Order Matching/Reference/`. Each reference scores 100/100. I checked every grader by planting typical mistakes in the reference (by-value `getBucket`, wrong table size, missing receiver credit, retyped log strings, logout through a plain pointer, and others), and each one costs marks. Your original Lab Quiz submission scores 12.5/100 on the full Lab Quiz tester.
