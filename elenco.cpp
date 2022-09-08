
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

// controla a geração de relatorio
bool Generate_Report = false;

int opt = oo;
vector<bool> x_opt = {};

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

    vector<int> precos_cpy = Precos;
    sort(next(precos_cpy.begin(), from), precos_cpy.end());

    for(int i = 0; i < (Required - count); i++)
        sum += precos_cpy[from + i];

    return sum;
}


// verifica se todos os grupos foram cobertos
bool is_covered(vector<bool> to_cover) {
    for(bool g : to_cover)
        if (!g) return false;
    return true;
}


bool is_coverable(vector<bool> covered, int from, int count)
{
    for(int i = from; i < M && count < Required; i++, count++)
        for(int& g : Grupos[i])
            covered[g] = true;

    return is_covered(covered);
}


// testa se há solucao viável
bool is_viable()
{
    // testa se é possivel cobrir todos os grupos
    vector<bool> actors (M, false);
    if (!is_coverable(actors, 0, 0));

    // testa se é possível atribuir um ator pra cada personagem
    return N <= M;
}

int (*bounding)(vector<bool>, int, int) = bounding_2;

int Cuts = 0;
int Nodes_Count = 0;
void solve(int i, int count, vector<bool> actors, vector<bool> covered)
{
    cout << i << ' ' << count << '\n';
    Nodes_Count += 1;
    if (i > N || (Viability_Cut && count == Required)) {
        if (count == Required && is_covered(covered)) {
            int cost = get_cost(actors, i);
            if (cost < opt) {
                opt = cost;
                x_opt = actors;
            }
        }
    }
    else {
        
        // determina se é possivel
        bool colocar = false;
        if (Viability_Cut && is_coverable(covered, i, count))
            colocar = true;

        if (Optimality_Cut && bounding(actors, i, count) >= opt) {
            Cuts += 1;
            return;
        }
        else {
            if (!Viability_Cut || colocar) {
                // coloca o ator
                actors[i] = 1;
                vector<bool> cov_cpy (covered);
                for(int& x : Grupos[i])
                    cov_cpy[x] = true;

                solve(i+1, count + 1, actors, cov_cpy);
            }

            // não coloca o ator
            actors[i] = 0;
            solve(i+1, count, actors, covered);
        }
    }
}


void init(int argc, char * argv[])
{
    int opt;
    while((opt = getopt(argc, argv, "foav")) != -1)
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
    case 'v':
        // gera relatorio
        Generate_Report = true;
        break;
    default:
        break;
    };
}


double timestamp() {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
    return((double)(tp.tv_sec*1.0e3 + tp.tv_nsec*1.0e-6));
}


int main(int argc, char * argv[]) {
    init(argc, argv);

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

    vector<bool> actors (M, false);
    vector<bool> covered (L, false);

    double time = timestamp();
    solve(0, 0, actors, covered);
    time = timestamp() - time;

    for(int j = 0; j < x_opt.size(); j++)
        if (x_opt[j])
            cout << j + 1 << ' ';

    cout << '\n' << opt << '\n';

    if (Generate_Report) {
        cout << "\nTempo de execução: " << time << " ms\n";
        cout << "Nodos percorridos: " << Nodes_Count << '\n';
        cout << "Cortes por otimalidade: " << Cuts << '\n';
    }
}
