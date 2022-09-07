
#include <unistd.h>
#include <bits/stdc++.h>
using namespace std;
#define oo 1987654321

int L, M, N;
vector<int> Precos;
vector<vector<int>> Grupos;
int Required;


// controla o corte por otimalidade
bool Optimality_Cut = true;
// controla o corte por viabilidade
bool Viability_Cut = true;

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


int bounding_1(vector<bool> actors, int from, int count)
{
    int sum = get_cost(actors, from);

    int min = *min_element(next(Precos.begin(), from), Precos.end());

    return sum + min * (Required - count);
}


int bounding_2(vector<bool> actors, int from, int count)
{
    int sum = get_cost(actors, from);

    int min = *min_element(next(Precos.begin(), from), Precos.end());

    return sum + min * (Required - count);
}


// verifica se todos os grupos foram cobertos
bool is_covered(vector<bool> cover)
{
    for (bool g : cover)
        if (!g)
            return false;            
    return true;
}


bool is_coverable(vector<bool>& cover, int from)
{
    vector<bool> to_cover(cover);
    for(int i = from; i < L; i++)
        for(int& x : Grupos[i])
            to_cover[x] = true;
    return is_covered(to_cover);
}


// testa se há solucao viável
bool is_viable()
{
    // testa se é possivel cobrir todos os grupos
    vector<bool> to_cover (L, false);
    if (!is_coverable(to_cover, 0));

    // testa se é possível atribuir um ator pra cada personagem
    return N <= M;
}

int (*bounding)(vector<bool>, int, int) = bounding_2;

void solve(int i, vector<bool> actors, int count, vector<bool> covered)
{
    if (i > N || count == Required) {
        if (is_covered(covered) && count == Required) {
            int cost = get_cost(actors, i);
            if (cost < opt) {
                opt = cost;
                x_opt = actors;
            }
        }
    }
    else {
        if (Optimality_Cut && bounding(actors, i+1, count) >= opt)
            return;
        else {
            // coloca o ator
            if (Viability_Cut && count < Required) {
                vector<bool> actors_copy (actors);
                vector<bool> covered_copy (covered);

                actors_copy[i] = true;

                for(int& g : Grupos[i])
                    covered_copy[g] = true;

                solve(i+1, actors_copy, count + 1, covered_copy);
            }
            
            // não coloca o ator
            solve(i+1, actors, count, covered);
        }
    }
}


void init(int argc, char * argv[])
{
    int opt;
    while((opt = getopt(argc, argv, "foa")) != -1)
    switch(opt)
    {
    case 'f':
        // desliga corte por viabilidade
        Viability_Cut = false;
        break;
    case 'o':
        // desliga cortes por otimalidade
        Optimality_Cut = false;
        break;
    case 'a':
        // usa funcao limitante do professor
        bounding = bounding_1;
        break;
    default:
        break;
    };
}



int main(int argc, char * argv[]) {
    cin.tie(0);

    cin >> L >> M >> N;

    Precos = vector<int> (M);
    Grupos = vector<vector<int>> (M);
    Required = N;

    int v, s, g;
    for(int i = 0; i < M; i++) {
        cin >> v >> s;
        Precos[i] = v;
        while(s--) {
            cin >> g;
            Grupos[i].push_back(--g);
        }
    }

    if (!is_viable()) {
        cout << "Inviável\n";
        exit(0);
    }

    vector<bool> covered (L, false);
    vector<bool> actors (M, false);

    solve(0, actors, 0, covered);

    for(int j = 0; j < x_opt.size(); j++)
        if (x_opt[j])
            cout << j + 1 << ' ';

    cout << '\n' << opt << '\n';
}
