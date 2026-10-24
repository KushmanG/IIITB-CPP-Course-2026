#!/bin/sh
# Weighted grader.  Run from the folder that contains your source file:
#     sh ./local_tester/grade.sh               (grades skeleton.cpp)
#   (or whichever tester folder this script is in)
#     sh ./local_tester/grade.sh main.cpp      (grades another file)
# Marks per section come from marks.txt next to this script.

SRC="${1:-skeleton.cpp}"
DIR="$(dirname "$0")"   # the folder this script lives in
TIMEOUT=10

if [ ! -f "$SRC" ]; then echo "Error: $SRC not found. Run this from the folder that contains it."; exit 1; fi
if [ ! -f "$DIR/marks.txt" ]; then echo "Error: $DIR/marks.txt not found. Run from the folder that contains $SRC."; exit 1; fi

run_limited() { perl -e '$t = shift; alarm $t; exec @ARGV or die "cannot run"' "$TIMEOUT" "$@"; }

echo "================================================================"
echo "  GRADING $SRC"
echo "================================================================"

# 1. What DOMjudge does during the exam: does the file compile on its own?
if g++ -std=c++17 -w "$SRC" -o "$DIR/__compile_check.out" 2> "$DIR/__compile_err.txt"; then
    COMPILES=1
    echo "[COMPILES] $SRC compiles on its own."
else
    COMPILES=0
    echo "[COMPILE ERROR] $SRC does not compile on its own  ==>  the judge gives ZERO."
    head -n 15 "$DIR/__compile_err.txt"
fi
PENALTY=0
if grep -q "bits/stdc++" "$SRC"; then PENALTY=1; echo "[WARNING] bits/stdc++.h used ==> 50% penalty."; fi

cp "$SRC" "$DIR/student.cpp"
cd "$DIR" || exit 1
: > __summary.txt

# 2. Run every test file listed in marks.txt
while read -r name marks n label; do
    case "$name" in ''|'#'*) continue ;; esac
    if [ "$name" = "@COMPILE" ]; then echo "@COMPILE|$marks|0|0|$n $label" >> __summary.txt; continue; fi
    echo ""
    echo "---> $name  ($label)"
    if g++ -std=c++17 -w "$name.cpp" -o "$name.out" 2> "$name.err"; then
        run_limited ./"$name.out" < /dev/null > "$name.log" 2>&1
        rc=$?
        cat "$name.log"
        pass=$(grep -c '\[PASS\]' "$name.log")
        if [ $rc -ne 0 ]; then
            echo "[CRASH/TIMEOUT] $name stopped early (exit code $rc). Checks it did not reach count as failed."
            echo "                (Infinite loop? Segfault? Uncaught exception?)"
        fi
        echo "$name|$marks|$n|$pass|$label|ok" >> __summary.txt
    else
        echo "[COMPILE FAIL] $name.cpp did not compile against your code (missing class/method or wrong signature):"
        grep -m 4 "error" "$name.err"
        echo "$name|$marks|$n|0|$label|cfail" >> __summary.txt
    fi
done < marks.txt

# 3. Report card
echo ""
awk -F'|' -v compiles="$COMPILES" -v penalty="$PENALTY" '
{
    rows[NR] = $0
    if ($1 != "@COMPILE") { files++; if ($6 == "ok") okfiles++ }
}
END {
    printf "================================================================\n"
    printf "  REPORT CARD\n"
    printf "================================================================\n"
    printf "  %-38s %8s %13s\n", "Section", "Checks", "Marks"
    printf "  ----------------------------------------------------------------\n"
    for (i = 1; i <= NR; i++) {
        split(rows[i], f, "|")
        if (f[1] == "@COMPILE") {
            got = (files > 0) ? f[2] * okfiles / files : 0
            printf "  %-38s %8s %6.1f / %4d\n", f[5], (okfiles + 0) "/" files " files", got, f[2]
        } else {
            p = f[4] + 0; if (p > f[3]) p = f[3]
            got = f[2] * p / f[3]
            note = (f[6] == "cfail") ? "  (did not compile)" : ""
            printf "  %-38s %8s %6.1f / %4d%s\n", f[5], p "/" f[3], got, f[2], note
        }
        total += got; max += f[2]
    }
    printf "  ----------------------------------------------------------------\n"
    printf "  %-38s %8s %6.1f / %4d\n", "Raw score", "", total, max
    final = total
    if (penalty == 1) { final = final / 2; printf "  bits/stdc++.h penalty (x0.5)\n" }
    if (compiles == 0) { printf "  Your file does not compile on its own: the judge would give 0.\n"; final = 0 }
    printf "  %-38s %8s %6.1f / %4d\n", "FINAL", "", final, max
    printf "================================================================\n"
}' __summary.txt

rm -f student.cpp *.out *.log *.err __summary.txt __compile_err.txt *_log.txt
