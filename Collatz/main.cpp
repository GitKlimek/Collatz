#define _CRT_SECURE_NO_WARNINGS
#include<cstdlib>
#include<stdio.h>
#include<iostream>
#include<conio.h>
using namespace std;

struct tab {
	unsigned int key;
	int indeksMin;
	int indeksMax;
};

int parent(int i) {
	return i / 2;
}

int left(int i) {
	return (i * 2 + 1);
}

int right(int i) {
	return (i * 2 + 2);
}

void swapMin(tab *min, tab *max, int in1, int in2) {
	max[min[in1].indeksMax].indeksMin = in2;
	max[min[in2].indeksMax].indeksMin = in1;
	tab tmp = min[in1];
	min[in1] = min[in2];
	min[in2] = tmp;
}

void swapMax(tab *min, tab *max, int in1, int in2) {
	min[max[in1].indeksMin].indeksMax = in2;
	min[max[in2].indeksMin].indeksMax = in1;
	tab tmp = max[in1];
	max[in1] = max[in2];
	max[in2] = tmp;
}

void minHeapify(int i, tab *tabmin, int dl, tab *tabmax) {
	int l = left(i);
	int r = right(i);
	int min;
	if (l<dl && tabmin[l].key < tabmin[i].key) {
		min = l;
	}
	else min = i;
	if (r<dl && tabmin[r].key < tabmin[min].key) {
		min = r;
	}
	if (min != i) {
		swapMin(tabmin, tabmax, i, min);
		minHeapify(min, tabmin, dl, tabmax);
	}
}

void maxHeapify(int i, tab *tabmax, int dl, tab *tabmin) {
	int l = left(i);
	int r = right(i);
	int max;
	if (l<dl && tabmax[l].key > tabmax[i].key) {
		max = l;
	}
	else max = i;
	if (r<dl && tabmax[r].key > tabmax[max].key) {
		max = r;
	}
	if (max != i) {
		swapMax(tabmin, tabmax, i, max);
		maxHeapify(max, tabmax, dl, tabmin);
	}
}

void buildMinHeap(tab *tabmin, int dl, tab *tabmax) {
	for (int i = dl / 2 - 1; i >= 0; i--) {
		minHeapify(i, tabmin, dl, tabmax);
	}
}

void buildMaxHeap(tab *tabmax, int dl, tab *tabmin) {
	for (int i = dl / 2 - 1; i >= 0; i--) {
		maxHeapify(i, tabmax, dl, tabmin);
	}
}

void upHeapMin(tab *tabmin, int in) {
	if (in == 0) return;
	else {
		while (parent(in) >= 0 && tabmin[parent(in)].key > tabmin[in].key) {
			tab tmp = tabmin[parent(in)];
			tabmin[parent(in)] = tabmin[in];
			tabmin[in] = tmp;
			in /= 2;
		}
	}
}

void upHeapMax(tab *tabmax, int in) {
	if (in == 0) return;
	else {
		while (parent(in) >= 0 && tabmax[parent(in)].key < tabmax[in].key) {
			tab tmp = tabmax[parent(in)];
			tabmax[parent(in)] = tabmax[in];
			tabmax[in] = tmp;
			in /= 2;
	}
	}
}

void popMin(tab *tabmax, int dl, int in, tab *tabmin) {
	swapMax(tabmin, tabmax, in, dl);
	upHeapMin(tabmin, in);
	minHeapify(0, tabmin, dl-1, tabmax);
}

void popMax(tab *tabmax, int dl, int in, tab *tabmin) {
	swapMin(tabmin, tabmax, in, dl);
	upHeapMax(tabmax, in);
	maxHeapify(0, tabmax, dl-1, tabmin);
}

void popElemMin(tab *tabmin, int *dl, tab *tabmax) {
	(*dl)--;
	int i = tabmin[0].indeksMax;
	tab tmp = tabmin[0];
	tabmin[0] = tabmin[*dl];
	tabmin[*dl] = tmp;
	minHeapify(0, tabmin, *dl, tabmax);
	popMax(tabmax, *dl, i, tabmin);
}

void popElemMax(tab *tabmax, int *dl, tab *tabmin) {
	(*dl)--;
	int i = tabmax[0].indeksMin;
	tab tmp = tabmax[0];
	tabmax[0] = tabmax[*dl];
	tabmax[*dl] = tmp;
	maxHeapify(0, tabmax, *dl, tabmin);
	popMin(tabmax, *dl, i, tabmin);
}


void collatzMin(tab *tabmin, tab *tabmax) {
	if (tabmin[0].key % 2 == 0) {
		tabmin[0].key = tabmin[0].key / 2;
		tabmax[tabmin[0].indeksMax].key = tabmin[0].key;
	}
	else {
		tabmin[0].key = tabmin[0].key * 3 + 1;
		tabmax[tabmin[0].indeksMax].key = tabmin[0].key;
	}

}

void collatzMax(tab *tabmax, tab *tabmin) {
	if (tabmax[0].key % 2 == 0) {
		tabmax[0].key = tabmax[0].key / 2;
		tabmin[tabmax[0].indeksMin].key = tabmax[0].key;
	}
	else {
		tabmax[0].key = tabmax[0].key * 3 + 1;
		tabmin[tabmax[0].indeksMin].key = tabmax[0].key;
	}

}

int main() {
	int dlugosc, calkDl;
	scanf("%d", &dlugosc);
	tab *tabMin = (tab*)malloc(dlugosc * sizeof(tab));
	for (int i = 0; i < dlugosc; i++) {
		scanf("%u", &tabMin[i].key);
		tabMin[i].indeksMin = i;
		tabMin[i].indeksMax = i;
	}
	calkDl = dlugosc;
	tab *tabMax = (tab*)malloc(dlugosc * sizeof(tab));
	for (int i = 0; i < dlugosc; i++) {
		tabMax[i].key = tabMin[i].key;
		tabMax[i].indeksMax = i;
		tabMax[i].indeksMin = i;
	}
	buildMinHeap(tabMin, dlugosc, tabMax);
	buildMaxHeap(tabMax, dlugosc, tabMin);

	
	cout << endl;
	for (int i = 0; i < calkDl; i++) {
		cout << tabMin[i].key << " ";
	}
	cout << endl;
	for (int i = 0; i < calkDl; i++) {      //wyswietlanie kopcow
		cout << tabMin[i].indeksMax << " ";
	}
	cout << endl<<endl;
	for (int i = 0; i < calkDl; i++) {
		cout << tabMax[i].key << " ";
	}
	cout << endl;
	for (int i = 0; i < calkDl; i++) {      //wyswietlanie kopcow
		cout << tabMax[i].indeksMin << " ";
	}
	cout << endl;

	popElemMax(tabMax, &dlugosc, tabMin);

	cout << endl;
	for (int i = 0; i < calkDl; i++) {
		cout << tabMin[i].key << " ";
	}
	cout << endl;
	for (int i = 0; i < calkDl; i++) {      //wyswietlanie kopcow
		cout << tabMin[i].indeksMax << " ";
	}
	cout << endl << endl;
	for (int i = 0; i < calkDl; i++) {
		cout << tabMax[i].key << " ";
	}
	cout << endl;
	for (int i = 0; i < calkDl; i++) {      //wyswietlanie kopcow
		cout << tabMax[i].indeksMin << " ";
	}

	return 0;
}