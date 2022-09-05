

#include <bits/stdc++.h>
using namespace std;
#define oo 1987654321

vector<int> Precos;
vector<vector<int>> Grupos;
int Required;

int opt = oo;
vector<bool> x_opt;

int get_cost(vector<bool>& actors, int to)
{
    int sum = 0;
    for(int i = 0; i < to; i++)
        if (actors[i])
            sum += Precos[i];
    return sum;
}

// conta quantos atores foram escalados
int count_actors(vector<bool> actors)
{
    int cnt = 0;
    for (bool a : actors)
        if (a)
            cnt++;
    return cnt;
}


int bounding_1(vector<bool> actors, int from)
{
    int sum = get_cost(actors, from);

    int min = *min_element(next(Precos.begin(), from), Precos.end());

    return sum + min * (Required - count_actors(actors));
}


int bounding_2(vector<bool> actors, int from)
{
    int sum = get_cost(actors, from);

    int min = *min_element(next(Precos.begin(), from), Precos.end());

    return sum + min * (Required - count_actors(actors));
}


// verifica se todos os grupos foram cobertos e se há actors o suficiente
bool is_covered(vector<bool> cov_groups, vector<bool> actors)
{
    for (bool g : cov_groups)
        if (!g)
            return false;            
    return count_actors(actors) == Required;
}


// testa se há solucao viável
bool is_viable(int l, int m, int n)
{
    // testa se é possivel cobrir todos os grupos
    vector<bool> to_cover(l, false);
    for(vector<int>& g : Grupos)
        for(int& x : g)
            to_cover[x] = true;
    for(bool c : to_cover)
        if (!c)
            return false;

    // testa se é possível atribuir um ator pra cada personagem
    return n <= m;
}

int (*bounding)(vector<bool>, int) = bounding_1;

void solve(int i, int n, vector<bool> actors, vector<bool> covered)
{
    if (i > n) {
        if (is_covered(covered, actors)) {
            int cost = get_cost(actors, actors.size());
            if (cost < opt) {
                opt = cost;
                x_opt = actors;
            }
        }
    }
    else {
        if (bounding(actors, i+1) < opt) {
            // coloca o ator
            vector<bool> actors_copy (actors);
            vector<bool> covered_copy (covered);

            actors_copy[i] = true;

            for(int& g : Grupos[i])
                covered_copy[g] = true;

            solve(i+1, n, actors_copy, covered_copy);

            // não coloca o ator
            solve(i+1, n, actors, covered);
        }
    }
}


int main() {
    int l, m, n;

    cin >> l >> m >> n;

    Precos = vector<int> (m);
    Grupos = vector<vector<int>> (m);
    Required = n;

    int v, s, g;
    for(int i = 0; i < m; i++) {
        cin >> v >> s;
        Precos[i] = v;
        while(s--) {
            cin >> g;
            Grupos[i].push_back(--g);
        }
    }

    if (!is_viable(l, m, n)) {
        cout << "Inviável\n";
        exit(0);
    }

    vector<bool> covered (l, false);
    vector<bool> actors (m, false);

    solve(0, n, actors, covered);

    for(int j = 0; j < x_opt.size(); j++)
        if (x_opt[j])
            cout << j + 1 << ' ';

    cout << '\n' << opt << '\n';
}
