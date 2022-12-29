#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
//#include <stack>

#define MAX 50
#define vc 100

using namespace std;
struct Graph
{
    int flag;
    int n, m, s, t;
    int mt[MAX][MAX];
};

Graph khoi_tao(int n)
{
    Graph g;
    g.flag = 0;
    g.n = n;
    g.m = g.s = g.t = 0;
    for (int i = 0; i < g.n; i++)
    {
        for (int j = 0; j < g.n; j++)
        {
            g.mt[i][j] = 0;
        }
    }
    return g;
}
Graph Doc_file(string fileName)
{
    Graph g;
    ifstream f;
    f.open(fileName, ios::in);
    if (f.fail())
    {
        cout << "\n File khong ton tai!!";
        g.flag = -1;
        return g;
    }
    int so_dinh, so_canh, batDau, ketThuc;
    f >> so_dinh;//so dinh (thanh pho)
    f >> so_canh;//so canh(thanh pho)
    f >> batDau;//bat dau tu thanh pho s
    f >> ketThuc;//toi thanh pho t

    g = khoi_tao(so_dinh);
    g.m = so_canh;
    g.s = batDau;
    g.t = ketThuc;

    for (int k = 0; k < g.m; k++)
    {
        int dinh_dau, dinh_cuoi, trong_tai;
        f >> dinh_dau;
        f >> dinh_cuoi;
        f >> trong_tai;
        //chuyen sang ma tran trong so
        g.mt[dinh_dau][dinh_cuoi] = trong_tai;
        if (g.flag == 0)
            g.mt[dinh_cuoi][dinh_dau] = trong_tai;
    }
    f.close();
    return g;
}
void Xuat_do_thi(Graph g)
{
    int n = g.n;
    cout << "So dinh: " << n << "\n";
    if (g.flag) {
        cout << "Do thi co huong.\n";
    }
    else {
        cout << "Do thi vo huong.\n";
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << setw(6) << g.mt[i][j];
        }
        cout << "\n";
    }
}
int Dijkstra(Graph Gr, int*& P, int s, int e)
{
    int temp = 0;
    int a = s, b = e;
    // Len[i] - Gia tri nho nhat tu a -> i. Len1 danh dau do dai.
    int* Len = new int[Gr.n];
    int* S = new int[Gr.n]; //Danh dau dinh thuoc danh sach dac biet
    P = new int[Gr.n]; //truy vet;
    fill(Len, Len + Gr.n, vc); //Gan duong di ban dau = vo cung
    fill(P, P + Gr.n, a);
    fill(S, S + Gr.n, 0); //Danh sach dac biet
    Len[a] = 0; // khoi tao do dai tu a->a = 0
    int i = a;
    for (int k = 0; k < Gr.n; k++)
    {
        //tim do dai ngan nhat trong cac dinh
        for (i = 0; i < Gr.n; i++) // tim v thuoc (V-S) va Len[v] < vo cung
            if (!S[i] && Len[i] != vc)
                break;
        for (int j = i + 1; j < Gr.n; j++) // tim dinh co Len max
            if (!S[j] && Len[j] > Len[i])
                i = j;
        S[i] = 1;
        
        //Len[i] = 0;
        //--------Tinh do dai tu dinh dang xet toi cac dinh tiep
        for (int j = 0; j < Gr.n; j++) //thay doi do dai neu co
        {
            if (!S[j] && Gr.mt[i][j])
                if (Len[i] + Gr.mt[i][j] > Len[j])
                {
                    Len[j] = Len[i] + Gr.mt[i][j];
                    P[j] = i; //truy vet
                    if(j != b)
                        temp += Gr.mt[i][j];
                }
            //if (j == Gr.n - 1)
            //    Len[j] -= vc;
        }
    }
    return Len[b] - vc - temp;
}
int main()
{
    Graph Gr;
    Gr = Doc_file("input.txt");
    Xuat_do_thi(Gr);
    cout << endl << "-----Thuat toan Dijkstra-----" << endl;
    int start = Gr.s;
    int end = Gr.t;
    int* P;
    int len = Dijkstra(Gr, P, start, end);
    // in ket qua
    cout << endl << "Do dai ngan nhat cua duong di tu " << start << " den " << end << " la "
        << len << endl;
    cout << "Qua trinh duong di: ";
    int i = end;
    cout << i;
    while (i != start)
    {
        cout << " <-- ";
        cout << P[i];
        i = P[i];
    }
    cout << endl;
    return 0;
}