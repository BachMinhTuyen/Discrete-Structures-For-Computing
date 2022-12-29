#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stack>
#include <conio.h>
using namespace std;
#define MAX 50
#define vc 100
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
/*
8 11 0 3
0 1 4
0 4 4
1 2 3
1 4 4
2 3 5
2 4 2
3 6 1
3 7 7
4 5 2
5 6 5
5 7 3
*/
Graph doc_File(string fileName)
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
	f >> so_dinh;//số đỉnh (hành tinh)
	f >> so_canh;//số cạnh (số cầu vồng)
	f >> batDau;//hành tinh Adam
	f >> ketThuc;//hành tinh Eva

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
int* Dijkstra(Graph Gr, int*& P, int s, int e)
{
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
		for (int j = i + 1; j < Gr.n; j++) // tim dinh co Len min
			if (!S[j] && Len[j] < Len[i])
				i = j;
		S[i] = 1;
		//--------Tinh do dai tu dinh dang xet toi cac dinh tiep
		for (int j = 0; j < Gr.n; j++) //thay doi do dai neu co
		{
			if (!S[j] && Gr.mt[i][j])
				if (Len[i] + Gr.mt[i][j] < Len[j])
				{
					Len[j] = Len[i] + Gr.mt[i][j];
					P[j] = i; //truy vet

				}
		}
	}
	return Len;
	//return Len[b];
}
int timHanhTinh(Graph g, int start, int end)
{
	
	int* P_ongNgau, * P_baNgau;
	int* len_ongNgau = Dijkstra(g, P_ongNgau, start, end);
	int* len_baNgau = Dijkstra(g, P_baNgau, end, start);
	int min = vc;
	int hanhTinh = -1;
	for (int i = 0; i < g.n; i++)
	{
		if (len_ongNgau[i] == len_baNgau[i] && P_ongNgau[i] != 0 && P_baNgau[i] != 0)
		{
			min = len_ongNgau[i];
			hanhTinh = i;
		}
	}
	return hanhTinh;
}
void ghi_file(string fileName, Graph g, int result)
{
	ofstream file;
	file.open(fileName, ios::out);
	if (file.fail())
		cout << "\n File khong ton tai!!";
	if (result >= 0 && result < g.n)
		file << result;
	else
		file << "CRY";
	file.close();
	cout << "\nGhi file thanh cong" << endl;
}
void main()
{
	Graph g;
	g = doc_File("ONGBANGAU.INP.txt");
	Xuat_do_thi(g);
	
	int start = g.s, end = g.t;

	int result = timHanhTinh(g, start, end);
	ghi_file("ONGBANGAU.OUT.txt", g, result);
	_getch();
}