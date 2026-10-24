// Command-driven driver used by local_test9a/9b and run_driver.cpp. No change required.
//   REG <name> <cash>            CANCEL <trader> <symbol> <id>
//   BUY <trader> <sym> <p> <q>   BOOK <symbol>
//   SELL <trader> <sym> <p> <q>  POS <trader> <symbol>
//   END
void runDriver(istream& in, const string& logFile) {
    Exchange ex(logFile);
    string cmd;
    while (in >> cmd) {
        try {
            if (cmd == "REG") {
                string n; double c; in >> n >> c;
                ex.registerTrader(n, c);
                cout << "OK\n";
            } else if (cmd == "BUY" || cmd == "SELL") {
                string t, s; double p; int q; in >> t >> s >> p >> q;
                int id = ex.placeOrder(t, s, cmd == "BUY" ? Side::BUY : Side::SELL, p, q);
                cout << "ORDER " << id << "\n";
            } else if (cmd == "CANCEL") {
                string t, s; int id; in >> t >> s >> id;
                cout << (ex.cancelOrder(t, s, id) ? "CANCEL OK\n" : "CANCEL FAIL\n");
            } else if (cmd == "BOOK") {
                string s; in >> s;
                ex.getBook(s).printDepth();
            } else if (cmd == "POS") {
                string t, s; in >> t >> s;
                int pos = ex.getPosition(t, s);
                cout << t << " " << s << " " << pos << " " << fmt2(ex.getCash(t)) << "\n";
            } else if (cmd == "END") {
                break;
            }
        } catch (ExchangeException& e) {
            cout << "ERR " << e.what() << "\n";
        }
    }
}
