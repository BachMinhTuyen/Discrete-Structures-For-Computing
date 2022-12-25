//Source code này chạy ổn định với đồ thị vô hướng, có hướng thì (đang kiểm tra ...)
//Source code này chỉ chạy ổn định với đồ thị bắt đầu từ đỉnh 0.
//Đồ thị bài này gồm 5 đỉnh là 0, 1, 2, 3, 4
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stack>
#include <conio.h>
using namespace std;
constexpr auto MAX = 50;
constexpr auto vc = 100;
struct Graph
{
	int flag;
	int n;
	int m[MAX][MAX];
};
/*
0
5
0 1 1 1 1
1 0 1 1 0
1 1 0 1 0
1 1 1 0 1
1 0 0 1 0
*/
// Đọc ma trận kề
Graph Doc_ma_tran_ke(string fileName)
{
	Graph g;
	ifstream fileIn;
	fileIn.open(fileName, ios::in);
	if (fileIn.fail())
	{
		cout << "\nFile khong ton tai !!!";
		g.flag = -1;
		return g;
	}
	fileIn >> g.flag;
	fileIn >> g.n;
	for (int i = 0; i < g.n; i++)
	{
		for (int j = 0; j < g.n; j++)
		{
			fileIn >> g.m[i][j];
		}
	}
	fileIn.close();
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
			cout << setw(6) << g.m[i][j];
		}
		cout << "\n";
	}
}
// Tìm đường đi Euler
// Kiểm tra số đỉnh bậc lẻ có nhiều hơn 2 hay không
int dem_so_dinh_bac_le(Graph g)
{
	int v = 0;
	int i, j, k, z, dem = 0;
	int tongbacvao, tongbacra;
	if (g.flag == 0) // vô hướng
	{
		for (i = 0; i < g.n; ++i)
		{
			k = 0;
			for (j = 0; j < g.n; ++j)
			{
				k += g.m[i][j];
			}
			if (k % 2 == 1)
			{
				v = i;
				dem++;
			}
		}
	}
	else // có hướng
	{ // đếm số đỉnh có |tổng bậc ra - tổng bậc vào| =1
		for (i = 0; i < g.n; ++i)
		{
			tongbacvao = 0;
			tongbacra = 0;
			for (z = 0; z < g.n; ++z) //Tính tổng bậc vào (cột)
				tongbacvao += g.m[z][i];
			for (j = 0; j < g.n; ++j) //Tính tổng bậc ra (hàng)
				tongbacra += g.m[i][j];
			if (tongbacvao - tongbacra != 0)
			{
				v = i;
				dem++;
			}
		}
	}
	return dem;
} 
// Duyệt đồ thị sử dụng DFS stack
void duyet_dothi(Graph g, int v, int daxet[MAX])
{
	// Khởi tạo lại đã xét
	for (int i = 0; i < g.n; ++i)
		daxet[i] = 0;
	stack<int> P;
	daxet[v] = 1;
	P.push(v);
	while (P.size() > 0)
	{
		v = P.top();
		daxet[v] = 1;
		P.pop();
		for (int i = g.n - 1; i >= 0; i--)
		{
			if (g.m[v][i] != 0 && daxet[i] == 0)
			{
				P.push(i);
				daxet[i] = 1;
			}
		}
	}
}
// Kiểm tra đồ thị có liên thông không
bool kiem_tra_lien_thong(Graph g)
{
	int daxet[MAX];
	duyet_dothi(g, 0, daxet);
	for (int i = 1; i <= g.n; ++i)
	{
		if (daxet[i] == 0)
			return false;
	}
	return true;
}
// Đếm số đỉnh có thể đến được từ đỉnh v
int Dem_so_dinh_den_duoc(Graph g, int v, bool daduyet[])
{
	daduyet[v] = true;
	int count = 1;
	for (int i = 0; i < g.n; i++)
		if (g.m[v][i] > 0 && !daduyet[i])
		{
			count += Dem_so_dinh_den_duoc(g, i, daduyet);
		}
	return count;
}
// Kiểm tra đồ thị có đường Euler không
bool KT_Euler(Graph g)
{
	if (g.flag == 0) //loại đồ thị vô hướng
	{
		if (!kiem_tra_lien_thong(g) || dem_so_dinh_bac_le(g) > 2)
			return false;
	}
	else // loại đồ thị có hướng
	{
		if (dem_so_dinh_bac_le(g) > 2)
			return false;
	}
	return true;
}
bool KT_canh_hop_le(Graph g, int u, int v)
{
	// Cạnh hợp lệ thỏa 1 trong 2 điều kiện
	// 1. v là đỉnh kề duy nhất của u
	int count = 0;
	for (int i = 0; i < g.n; i++) if (g.m[u][i] > 0) count++;
	if (count == 1) return true;
	// 2. v không phải đỉnh kề duy nhất của u và (u, v) không phải cầu
	bool daduyet[MAX];
	memset(daduyet, false, g.n);
	int count1 = Dem_so_dinh_den_duoc(g, u, daduyet);
	// Xóa cạnh (u, v)
	g.m[u][v] = 0;
	g.m[v][u] = 0;
	memset(daduyet, false, g.n);
	int count2 = Dem_so_dinh_den_duoc(g, u, daduyet);
	if (count1 > count2) return false;
	else return true;
}
// Hàm đệ quy in ra đường đi Euler (hoặc chu trình Euler)
void In_duong_di_Euler(Graph& g, int u)
{
	// Đệ quy qua tất cả các đỉnh kề của đỉnh này
	for (int i = 0; i < g.n; i++)
	{
		if (g.m[u][i] > 0)
		{
			int v = i;
			if (KT_canh_hop_le(g, u, v))
			{
				cout << u << "-" << v << " ";
				g.m[u][v] = 0;
				g.m[v][u] = 0;
				In_duong_di_Euler(g, v);
			}
		}
	}
}
int Tim_dinh_bac_le(Graph g)
{
	for (int i = 0; i < g.n; i++)
	{
		int count = 0;
		for (int j = 0; j < g.n; j++)
			if (g.m[i][j] > 0) count++;
		if (count % 2 != 0) return i;
	}
	return 0;
}
int main()
{
	Graph G;
	G = Doc_ma_tran_ke("input2.txt");
	Xuat_do_thi(G);
	// Copy G sang g
	Graph g;
	g.flag = G.flag;
	g.n = G.n;
	for (int i = 0; i < g.n; i++)
	{
		for (int j = 0; j < g.n; j++)
		{
			g.m[i][j] = G.m[i][j];
		}
	}
	bool daduyet[MAX];
	memset(daduyet, false, g.n);
	cout << "Thanh phan lien thong: " << Dem_so_dinh_den_duoc(g, 0, daduyet);
	if (!KT_Euler(g))
	{
		cout << endl << "Do thi da cho khong co duong di Euler." << endl;
	}
	else
	{
		cout << endl << "Duong di Euler cua do thi da cho la: " << endl;
		int u = Tim_dinh_bac_le(g);
		In_duong_di_Euler(g, u);
	}
	cout << endl;
	_getch();
	return 0;
}