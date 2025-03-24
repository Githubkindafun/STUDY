#include <bits/stdc++.h>

using namespace std;

// const int MAX_N = 1e6;

// void funprint(pair<int,int> mom_son[], int n) {
//     for(int i = 0; i < n; i++) {
//         cout << mom_son[i].first << " " << mom_son[i].second << endl;
//     }
// }

int bin_search(pair<int,int> mom_son[], int mom, int size) {
    int low = 0;
    int high = size - 1;
    int middle;
    
    while(low + 1 != high && low < high) {
        middle = (low + high) / 2;

        // if(mom_son[middle].first == mom) {
        //     return middle;
        // }
        if (mom_son[middle].first < mom) {
            low = middle;
        }
        else {
            high = middle;
        }
    }
    if (mom_son[low].first == mom) {return low;}
    else if (mom_son[high].first == mom) {return high;}
    else {
        return -1;
    }
    
}


int main() {
    ios_base::sync_with_stdio(false);
    cout.tie(0);
    cin.tie(0);

    int n, q; cin >> n >> q;
    n--;
    pair<int,int> mom_son[n];
    int pre[n+1];
    int post[n+1];

    int mother;
    for(int i = 0; i < n; i++) {
        cin >> mother;
        mom_son[i].first = mother; // info o mamie z inputu
        mom_son[i].second = i + 2; // info kto jest corka mamy z inputu
    }

    // funprint(mom_son, n);
    
    // w zadaniu mamy info o tym ze dane moga byc nie chronologicznie wiec przydalo by sie je posortowac
    sort(mom_son, mom_son + n);
    
    // funprint(mom_son, n);
    
    //dfs
    bool visited[n+1] = {}; // odwiedzone false
    stack<int> stack;
    stack.push(1); // krolowa jest zawsze nad wszystkimi
    int time = 0;
    
    // int parent;
    // int top;

    while(!stack.empty()) {
        int top = stack.top();
        // cout << top << "\n";
        

        if(!visited[top-1]) { //jezeli nie odwiedzony
            visited[top-1] = true;
            pre[top-1] = time++;

            int parent = bin_search(mom_son, top, n);
            // cout << "top: " << top << " parent: " << parent << endl; // szukamy na jakim indeksie jest top - rodzic
            

            while(mom_son[parent].first == top && parent <= n) {
                stack.push(mom_son[parent].second);
                // cout << "push " << mom_son[parent].second << endl;
                parent++; 
            }
        }
        else {
            post[top-1] = time++;
            stack.pop();
        }
    }

    // for(int i = 0; i <= n; i++) {
    //     cout << pre[i] << " " << post[i] << endl;
    // }

    int a, b;
    while(q--) { // sprawdzanie zapytań
        cin >> a >> b;  a--; b--;
        if(pre[a] < pre[b] && post[a] > post[b]) {
            // cout << "TAK\n";
            printf("TAK\n");
        } else {
            // cout << "NIE\n";
            printf("NIE\n");
        }
    }    
}
