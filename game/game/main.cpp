#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	int n;
	cin >> n;
	vector<int> g(n + 1);
	g[0] = 0;
	g[1] = 2;
	g[2] = 3;
	for (int position = 3; position <= n; ++position) {
		vector<int> entries;
		entries.push_back(g[position - 3]);
		for (int i = 3; i < position; ++i) {
			entries.push_back(g[i - 3] ^ g[position - i]);
		}
		sort(entries.begin(), entries.end());
		int currentEntry = 0;
		if (entries[currentEntry] > 0) {
			g[position] = 0;
		} else {
			while ((currentEntry + 1 < entries.size()) && (entries[currentEntry] + 1 == entries[currentEntry + 1])) {
				++currentEntry;
			}
			g[position] = entries[currentEntry] + 1;
		}
 	}

	if (g[n] == 0) {
		cout << '2';
	} else {
		cout << '1';
	}

	return 0;
}