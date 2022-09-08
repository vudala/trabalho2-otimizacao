
#include <unistd.h>
#include <bits/stdc++.h>
using namespace std;
#define oo 1987654321

int L, M, Required;
struct Ator {
    int id, preco;
    vector<int> grupos;
};
vector<Ator> Atores;

// controla o corte por otimalidade
bool Optimality_Cut = true;

// controla o corte por viabilidade
bool Viability_Cut = true;

// controla a geração de relatorio
bool Generate_Report = false;

int get_cost(vector<bool>& actors, int to)
{
    int sum = 0;
    for(int i = 0; i < to; i++)
        if (actors[i])
            sum += Atores[i].preco;
    return sum;
}


int bounding_1(vector<bool> actors, int from, int count)
{
    int sum = get_cost(actors, from);

    int min = Atores[from].preco;

    return sum + min * (Required - count);
}


int bounding_2(vector<bool> actors, int from, int count)
{
    int sum = get_cost(actors, from);

    for(int i = 0; i < (Required - count); i++)
        sum += Atores[from + i].preco;

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
        for(int& g : Atores[i].grupos)
            covered[g] = true;

    return is_covered(covered);
}


// testa se há solucao viável
bool is_viable()
{
    // testa se é possivel cobrir todos os grupos
    vector<bool> covered (L, false);
    if (!is_coverable(covered, 0, 0))
        return false;

    // testa se é possível atribuir um ator pra cada personagem
    return Required <= M;
}


// valores otimos
int opt = oo;
vector<bool> x_opt = {};


// informacoes sobre a execução
int Cuts = 0;
int Nodes_Count = 0;


// funcao de bounding a ser utilizada
int (*bounding)(vector<bool>, int, int) = bounding_2;

void solve(int i, int count, vector<bool> actors, vector<bool> covered)
{
    cout << i << ' ' << count << '\n';
    Nodes_Count += 1;
    if (i > M || (Viability_Cut && count == Required)) {
        if (count == Required && is_covered(covered)) {
            int cost = get_cost(actors, i);
            if (cost < opt) {
                opt = cost;
                x_opt = actors;
            }
        }
    }
    else {
        // determina se deve cortar
        bool cortar = false;
        if (Viability_Cut && !is_coverable(covered, i, count))
            cortar = true;

        if (Optimality_Cut && bounding(actors, i, count) >= opt) {
            Cuts += 1;
            return;
        }
        else {
            if (!Viability_Cut || !cortar) {
                // coloca o ator
                actors[i] = 1;
                vector<bool> cov_cpy (covered);
                for(int& x : Atores[i].grupos)
                    cov_cpy[x] = true;

                solve(i+1, count + 1, actors, cov_cpy);

                // não coloca o ator
                actors[i] = 0;
                solve(i+1, count, actors, covered);
            }
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

    cin >> L >> M >> Required;

    int v, s, g;
    for(int i = 0; i < M; i++) {
        cin >> v >> s;
        Atores.push_back({i, v});
        while(s--) {
            cin >> g;
            Atores[i].grupos.push_back(--g);
        }
    }

    if (!is_viable()) {
        cout << "Inviável\n";
        exit(0);
    }

    // ordena o vetor de atores pelo preco do maior pro menor
    sort(Atores.begin(), Atores.end(), [](Ator a, Ator b) {return a.preco < b.preco;});

    vector<bool> actors (M, false);
    vector<bool> covered (L, false);

    double time = timestamp();
    solve(0, 0, actors, covered);
    time = timestamp() - time;

    // processa o x_opt
    vector<int> out;
    for(int j = 0; j < x_opt.size(); j++)
        if (x_opt[j])
            out.push_back(Atores[j].id + 1);

    sort(out.begin(), out.end());

    for(int& a : out) cout << a << ' ';
    cout << '\n' << opt << '\n';

    if (Generate_Report) {
        cout << "\nTempo de execução: " << time << " ms\n";
        cout << "Nodos percorridos: " << Nodes_Count << '\n';
        cout << "Cortes por otimalidade: " << Cuts << '\n';
    }
}
