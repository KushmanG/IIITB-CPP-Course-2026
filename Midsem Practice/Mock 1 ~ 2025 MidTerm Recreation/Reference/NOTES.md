# How this paper was reconstructed

Only last year's solution survives (Classes.h / Classes.cpp / main.cpp), not the question. The spec was rebuilt from that code. Where the code was buggy or left something undefined, the spec below decides, and the local tester enforces it.

## Bugs in last year's solution

1. `extern const vector<string> paperStatus;` in the .h vs `vector<string> paperStatus` in the .cpp → **compile error**. On DOMjudge that is 0 marks.
2. `Paper(title, author)` never initialises `status`. It printed "In progress" only because the memory happened to be 0.
3. `assignPaperToReviewer` assigns the paper to **every** reviewer, not just the named one, and never checks that the reviewer exists.
4. `findReviewer` throws a `string`, but driver command 5 only catches `int`, so an unknown reviewer crashes the program.
5. `averageScore` isn't virtual, so it isn't polymorphic through `Paper&`.
6. The `Under_Review` status is declared but never used.
7. `decideAcceptance` also rejects drafts and unreviewed papers, because their average is 0.

## Decisions taken in the spec

- IDs start at 1 (`++uniq`), and every new paper is `In_progress`.
- `addAuthor`: allowed only while `In_progress`. A duplicate author is ignored.
- `assignPaperToReviewer` sets the paper to `Under_Review`. Only `Under_Review` papers can be reviewed.
- Check order is fixed (see the tables in question.pdf). The score check comes first in `reviewPaper`, which keeps the original code's order.
- `decideAcceptance` only touches `Under_Review` papers that have at least one score. The threshold is ≥ 7.0, as in the original.
- `printAllPapers` prints newest first, as in the original.
- `reviewPaper(int score, int paperID)` keeps the original parameter order. It's a trap on purpose.
- The driver prints NOK lines to stdout and continues (the original used cerr and stopped). The driver isn't graded either way.
