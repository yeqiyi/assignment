// Sudoku.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<string.h>
#include<math.h>
using namespace std;
int mark[10][10][10] = { 0 };
int n_size = 0;
int sgle[10][10][10] = { 0 };
int sudo[10][10] = { 0 };
bool success = false;
int times;
void single() {
	int count = 0;
	for (int i = 0; i < n_size; i++) {
		for (int j = 0; j < n_size; j++) {
			for (int k = 0; k<n_size; k++)//判断行
				if (sudo[k][j] && !sgle[i][j][sudo[k][j]]) {
					count++;
					sgle[i][j][sudo[k][j]] = 1;
				}
			for (int k = 0; k < n_size; k++) //判断列
				if (sudo[i][k] && !sgle[i][j][sudo[i][k]]) {
					count++;
					sgle[i][j][sudo[i][k]] = 1;
				}
			if (n_size == 4 || n_size == 6 || n_size == 8 || n_size == 9)//判断宫 
			{
				if (n_size == 4 || n_size == 9) {
					int n = (int)sqrt(n_size);
					int k1 = i / n;
					int k2 = j / n;
					k1 *= n;
					k2 *= n;
					for (int k = k1; k < k1 + n; k++)
						for (int l = k2; l < k2 + n; l++)
							if (sudo[k][l] && !sgle[i][j][sudo[k][l]]) {
								count++;
								sgle[i][j][sudo[k][l]] = 1;
							}
				}
				else if (n_size == 6) {
					int k1 = i / 2;
					int k2 = j / 3;
					k1 *= 2;
					k2 *= 3;
					for (int k = k1; k < k1 + 2; k++)
						for (int l = k2; l < k2 + 3; l++)
							if (sudo[k][l] && !sgle[i][j][sudo[k][l]]) {
								sgle[i][j][sudo[k][l]] = 1;
								count++;
							}
				}
				else if (n_size == 8) {
					int k1 = i / 4;
					int k2 = j / 2;
					k1 *= 4;
					k2 *= 2;
					for (int k = k1; k < k1 + 4; k++)
						for (int l = k2; l < k2 + 2; l++)
							if (sudo[k][l] && !sgle[i][j][sudo[k][l]]) {
								sgle[i][j][sudo[k][l]] = 1;
								count++;
							}
				}
			}
			if (count == n_size - 1) {
				for (int k = 1; k <= n_size; k++)
				{
					if (!sgle[i][j][k] && !sudo[i][j]) {
						sudo[i][j] = k;
						break;
					}
				}
			}
			count = 0;
		}
	}
}
bool search(int x, int y)//查找当前位置可能存在的解
{
	for (int i = 0; i < n_size; i++) {
		mark[x][y][sudo[i][y]] = 1;//判断列
	}
	for (int i = 0; i < n_size; i++) {
		mark[x][y][sudo[x][i]] = 1;//判断行
	}
	if (n_size == 4 || n_size == 6 || n_size == 8 || n_size == 9) {
		if (n_size == 4 || n_size == 9) {
			int n = (int)sqrt(n_size);
			int k1 = x / n;
			int k2 = y / n;
			k1 *= n;
			k2 *= n;
			for (int i = k1; i < k1 + n; i++)
				for (int j = k2; j < k2 + n; j++) {
					mark[x][y][sudo[i][j]] = 1;
				}
		}
		else if (n_size == 6) {
			int k1 = x / 2;
			int k2 = y / 3;
			k1 *= 2;
			k2 *= 3;
			for (int i = k1; i < k1 + 2; i++)
				for (int j = k2; j < k2 + 3; j++) {
					mark[x][y][sudo[i][j]] = 1;
				}
		}
		else if (n_size == 8) {
			int k1 = x / 4;
			int k2 = y / 2;
			k1 *= 4;
			k2 *= 2;
			for (int i = k1; i < k1 + 4; i++)
				for (int j = k2; j < k2 + 2; j++) {
					mark[x][y][sudo[i][j]] = 1;
				}
		}
	}
	for (int i = 1; i <= n_size; i++) {
		if (mark[x][y][i] == 0) {
			return true;
		}
	}
	return false;
}
void sudoku(int x, int y, char*o) //深度优先搜索，数独求解函数
{
	for (int i = 1; i <= n_size; i++)
	{
		mark[x][y][i] = 0;
	}
	if (x == n_size) {
		success = true;
		ofstream InFire(o, ios::app);
		for (int i = 0; i < n_size; i++) {
			for (int j = 0; j < n_size; j++)
				InFire << sudo[i][j] << ' ';
			InFire << endl;
		}
		InFire << endl;
	}
	else if (x < n_size&&y < n_size && !sudo[x][y]) {
		if (search(x, y)) {
			for (int k = 1; k <= n_size; k++) {
				if (!mark[x][y][k]) {
					sudo[x][y] = k;
					if (y < n_size - 1) {
						sudoku(x, y + 1, o);
					}
					else if (x < n_size) {
						sudoku(x + 1, 0, o);
					}
					sudo[x][y] = 0;
				}
			}
		}
		else
			return;
	}
	else if (x < n_size&&y < n_size&&sudo[x][y]) {
		if (y < n_size - 1) {
			sudoku(x, y + 1, o);
		}
		else if (x < n_size) {
			sudoku(x + 1, 0, o);
		}
	}
}
int main(int argc, char*argv[]) {
	FILE *fp;
	char *file_path = { "./" }, *output = { "./" };
	for (int i = 0; i < argc; i++)//读取命令行参数（这里借鉴了李承泽同学的方法）
	{
		if (strlen(argv[i]) == 1)
		{
			if (i == 2)
				n_size = atoi(argv[i]);
			if (i == 4)
				times = atoi(argv[i]);
		}
		if (i == 6)
			file_path = argv[i];
		if (i == 8)
			output = argv[i];
	}
	fopen_s(&fp,file_path, "r");
	if (fp == NULL)
		return -1;
	for (int t = 1; t <= times; t++)//读入盘面
	{
		for (int i = 0; i < n_size; i++) {
			for (int j = 0; j < n_size; j++)
				fscanf_s(fp, "%d", &sudo[i][j]);
		}
		cout << endl;
		single();//判断初始盘面中存在的唯一解
		ofstream InFire(output, ios::app);
		InFire << "盘面" << t << ":" << endl;
		sudoku(0, 0, output);
		if (!success)
			InFire << "No answer!!" << endl;
		success = false;//完成一次盘面计算后数据重新初始化
		memset(sudo, 0, sizeof(sudo));
		memset(mark, 0, sizeof(mark));
		memset(sgle, 0, sizeof(sgle));
	}
	return 0;
}

