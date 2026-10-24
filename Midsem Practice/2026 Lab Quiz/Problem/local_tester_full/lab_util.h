// Log-line normaliser for the 2026 Lab Quiz. The question pdf and Sample_Runs.pdf disagree in a few
// places, so both spellings are accepted:
//   ’ vs '      "Add money failed" vs "Add-money failed"      "user . Check" vs "user. Check"
//   trailing zeros: Rs.500.000000 == Rs.500 == Rs.500.00
// Everything else must match exactly.
#include "tester_util.h"

inline string normLine(string s) {
    string r;
    for (size_t i = 0; i < s.size(); i++) {           // curly apostrophe (UTF-8 E2 80 99) -> '
        if ((unsigned char)s[i] == 0xE2 && i + 2 < s.size() && (unsigned char)s[i+1] == 0x80 && (unsigned char)s[i+2] == 0x99) { r += '\''; i += 2; }
        else r += s[i];
    }
    s = r;
    size_t p;
    while ((p = s.find("Add money failed")) != string::npos) s.replace(p, 16, "Add-money failed");
    while ((p = s.find("Send-money failed")) != string::npos) s.replace(p, 17, "Send money failed");
    while ((p = s.find(" . Check")) != string::npos) s.replace(p, 8, ". Check");
    while (!s.empty() && (s.back() == ' ' || s.back() == '\r' || s.back() == '\n')) s.pop_back();
    // strip trailing zeros in decimal numbers
    string o;
    for (size_t i = 0; i < s.size(); ) {
        if (isdigit((unsigned char)s[i])) {
            size_t j = i; while (j < s.size() && isdigit((unsigned char)s[j])) j++;
            string num = s.substr(i, j - i);
            if (j + 1 < s.size() && s[j] == '.' && isdigit((unsigned char)s[j+1])) {
                size_t k = j + 1; while (k < s.size() && isdigit((unsigned char)s[k])) k++;
                string frac = s.substr(j + 1, k - j - 1);
                while (!frac.empty() && frac.back() == '0') frac.pop_back();
                num += frac.empty() ? "" : "." + frac;
                j = k;
            }
            o += num; i = j;
        } else o += s[i++];
    }
    return o;
}

inline vector<string> normLog(const string& file) {
    vector<string> v;
    for (string& l : readLines(file)) { string n = normLine(l); if (!n.empty()) v.push_back(n); }
    return v;
}

inline string joinLines(const vector<string>& v) { string s; for (auto& l : v) s += "      " + l + "\n"; return s; }

// exact (normalised) comparison of a whole log file
inline void testLog(const string& file, const vector<string>& expected, const string& label) {
    vector<string> got = normLog(file), exp;
    for (auto& e : expected) exp.push_back(normLine(e));
    test(got == exp, label, label + " -- log file mismatch.\n    got:\n" + joinLines(got) + "    expected:\n" + joinLines(exp));
}

inline bool logHas(const string& file, const string& line) {
    string n = normLine(line);
    for (auto& l : normLog(file)) if (l == n) return true;
    return false;
}
