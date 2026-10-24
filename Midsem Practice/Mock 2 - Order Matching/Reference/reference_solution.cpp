// Reference solution — Mock 2: Order Matching Engine. Do NOT open before attempting.
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
using namespace std;

/*================================= NO EDIT ZONE =================================*/
enum class Side { BUY, SELL };

// Formats a price with exactly 2 decimals: fmt2(1500) == "1500.00"
string fmt2(double x) {
    ostringstream os;
    os << fixed << setprecision(2) << x;
    return os.str();
}
/*============================== END OF NO EDIT ZONE =============================*/

/* ---------------- 1. ExchangeException ---------------- */
class ExchangeException : public runtime_error {
public:
    ExchangeException(const string& msg) : runtime_error(msg) {}
};

/* ---------------- 2. Logger ---------------- */
class Logger {
    ofstream file;
    int count;
public:
    Logger(const string& filename) : file(filename, ios::app), count(0) {
        if (!file.is_open()) throw runtime_error("Cannot open log file");
    }
    void log(const string& msg) {
        file << msg << "\n";
        file.flush();
        count++;
    }
    int getCount() const { return count; }
};

/* ---------------- 3. Order ---------------- */
class Order {
    static int nextId;
    int id;
    string trader;
    string symbol;
    double price;
    int quantity;
    int remaining;
public:
    Order(const string& trader, const string& symbol, double price, int quantity)
        : trader(trader), symbol(symbol), price(price), quantity(quantity), remaining(quantity) {
        if (quantity <= 0) throw ExchangeException("Invalid quantity");
        if (price <= 0) throw ExchangeException("Invalid price");
        id = nextId++;
    }
    virtual ~Order() {}
    int getId() const { return id; }
    string getTrader() const { return trader; }
    string getSymbol() const { return symbol; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    int getRemaining() const { return remaining; }
    bool isFilled() const { return remaining == 0; }

    virtual Side side() const = 0;
    virtual bool crosses(double otherPrice) const = 0;

    Order& operator-=(int q) {
        if (q <= 0 || q > remaining) throw ExchangeException("Invalid fill");
        remaining -= q;
        return *this;
    }
    string describe() const {
        return "#" + to_string(id) + " " + (side() == Side::BUY ? "BUY" : "SELL") + " " + trader + " " +
               symbol + " " + to_string(remaining) + "/" + to_string(quantity) + "@" + fmt2(price);
    }
};
int Order::nextId = 1;

/* ---------------- 4. BuyOrder / SellOrder ---------------- */
class BuyOrder : public Order {
public:
    BuyOrder(const string& trader, const string& symbol, double price, int quantity)
        : Order(trader, symbol, price, quantity) {}
    Side side() const override { return Side::BUY; }
    bool crosses(double otherPrice) const override { return getPrice() >= otherPrice; }
};

class SellOrder : public Order {
public:
    SellOrder(const string& trader, const string& symbol, double price, int quantity)
        : Order(trader, symbol, price, quantity) {}
    Side side() const override { return Side::SELL; }
    bool crosses(double otherPrice) const override { return getPrice() <= otherPrice; }
};

/* ---------------- 5. Trade ---------------- */
class Trade {
    string symbol;
    int buyId;
    string buyer;
    int sellId;
    string seller;
    double price;
    int quantity;
public:
    Trade(const string& symbol, int buyId, const string& buyer, int sellId, const string& seller,
          double price, int quantity)
        : symbol(symbol), buyId(buyId), buyer(buyer), sellId(sellId), seller(seller),
          price(price), quantity(quantity) {}
    string getSymbol() const { return symbol; }
    int getBuyId() const { return buyId; }
    string getBuyer() const { return buyer; }
    int getSellId() const { return sellId; }
    string getSeller() const { return seller; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    friend ostream& operator<<(ostream& os, const Trade& t) {
        os << "TRADE " << t.symbol << " " << t.quantity << "@" << fmt2(t.price)
           << " B#" << t.buyId << ":" << t.buyer << " S#" << t.sellId << ":" << t.seller;
        return os;
    }
};

/* ---------------- 6. OrderBook ---------------- */
class OrderBook {
    string symbol;
    vector<shared_ptr<Order>> bids;
    vector<shared_ptr<Order>> asks;

    // index of best eligible resting order on `side` for `incoming`, or -1
    int bestMatch(const vector<shared_ptr<Order>>& resting, const shared_ptr<Order>& incoming) const {
        int best = -1;
        for (int i = 0; i < (int)resting.size(); i++) {
            const shared_ptr<Order>& r = resting[i];
            if (r->getTrader() == incoming->getTrader()) continue;
            if (!incoming->crosses(r->getPrice())) continue;
            if (best == -1) { best = i; continue; }
            const shared_ptr<Order>& b = resting[best];
            bool better = (r->side() == Side::SELL) ? r->getPrice() < b->getPrice()
                                                    : r->getPrice() > b->getPrice();
            if (better || (r->getPrice() == b->getPrice() && r->getId() < b->getId())) best = i;
        }
        return best;
    }
public:
    OrderBook(const string& symbol) : symbol(symbol) {}
    string getSymbol() const { return symbol; }

