
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

// Nodos percorridos
int Nodes_Count = 0;

// controla o corte por otimalidade
bool Optimality_Cut = true;
int O_Cuts = 0;

// controla o corte por viabilidade
bool Viability_Cut = true;
int V_Cuts = 0;

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


// bounding do professor
int bounding_1(vector<bool> actors, int from, int count)
{
    int sum = get_cost(actors, from);

    int min = Atores[from].preco;

    return sum + min * (Required - count);
}


// nosso bounding
int bounding_2(vector<bool> actors, int from, int count)
{
    int sum = get_cost(actors, from);

    for(int i = 0; i < (Required - count); i++)
        sum += Atores[from + i].preco;

    return sum;
}


bool is_covered(vector<int> grupos)
{
    for(int& x : grupos)
        if (x <= 0) return false;
    return true;
}

// valores otimos
int opt = oo;
vector<bool> x_opt = {};

// funcao de bounding a ser utilizada
int (*bounding)(vector<bool>, int, int) = bounding_2;

void solve(int i, int count, vector<bool>& actors, vector<int> grupos)
{
    Nodes_Count += 1;

    if (i == M) {
        if (count == Required && is_covered(grupos)) {
            int cost = get_cost(actors, i);
            if (cost < opt) {
                opt = cost;
                x_opt = actors;
            }
        }
    }
    else {
        if (Viability_Cut && (Required - count > M - i || count > Required)) {
            V_Cuts += 1;
            return;
        }

        int bound = bounding(actors, i, count);

        if (!Optimality_Cut || bound < opt) {
            // coloca o ator
            actors[i] = 1;
            solve(i+1, count + 1, actors, grupos);

            // não coloca o ator
            if (!Optimality_Cut || bound < opt) {
                actors[i] = 0;

                // verifica se não colocar o ator irá quebrar a viabilidade
                for(int& x : Atores[i].grupos) {
                    grupos[x] -= 1;
                    if (Viability_Cut && grupos[x] == 0) {
                        V_Cuts += 1;
                        return;
                    }
                }
                solve(i+1, count, actors, grupos);
            }
            else
                O_Cuts += 1;
        }
        else
            O_Cuts += 1;        
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

    vector<int> Grupos (L, 0);

    if (M < Required) {
        cout << "Inviável\n";
        exit(0);
    }

    int v, s, g;
    for(int i = 0; i < M; i++) {
        cin >> v >> s;
        Atores.push_back({i, v});
        while(s--) {
            cin >> g;
            g--;
            Grupos[g] += 1;
            Atores[i].grupos.push_back(g);
        }
    }

    // ordena o vetor de atores pelo preco do maior pro menor
    sort(Atores.begin(), Atores.end(), [](Ator a, Ator b) {return a.preco < b.preco;});

    vector<bool> actors (M, false);

    double time = timestamp();
    solve(0, 0, actors, Grupos);
    time = timestamp() - time;

    // verifica se achou uma solução
    if (opt == oo) {
        cout << "Inviável\n";
        exit(0);
    }

    // processa o x_opt
    vector<int> out;
    for(int j = 0; j < x_opt.size(); j++)
        if (x_opt[j])
            out.push_back(Atores[j].id + 1);
    sort(out.begin(), out.end());

    if (Generate_Report) {
        cout << "Tempo de execução: " << time << " segundos\n";
        cout << "Nodos percorridos: " << Nodes_Count << '\n';
        cout << "Cortes por otimalidade: " << O_Cuts << '\n';
        cout << "Cortes por viabilidade: " << V_Cuts << '\n';
    }

    for(int& a : out) cout << a << ' ';
    cout << '\n' << opt << '\n';
}
