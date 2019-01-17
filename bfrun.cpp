#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

class BF {
  public:
    BF(ifstream &ifs, ostream &os);
    void run(const string &args = "");

  private:
    istream_iterator<char> isit;
    ostream_iterator<char> osit;
    vector<char> program;
    vector<uint32_t> bracket_map;
};

BF::BF(ifstream &ifs, ostream &os) : isit(ifs), osit(os) {
    program.reserve(1000);
    stack<uint32_t> tmp_bkt_stk;
    map<uint32_t, uint32_t> tmp_bkt_map;
    uint32_t i = 0;
    for (; isit != istream_iterator<char>();) {
        char c = *(isit++);
        if (c == '>' || c == '<' || c == '+' || c == '-' || c == '.' ||
            c == ',' || c == '[' || c == ']') {
            program.push_back(c);
            if (c == '[') {
                tmp_bkt_stk.push(i);
            } else if (c == ']') {
                uint32_t j = tmp_bkt_stk.top();
                tmp_bkt_stk.pop();
                tmp_bkt_map[i] = j;
            }
            ++i;
        }
    }
    bracket_map.reserve(i);
    for (auto it : tmp_bkt_map) {
        bracket_map[it.first] = it.second;
        bracket_map[it.second] = it.first;
    }
}

void BF::run(const string &args) {
    const char *args_ptr = args.c_str();
    uint32_t program_it = 0;
    uint32_t program_size = program.size();

    vector<int> papertype = {0};
    papertype.reserve(128);
    uint32_t pt_it = 0;

    while (program_it != program_size) {
        switch (program[program_it]) {
        case '>':
            ++pt_it;
            if (pt_it == papertype.size()) {
                papertype.push_back(0);
            }
            break;
        case '<':
            if (!pt_it) {
                throw pt_it;
            }
            --pt_it;
            break;
        case '+':
            ++papertype[pt_it];
            break;
        case '-':
            --papertype[pt_it];
            break;
        case '.':
            *(osit++) = char(papertype[pt_it]);
            break;
        case ',':
            if (*args_ptr) {
                papertype[pt_it] = *args_ptr;
                cout << char(*args_ptr);
                ++args_ptr;
            } else {
                papertype[pt_it] = cin.get();
            }
            break;
        case '[':
            if (papertype[pt_it] == 0) {
                program_it = bracket_map[program_it];
            }
            break;
        case ']':
            if (papertype[pt_it] != 0) {
                program_it = bracket_map[program_it];
            }
            break;
        }
        ++program_it;
    }
}

int main(int argc, char *argv[]) {

    const char *infile = argc > 1 ? argv[1] : "prime.bf";
    const char *outfile = argc > 2 ? argv[2] : nullptr;

    ifstream fin(infile);
    BF bf(fin, cout);

    string args = "100\n";
    auto tic = chrono::high_resolution_clock::now();
    bf.run(args);
    auto toc = chrono::high_resolution_clock::now();
    auto t = chrono::duration_cast<chrono::milliseconds>(toc - tic).count();
    cout << endl << t << " ms used." << endl;
    return 0;
}
