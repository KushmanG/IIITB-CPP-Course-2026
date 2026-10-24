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

class ExchangeException : public runtime_error
{
    public:
        ExchangeException(const string& msg) : runtime_error(msg) {};
};

class Logger 
{
    ofstream file;
    int count;

    public:
        Logger(const string& filename) {}
        void log(const string& msg) 
        {
            cout << msg << endl;
            file.flush();
            count++;
        }
        int getCount() const { return count; }
};


class Order 
{
    static int nextId;
    int id;
    string trader;
    string symbol;
    double price;
    int quantity;
    int remaining;

    public:
        Order(const string& trader, const string& symbol, double price, int quantity) {
            this->trader = trader;
            this->symbol = symbol;
            this->quantity = quantity;
            this->remaining = quantity;
            this->price = price;

            if (quantity <= 0) throw ExchangeException("Invalid quantity");
            if (price <= 0) throw ExchangeException("Invalid price");

            this->id = nextId++;
        }
        ~Order() {}

        int getId() const { return id; }
        string getTrader() const { return trader; }
        string getSymbol() const { return symbol; }
        double getPrice() const { return price; }
        int getQuantity() const { return quantity; }
        int getRemaining() const { return remaining; }

        bool isFilled() const { return remaining == 0; }

        virtual Side side() const = 0;
        virtual bool crosses(double otherPrice) const = 0;

        Order& operator-=(int q) 
        {
            if (q <= 0 || q > remaining) throw ExchangeException("Invalid fill");
            remaining -= q;
            return *this;
        }

        string describe() const { return "#" + to_string(id) + " " + (side() == Side::BUY ? "BUY" : "SELL") + " " + trader + " " + symbol + " " + to_string(remaining) + "/" + to_string(quantity) + "@" + fmt2(price); }
};

int Order::nextId = 1;

class BuyOrder : public Order
{
    public:
        BuyOrder(const string& trader, const string& symbol, double price, int quantity) : Order(trader, symbol, price, quantity) {}
        Side side() const { return Side::BUY; }
        bool crosses(double otherPrice) const { return this->getPrice() >= otherPrice; }
};
class SellOrder : public Order 
{
    public:
        SellOrder(const string& trader, const string& symbol, double price, int quantity) : Order(trader, symbol, price, quantity) {}
        Side side() const { return Side::SELL; }
        bool crosses(double otherPrice) const { return this->getPrice() <= otherPrice; } 
};

class Trade 
{
    string symbol;
    int buyId;
    string buyer;
    int sellId;
    string seller;
    double price;
    double quantity;

    public:
        Trade(string symbol, int buyId, string buyer, int sellId, string seller, double price, double quantity) 
        {
            this->symbol = symbol;
            this->buyId = buyId;
            this->buyer = buyer;
            this->sellId = sellId;
            this->seller = seller;
            this->price = price;
            this->quantity = quantity;
        }

        string getSymbol() const { return symbol; }
        int getBuyId() const { return buyId; }
        string getBuyer() const { return buyer; }
        int getSellId() const { return sellId; }
        string getSeller() const { return seller; }
        double getPrice() const { return price; }
        double getQuantity() const { return quantity; }

        friend ostream& operator<<(ostream& os, const Trade& t)
        {
            os << "TRADE " << t.symbol << " " << t.quantity << "@" << fmt2(t.price) << " B#" << t.buyId << ":" << t.buyer << " S#" << t.sellId << ":" << t.seller;
            return os;
        }
};

class OrderBook 
{
    string symbol;
    vector<shared_ptr<Order>> bids;
    vector<shared_ptr<Order>> asks;

    public: 
        OrderBook(const string& symbol) { this->symbol = symbol; }
        string getSymbol() const { return symbol; }

        vector<Trade> submit(shared_ptr<Order> incoming) 
        {
            if (incoming->getSymbol() != symbol) throw ExchangeException("Wrong symbol");

            vector<shared_ptr<Order>>* opposite;
            if (incoming->side() == Side::BUY) opposite = &asks;
            else opposite = &bids;

            while (!incoming->isFilled())
            {
                vector<shared_ptr<Order>> elligible;
                for (auto& r : *opposite) { if (r->getTrader() != incoming->getTrader() && incoming->crosses(r->getPrice())) elligible.push_back(r); }
                shared_ptr<Order> best_r;
                for (auto& r : elligible) 
                {
                    if (r->side() == Side::BUY) best_r->getPrice() >
                }
            }
        }

        shared_ptr<Order> find(int id) const 
        {
            for (size_t i = 0; i < bids.size(); i++) if (bids[i]->getId() == id) return bids[i];
            for (size_t i = 0; i < asks.size(); i++) if (asks[i]->getId() == id) return asks[i];
            return nullptr;
        }

        void cancel(int id, const string& trader) 
        {
            shared_ptr<Order> order = find(id);
            if (!order) throw ExchangeException("Order not found");
            if (order->getTrader() != trader) throw ExchangeException("Not your order");
            // ?? delete order;
        }

        int restingCount() const { return bids.size() + asks.size(); }
        double bestBid() const 
        { 
            if (bids.empty()) throw ExchangeException("No bids");
            double best = bids[0]->getPrice();
            for (size_t i = 1; i < bids.size(); i++) if (bids[i]->getPrice() > best) best = bids[i]->getPrice();
            return best;
        }
        double bestAsk() const 
        {
            if (asks.empty()) throw ExchangeException("No asks");
            double best = asks[0]->getPrice();
            for (size_t i = 1; i < asks.size(); i++) if (asks[i]->getPrice() < best) best = bids[i]->getPrice();
            return best;
        }

        double spread() const 
        {
            double best_bid = bestBid();
            double best_ask = bestAsk();
            return best_ask - best_bid;
        }
        void printDepth() const {}


};
class Exchange {};


/* main is not evaluated. Use it for your own testing if you like. */
int main() {
    return 0;
}
