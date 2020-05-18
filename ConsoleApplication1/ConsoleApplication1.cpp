#include <iostream>
#include <bitset>
#include <functional>
#include <vector>
#include <array>
#include <conio.h>
#include <cstdarg>
#include <tuple>

using namespace std;

template <int h, int w>
class GOL {
public:
    GOL(const char* const* playground) {
        _playground[0] = playground_row();
        _playground[h] = playground_row();
        string s(playground[0]);
        auto b = playground_row(s) << 1;
        _playground[1] = b;
        apply([this, playground](int i) { _playground[i + 1] = (playground_row(string(playground[i]))) << 1; });
    }

    void print() {
        apply([this](int i) { cout << _playground[i + 1].to_string().substr(1, w) << endl; });
        cout << endl;
    }

    void evolve() {
        vector<function<void()>> cell_changers;
        apply([this, &cell_changers](int i) {evolve(i + 1, cell_changers); });
        for (auto f : cell_changers) f();
    }

    void evolve1() {
        vector<tuple<int, int>> toggle_cells;
        for (int r = 1; r < h; r++) {
            for (int c = 1; c <= w; c++) {
                bool pre = _playground[r][c];
                int count = (int)_playground[r - 1][c - 1];
                count += (int)_playground[r - 1][c];
                count += (int)_playground[r - 1][c + 1];
                count += (int)_playground[r][c - 1];
                count += (int)_playground[r][c + 1];
                count += (int)_playground[r + 1][c - 1];
                count += (int)_playground[r + 1][c];
                count += (int)_playground[r + 1][c + 1];
                bool post = count == 3 || count == 2 && pre;
                if (post ^ pre) toggle_cells.push_back({ r, c });
            }
        }
        for (auto [row, col] : toggle_cells) {
            _playground[row][col] = !_playground[row][col];
        }
    }

    void play() {
        print();
        char c = _getch();
        while (c != 13) {
            evolve1();
            print();
            c = _getch();
        }
    }

private:
    typedef bitset<w + 2> playground_row;
    typedef playground_row playground_field[h + 2];

    //template <typename F>
    //void apply(F&& f) { for (size_t i = 0; i < h; i++) f(i); }
    void apply(function<void(int)> f) { for (int i = 0; i < h; i++) f(i); }
    playground_field _playground;
    const playground_row mask_above_below = playground_row("111");
    const playground_row mask = playground_row("101");

    void evolve(int r, vector<function<void()>> &changers) {
        auto mab = mask_above_below;
        auto m = mask;
        for (int c = 1; c <= w; c++) {
            bool pre = _playground[r][c];
            auto above = _playground[r - 1] & mab;
            auto it = _playground[r] & m;
            auto below = _playground[r + 1] & mab;
            auto count = above.count() + it.count() + below.count();
            bool post = count == 3 || count == 2 && pre;
            if (post ^ pre) changers.push_back([this, post, r, c] { _playground[r][c] = post; });
            mab <<= 1;
            m <<= 1;
        }
    }
};

template <int h, int w>
GOL<h, w> build(initializer_list<const char*> def) {
    char** data = new char* [def.size()];
    for (auto [i, j] = tuple{ 0, def.begin() }; j != def.end(); i++, j++) {
        auto d = *j;
        data[i] = (char*)d;    
    }
        
    return GOL<h, w>(data);
}

int main() {
    auto blinker =
        build<5, 5>({
            "00000",
            "00100",
            "00100",
            "00100",
            "00000"
            });
    blinker.play();

    auto toad =
        build<6, 6>({
            "000000",
            "000100",
            "010010",
            "010010",
            "001000",
            "000000",
            });
    toad.play();

    auto glider =
        build<10, 10>({
            "0100000000",
            "0010000000",
            "1110000000",
            "0000000000",
            "0000000000",
            "0000000000",
            "0000000000",
            "0000000000",
            "0000000000",
            "0000000000",
            });
    glider.play();

    auto lw_spaceship =
        build<8, 20>({
            "00000000000000000000",
            "00000000000000000000",
            "01111000000000000000",
            "10001000000000000000",
            "00001000000000000000",
            "10010000000000000000",
            "00000000000000000000",
            "00000000000000000000",
            });
    lw_spaceship.play();
}
