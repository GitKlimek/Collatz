#define _CRT_SECURE_NO_WARNINGS
#include<cstdlib>
#include<stdio.h>

#define RANGE 4294967295

struct tab {
	unsigned int key;
	int indeksMin;
	int indeksMax;
	int indeks;
};

int parent(int i) {
	return (i - 1) / 2;
}

int left(int i) {
	return (i * 2) + 1;
}

int right(int i) {
	return (i * 2) + 2;
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
	if (l > dl || r > dl) return;
	int min = i;
	if (l < dl && tabmin[l].key <= tabmin[i].key) {
		if (tabmin[l].key == tabmin[i].key) {
			if (tabmin[l].indeks < tabmin[i].indeks) {
				min = l;
			}
		}
		else min = l;
	}
	if (r < dl && tabmin[r].key <= tabmin[min].key) {
		if (tabmin[r].key == tabmin[min].key) {
			if (tabmin[r].indeks < tabmin[min].indeks) {
				min = r;
			}
		}
		else min = r;
	}
	if (min != i) {
		swapMin(tabmin, tabmax, i, min);
		minHeapify(min, tabmin, dl, tabmax);
	}
}

void maxHeapify(int i, tab *tabmax, int dl, tab *tabmin) {
	int l = left(i);
	int r = right(i);
	if (l > dl || r > dl) return;
	int max = i;
	if (l < dl && tabmax[l].key >= tabmax[i].key) {
		if (tabmax[l].key == tabmax[i].key) {
			if (tabmax[l].indeks < tabmax[i].indeks) max = l;
			else max = i;
		}
		else max = l;
	}
	if (r < dl && tabmax[r].key >= tabmax[max].key) {
		if (tabmax[r].key == tabmax[max].key) {
			if (tabmax[r].indeks < tabmax[max].indeks) {
				max = r;
			}
		}
		else max = r;
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

void upHeapMin(tab *tabmin, int *in, tab *tabmax) {
	if (*in == 0) return;
	else {
		while (parent(*in) >= 0 && tabmin[parent(*in)].key > tabmin[*in].key) {
			swapMin(tabmin, tabmax, *in, parent(*in));
			(*in) = parent(*in);
		}
	}
}

void upHeapMax(tab *tabmax, int *in, tab *tabmin) {
	if (*in == 0) return;
	else {
		while (parent(*in) >= 0 && tabmax[parent(*in)].key < tabmax[*in].key) {
			swapMax(tabmin, tabmax, *in, parent(*in));
			(*in) = parent(*in);
		}
	}
}

void popMin(tab *tabmax, int *dl, int *in, tab *tabmin) {
	if (dl <= 0) return;
	swapMin(tabmin, tabmax, *in, *dl - 1);
	(*dl)--;
	minHeapify(*in, tabmin, *dl, tabmax);
}

void popMax(tab *tabmax, int *dl, int *in, tab *tabmin) {
	if (dl <= 0) return;
	swapMax(tabmin, tabmax, *in, *dl - 1);
	(*dl)--;
	if (tabmax[parent(*in)].key < tabmax[*in].key) {
		upHeapMax(tabmax, in, tabmin);
	}
	maxHeapify(*in, tabmax, *dl, tabmin);
}

void popElemMin(tab *tabmin, int *dl, tab *tabmax) {
	if (*dl <= 0) return;
	int i = tabmin[0].indeksMax;
	swapMin(tabmin, tabmax, 0, *dl - 1);
	popMax(tabmax, dl, &i, tabmin);
	minHeapify(0, tabmin, *dl, tabmax);

}

void popElemMax(tab *tabmax, int *dl, tab *tabmin) {
	if (*dl <= 0) return;
	int i = tabmax[0].indeksMin;
	swapMax(tabmin, tabmax, 0, *dl - 1);
	popMin(tabmax, dl, &i, tabmin);
	maxHeapify(0, tabmax, *dl, tabmin);
}


void collatzMin(tab *tabmin, tab *tabmax, int *dl) {
	if (*dl == 0) return;
	int indekswmax = tabmin[0].indeksMax;
	if (tabmin[0].key % 2 == 0) {
		tabmin[0].key = tabmin[0].key / 2;
		tabmax[indekswmax].key = tabmin[0].key;
		if (tabmin[0].key == 1) popElemMin(tabmin, dl, tabmax);
		else {
			minHeapify(0, tabmin, *dl, tabmax);
			maxHeapify(indekswmax, tabmax, *dl, tabmin);
		}
	}
	else {
		unsigned int a = tabmin[0].key;
		tabmin[0].key = tabmin[0].key * 3 + 1;
		tabmax[indekswmax].key = tabmin[0].key;
		if (a >= (RANGE / 3)) {
			tabmin[0].key = NULL;
			tabmax[indekswmax].key = NULL;
			popElemMin(tabmin, dl, tabmax);
		}
		else {
			minHeapify(0, tabmin, *dl, tabmax);
			upHeapMax(tabmax, &indekswmax, tabmin);
			maxHeapify(indekswmax, tabmax, *dl, tabmin);
		}
	}
	while ((tabmin[0].key == 1 || tabmin[0].key == NULL) && *dl > 0) popElemMin(tabmin, dl, tabmax);
}

void collatzMax(tab *tabmax, tab *tabmin, int *dl) {
	if (*dl == 0) return;
	int indekswmin = tabmax[0].indeksMin;
	if (tabmax[0].key % 2 == 0) {
		tabmax[0].key = tabmax[0].key / 2;
		tabmin[indekswmin].key = tabmax[0].key;
		if (tabmax[0].key == 1) popElemMax(tabmax, dl, tabmin);
		else {
			maxHeapify(0, tabmax, *dl, tabmin);
			upHeapMin(tabmin, &indekswmin, tabmax);
			minHeapify(indekswmin, tabmin, *dl, tabmax);
		}
	}
	else {
		unsigned int a = tabmax[0].key;
		tabmax[0].key = tabmax[0].key * 3 + 1;
		tabmin[indekswmin].key = tabmax[0].key;
		if (a >= (RANGE / 3)) {
			tabmax[0].key = NULL;
			tabmin[indekswmin].key = NULL;
			popElemMax(tabmax, dl, tabmin);
		}
		else {
			maxHeapify(0, tabmax, *dl, tabmin);
			minHeapify(indekswmin, tabmin, *dl, tabmax);
		}
	}
	while ((tabmax[0].key == 1 || tabmax[0].key == NULL) && *dl > 0) popElemMax(tabmax, dl, tabmin);
}

int main() {
	int dlugosc, lkomend, k, calkDl;
	char c;
	scanf("%d", &dlugosc);
	tab *tabMin = (tab*)malloc(dlugosc * sizeof(tab));
	tab *tabMax = (tab*)malloc(dlugosc * sizeof(tab));
	for (int i = 0; i < dlugosc; i++) {
		scanf("%u", &tabMin[i].key);
		tabMin[i].indeksMin = i;
		tabMin[i].indeksMax = i;
		tabMin[i].indeks = i;
		tabMax[i].key = tabMin[i].key;
		tabMax[i].indeksMax = i;
		tabMax[i].indeksMin = i;
		tabMax[i].indeks = i;
	}
	unsigned int *wynik = (unsigned int*)malloc(dlugosc * sizeof(unsigned int));
	calkDl = dlugosc;
	buildMinHeap(tabMin, dlugosc, tabMax);
	buildMaxHeap(tabMax, dlugosc, tabMin);


	while (tabMin[0].key == 1 || tabMin[0].key == 0) popElemMin(tabMin, &dlugosc, tabMax);

	scanf("%d", &lkomend);
	for (int i = 0; i < lkomend; i++) {
		if (dlugosc == 0) break;
		scanf("%d ", &k);
		scanf("%c", &c);
		if (c == 's') {
			for (int j = 0; j < k; j++) {
				collatzMin(tabMin, tabMax, &dlugosc);
				if (dlugosc == 0) break;
			}
		}
		else if (c == 'l') {
			for (int j = 0; j < k; j++) {
				collatzMax(tabMax, tabMin, &dlugosc);
				if (dlugosc == 0) break;
			}
		}
	}

	for (int i = 0; i < calkDl; i++) {
		wynik[tabMin[i].indeks] = tabMin[i].key;
	}

	//wyswietlanie wyniku
	for (int i = 0; i < calkDl; i++) {
		if (wynik[i] == NULL) {
			printf("m ");
		}
		else {
			printf("%u ", wynik[i]);
		}
	}
	free(tabMax);
	free(tabMin);
	free(wynik);
	return 0;
}