    vector<Trade> submit(shared_ptr<Order> incoming) {
        if (incoming->getSymbol() != symbol) throw ExchangeException("Wrong symbol");
        vector<Trade> trades;
        bool isBuy = incoming->side() == Side::BUY;
        vector<shared_ptr<Order>>& opposite = isBuy ? asks : bids;
        while (!incoming->isFilled()) {
            int idx = bestMatch(opposite, incoming);
            if (idx == -1) break;
            shared_ptr<Order> resting = opposite[idx];
            int q = min(incoming->getRemaining(), resting->getRemaining());
            double p = resting->getPrice();
            if (isBuy)
                trades.push_back(Trade(symbol, incoming->getId(), incoming->getTrader(),
                                       resting->getId(), resting->getTrader(), p, q));
            else
                trades.push_back(Trade(symbol, resting->getId(), resting->getTrader(),
                                       incoming->getId(), incoming->getTrader(), p, q));
            *incoming -= q;
            *resting -= q;
            if (resting->isFilled()) opposite.erase(opposite.begin() + idx);
        }
        if (!incoming->isFilled()) (isBuy ? bids : asks).push_back(incoming);
        return trades;
    }

    shared_ptr<Order> find(int id) const {
        for (const auto& o : bids) if (o->getId() == id) return o;
        for (const auto& o : asks) if (o->getId() == id) return o;
        return nullptr;
    }

    void cancel(int id, const string& trader) {
        for (vector<shared_ptr<Order>>* side : {&bids, &asks}) {
            for (int i = 0; i < (int)side->size(); i++) {
                if ((*side)[i]->getId() == id) {
                    if ((*side)[i]->getTrader() != trader) throw ExchangeException("Not your order");
                    side->erase(side->begin() + i);
                    return;
                }
            }
        }
        throw ExchangeException("Order not found");
    }

    int restingCount() const { return bids.size() + asks.size(); }

    double bestBid() const {
        if (bids.empty()) throw ExchangeException("No bids");
        double best = bids[0]->getPrice();
        for (const auto& o : bids) if (o->getPrice() > best) best = o->getPrice();
        return best;
    }
    double bestAsk() const {
        if (asks.empty()) throw ExchangeException("No asks");
        double best = asks[0]->getPrice();
        for (const auto& o : asks) if (o->getPrice() < best) best = o->getPrice();
        return best;
    }
    double spread() const {
        double b = bestBid();
        double a = bestAsk();
        return a - b;
    }

    void printDepth() const {
        map<double, pair<int, int>> askLv, bidLv;   // price -> (total qty, count)
        for (const auto& o : asks) { askLv[o->getPrice()].first += o->getRemaining(); askLv[o->getPrice()].second++; }
        for (const auto& o : bids) { bidLv[o->getPrice()].first += o->getRemaining(); bidLv[o->getPrice()].second++; }
        for (auto it = askLv.rbegin(); it != askLv.rend(); ++it)
            cout << "ASK " << fmt2(it->first) << " " << it->second.first << " (" << it->second.second << ")\n";
        cout << "---\n";
        for (auto it = bidLv.rbegin(); it != bidLv.rend(); ++it)
            cout << "BID " << fmt2(it->first) << " " << it->second.first << " (" << it->second.second << ")\n";
    }
};

/* ---------------- 7. Exchange ---------------- */
class Exchange {
    map<string, OrderBook> books;
    map<string, double> cash;
    map<string, map<string, int>> positions;
    Logger logger;
public:
    Exchange(const string& logFile) : logger(logFile) {}

    void registerTrader(const string& name, double initialCash) {
        if (cash.count(name)) throw ExchangeException("Trader already registered");
        if (initialCash < 0) throw ExchangeException("Invalid cash");
        cash[name] = initialCash;
        logger.log("REGISTER " + name + " " + fmt2(initialCash));
    }

    int placeOrder(const string& trader, const string& symbol, Side side, double price, int qty) {
        try {
            if (!cash.count(trader)) throw ExchangeException("Unknown trader " + trader);
            if (side == Side::BUY && cash[trader] < price * qty) throw ExchangeException("Insufficient cash");
            shared_ptr<Order> order;
            if (side == Side::BUY) order = make_shared<BuyOrder>(trader, symbol, price, qty);
            else order = make_shared<SellOrder>(trader, symbol, price, qty);
            logger.log("ACCEPT " + order->describe());
            if (!books.count(symbol)) books.emplace(symbol, OrderBook(symbol));
            vector<Trade> trades = books.at(symbol).submit(order);
            for (const Trade& t : trades) {
                double value = t.getPrice() * t.getQuantity();
                cash[t.getBuyer()] -= value;
                cash[t.getSeller()] += value;
                positions[t.getBuyer()][symbol] += t.getQuantity();
                positions[t.getSeller()][symbol] -= t.getQuantity();
                ostringstream os;
                os << t;
                logger.log(os.str());
            }
            return order->getId();
        } catch (ExchangeException& e) {
            logger.log("REJECT " + trader + ": " + e.what());
            return -1;
        }
    }

    bool cancelOrder(const string& trader, const string& symbol, int id) {
        try {
            if (!books.count(symbol)) throw ExchangeException("Order not found");
            books.at(symbol).cancel(id, trader);
            logger.log("CANCEL #" + to_string(id) + " " + trader);
            return true;
        } catch (ExchangeException& e) {
            logger.log("REJECT " + trader + ": " + e.what());
            return false;
        }
    }

    double getCash(const string& trader) const {
        if (!cash.count(trader)) throw ExchangeException("Unknown trader " + trader);
        return cash.at(trader);
    }
    int getPosition(const string& trader, const string& symbol) const {
        if (!cash.count(trader)) throw ExchangeException("Unknown trader " + trader);
        if (!positions.count(trader) || !positions.at(trader).count(symbol)) return 0;
        return positions.at(trader).at(symbol);
    }
    OrderBook& getBook(const string& symbol) {
        if (!books.count(symbol)) throw ExchangeException("Unknown symbol " + symbol);
        return books.at(symbol);
    }
    int getLogCount() const { return logger.getCount(); }
};

int main() {
    return 0;
}
