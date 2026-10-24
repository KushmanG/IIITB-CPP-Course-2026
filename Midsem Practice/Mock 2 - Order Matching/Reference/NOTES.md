# Mock 2: what the tests are really checking

Open this only after you've attempted the mock.

The traps, and which test catches each one:

| Trap | Test |
|---|---|
| Trade at the incoming order's price instead of the resting order's (maker) price | 4, 5, 8, 9a |
| No self-trade skip, or removing your own order instead of skipping it | 6, 9a, 9b |
| At equal price, the later order wins (should be the earliest ID) | 5 |
| ID consumed by a rejected order (`id(nextId++)` in the init list runs before validation) | 2, 8, 9b |
| Logger opened without `ios::app` (truncates the file) | 1 |
| Logger not flushed (the file looks empty until the destructor runs) | 1, 8 |
| `printDepth` sorted ascending, or summing `quantity` instead of `remaining` | 7, 9a |
| `placeOrder` letting exceptions escape instead of logging `REJECT` and returning -1 | 8, 9b |
| Missing BUY cash check, or doing it after creating the order (consumes an ID) | 8, 9b |
| `spread()` calling `bestAsk()` first (wrong message when both sides are empty) | 7 |
| `fixed`/`setprecision` left on `cout` (fmt2 avoids this) | 9a |

A design point: `map<string, OrderBook>` with `books[symbol]` needs a default constructor, which `OrderBook` doesn't have. Use `books.count()`, `books.emplace(symbol, OrderBook(symbol))` and `books.at(symbol)`.

Each of these mistakes was injected into the reference solution, and every one drops the score below 28.
