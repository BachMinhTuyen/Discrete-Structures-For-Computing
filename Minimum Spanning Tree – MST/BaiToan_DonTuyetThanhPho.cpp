//Source code này chỉ chạy ổn định với đồ thị bắt đầu từ đỉnh 1.
//Đồ thị bài này gồm 5 đỉnh là 1, 2, 3, 4, 5
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <conio.h>
using namespace std;

#define MAX 50
struct Graph
{
	int flag; //0 là vô hướng, 1 là có hướng
	int n, m; //số đỉnh, điểm bắt đầu
	int mt[MAX][MAX]; //ma trận trọng số
};
Graph khoi_tao(int n)
{
	Graph g;
	g.flag = 0;
	g.n = n;
	g.m = 0;
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
5 2
1 2 3
1 4 4
1 5 1
2 3 3
2 5 1
3 4 2
*/
//Hàm đọc danh sách cạnh có trọng số
Graph Doc_file(string fileName)
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
	int n, dinh_dau_tien;
	fileIn >> n; //số giao lộ (số đỉnh)
	fileIn >> dinh_dau_tien; //đỉnh xuất phát
	g = khoi_tao(n);
	g.m = dinh_dau_tien;

	int k = 0;
	while (!fileIn.eof())
	{
		int dinh_dau, dinh_cuoi, trong_so;
		fileIn >> dinh_dau;
		fileIn >> dinh_cuoi;
		fileIn >> trong_so;

		//Chuyển sang ma trận trọng số
		if (g.flag == 0)
		{
			g.mt[dinh_dau - 1][dinh_cuoi - 1] = trong_so;
			g.mt[dinh_cuoi - 1][dinh_dau - 1] = trong_so;
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
			cout << setw(6) << g.mt[i][j];
		}
		cout << "\n";
	}
}
//// Cây khung nhỏ nhất

//Hàm tìm đỉnh kề có trọng số nhỏ nhất
int minKey(Graph g, int key[], bool mstSet[])
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < g.n; v++)
		if (mstSet[v] == false && key[v] < min)
			min = key[v], min_index = v;

	return min_index;
}

//Hàm in thông tin
void printMST(Graph g, int parent[], int graph[][MAX], int sum)
{
	cout << "\nDanh sach cac canh thuoc cay khung nho nhat" << endl;
	cout << "Canh \tTrong so\n";

	//for (int i = 1; i < g.n; i++)
	for (int i = 0; i < g.n; i++)
	{
		//Kiểm tra vị trí bắt đầu
		if (parent[g.m - 1] == -1 && parent[i] == -1)
			continue;
		cout << parent[i] + 1 << " - " << i + 1 << " \t"
			<< graph[i][parent[i]] << " \n";
	}
	cout << "Tong so nhien lieu can co la: " << sum << endl;
}

//Hàm xây dựng và in cây khung nhỏ nhất với đầu vào được biểu diễn bằng ma trận kề
int primMST(Graph g, int graph[][MAX])
{
	//Mảng lưu trữ đỉnh thuộc cây khung nhỏ nhất
	int parent[MAX];

	//Mảng lưu giá trị được sử dụng để chọn cạnh có trọng số nhỏ
	int key[MAX];

	// To represent set of vertices included in MST
	bool mstSet[MAX];

	// Initialize all keys as INFINITE
	for (int i = 0; i < g.n; i++)
		key[i] = INT_MAX, mstSet[i] = false;

	// Khóa (được chọn) làm đỉnh xuất phát đầu tiên
	key[g.m - 1] = 0;
	parent[0] = -1; // Nút đầu tiên luôn là gốc của cây khung nhỏ nhất

	//khởi tạo đỉnh thuộc cây khung nhỏ nhất là  -1
	for (int i = 0; i < g.n; i++)
		parent[i] = -1;

	int sum = 0;
	
	for (int count = 0; count < g.n - 1; count++)
	{
		//đỉnh gốc đang xét
		int u = minKey(g, key, mstSet);		
		int dinh_dau = u;

		//Thêm đỉnh đã chọn vào mstSet
		mstSet[u] = true;

		//Duyệt các đỉnh kề và trọng số với đỉnh gốc đang xét
		for (int v = 0; v < g.n; v++)
		{
			if (graph[u][v] && mstSet[v] == false
				&& graph[u][v] < key[v])
				parent[v] = u, key[v] = graph[u][v];
		}
		//lưu đỉnh kề với trọng số nhỏ nhất
		int dinh_cuoi = minKey(g, key, mstSet);
		int check = sum;
		sum += graph[dinh_dau][dinh_cuoi];
		//kiểm tra nếu đi hết đường và quay lại thì ..
		if (check == sum)
		{
			dinh_dau = g.m - 1;
			dinh_cuoi = minKey(g, key, mstSet);
			sum += sum;
			sum += graph[dinh_dau][dinh_cuoi];
		}
	}
	
	printMST(g, parent, graph, sum);
	return sum;
}
void Ghi_file(int result)
{
	ofstream MyFile("ONE.OUT.txt");
	MyFile << result << endl;
	MyFile.close();
}
//File này chỉ đọc file mà đồ thị có đỉnh bắt đầu từ 1 
int main()
{
	
	Graph g;
	g = Doc_file("ONE.INP.txt");
	Xuat_do_thi(g);

	int result = primMST(g, g.mt);
	Ghi_file(result);

	_getch();
	return 0;
}