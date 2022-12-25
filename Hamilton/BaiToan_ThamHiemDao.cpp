//Source code này chỉ chạy ổn định với đồ thị bắt đầu từ đỉnh 0.
//Đồ thị bài này gồm 7 cạnh và 5 đỉnh là 0, 1, 2, 3, 4
#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <stdlib.h>
using namespace std;

struct Graph
{
    int flag; //có hướng hay vô hướng
    int n, m; //số đỉnh, số cạnh 
    int tg[100];//thời gian tham quan đảo
    int mt[100][100]; //ma trận trọng số
};
int printSolution(Graph g, int path[], int n);

/*Kiểm tra xem đỉnh v có thể được thêm vào chỉ mục 'pos' trong Chu trình Hamilton*/
int isSafe(int v, int graph[][100], int path[], int pos)
{
    /* Kiểm tra xem đỉnh này có phải là đỉnh liền kề với đỉnh đã thêm trước đó */
    if (graph[path[pos - 1]][v] == 0)
        return 0;

    for (int i = 0; i < pos; i++)
        if (path[i] == v)
            return 0;

    return 1;
}

/*Hàm đệ quy để giải bài toán chu trình hamilton*/
int hamCycleUtil(Graph g, int graph[][100], int path[], int pos, int n)
{
    if (pos == n)
    {
        if (graph[path[pos - 1]][path[0]] != 0)
            return 1;
        else
            return 0;
    }

    for (int v = 1; v < n; v++)
    {
        if (isSafe(v, graph, path, pos))
        {
            path[pos] = v;
            if (hamCycleUtil(g, graph, path, pos + 1, n) == 1)
                return 1;

            path[pos] = -1;
        }
    }

    return 0;
}

/*Hàm này giải quyết vấn đề chu kỳ Hamilton bằng cách sử dụng backtracking*/
int hamCycle(Graph g, int graph[][100], int n)
{
    int* path = new int[n];
    for (int i = 0; i < n; i++)
        path[i] = -1;

    /*Đặt đỉnh 0 là đỉnh đầu tiên trong đường dẫn.
    Nếu tồn tại Chu trình Hamilton thì đường đi có thể là bắt đầu
    từ bất kỳ điểm nào của chu trình vì đồ thị là vô hướng*/
    path[0] = 0;
    if (hamCycleUtil(g, graph, path, 1, n) == 0)
    {
        cout << "\nKhong ton tai chu trinh Hamiltonian";
        return -1;
    }
    return printSolution(g, path, n);
}

int printSolution(Graph g, int path[], int n)
{
    int sum = 0;
    cout << "Ton tai chu trinh Hamiltonian\n";
    for (int i = 0; i < n; i++)
    {
        cout << path[i] << " ";
        //Thời gian trên mỗi đảo
        sum += g.tg[i];
    }
    
    /*in đỉnh đầu tiên một lần nữa 
    để hiển thị chu trình hoàn chỉnh*/
    cout << path[0] << " ";
    cout << endl;

    //Thời gian di chuyển giữa các đảo
    int k = 0;
    int dau, cuoi;
    while (k < n)
    {
        dau = path[k];
        if (k == n - 1)
        {
            cuoi = path[0];
            k++;
        }
        else
            cuoi = path[++k];
        
        sum += g.mt[dau][cuoi];
    };
    cout << "Tong thoi gian de hoan thanh nhiem vu: " << sum << endl;
    return sum;
}
Graph khoi_tao(int n)
{
    Graph g;
    g.flag = 0;
    g.n = n;
    for (int i = 0; i < g.n; i++)
    {
        for (int j = 0; j < g.n; j++)
        {
            g.mt[i][j] = 0;
        }
    }
    return g;
}
/*
5 7
3 3 2 4 3
0 1 3
0 3 4
1 2 4
1 3 5
1 4 2
2 4 6
3 4 4
*/
//Hàm đọc danh sách cạnh có trọng số
Graph doc_file(string fileName)
{
    Graph g;
    ifstream f;
    f.open(fileName, ios::in);
    if (f.fail())
    {
        cout << "\nFile khong ton tai !!!";
        g.flag = -1;
        return g;
    }
    int so_dinh, so_canh;
    f >> so_dinh; //số đỉnh (đảo)
    f >> so_canh; //số cạnh (đường đi giữa các đảo)

    g = khoi_tao(so_dinh);
    g.m = so_canh;

    for (int i = 0; i < g.n; i++)
        f >> g.tg[i];

    for (int k = 0; k < g.m; k++)
    {
        int dinh_dau, dinh_cuoi, thoi_gian_di;
        f >> dinh_dau;
        f >> dinh_cuoi;
        f >> thoi_gian_di;
        //Chuyển sang ma trận trọng số
        g.mt[dinh_dau][dinh_cuoi] = thoi_gian_di;
        g.mt[dinh_cuoi][dinh_dau] = thoi_gian_di;
    }
    f.close();
    return g;
}
void Xuat_do_thi(Graph g)
{
    cout << "So dinh: " << g.n << endl;
    if (g.flag)
        cout << "Do thi co huong\n";
    else
        cout << "Do thi vo huong\n";
    for (int i = 0; i < g.n; i++)
    {
        for (int j = 0; j < g.n; j++)
        {
            cout << g.mt[i][j] << " ";
        }
        cout << endl;
    }
}
void Ghi_file_ket_qua(int result)
{
    ofstream f;
    f.open("LOAN.OUT.txt", ios::out);
    f << result << endl;
    //cout << "Ghi file ket qua thanh cong !!!" << endl;
    f.close();
}
int main()
{
    Graph dothi;

    dothi = doc_file("LOAN.INP.txt");
    Xuat_do_thi(dothi);

    int result = hamCycle(dothi, dothi.mt, dothi.n);

    Ghi_file_ket_qua(result);

    _getch();
    return 0;
}