#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int CROSSOVER_POINT = 0;

vector<vector<int>> standard(vector<vector<int>>& A, vector<vector<int>>& B, int d) {
    vector<vector<int>> output (d, vector<int> (d, 0));
    for (int i = 0; i < d; i++) {
        for (int k = 0; k < d; k++) {
            for (int j = 0; j < d; j++) {
                output[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return output;
}

vector<vector<int>> add(vector<vector<int>>& M1, vector<vector<int>>& M2, int d, bool isSub = false) {
    vector<vector<int>> result (d, vector<int> (d, 0));
    for (int i = 0; i < d; i++) {
        for(int j = 0; j < d; j++) {
            result[i][j] = isSub ? M1[i][j] - M2[i][j] : M1[i][j] + M2[i][j];
        }
    }
    return result;
}

vector<vector<int>> sub(vector<vector<int>>& M1, vector<vector<int>>& M2, int d) {
    return add(M1, M2, d, true);
}

vector<vector<int>> pad(vector<vector<int>>& A, int d) {
    for (int i = 0; i < d; i++) {
        A[i].push_back(0);
    }
    A.push_back(vector<int> (d + 1, 0));
    return A;
}

vector<vector<int>> unpad(vector<vector<int>>& A, int desired_d) {
    A.pop_back();
    for (int i = 0; i < desired_d; i++) {
        A[i].pop_back();
    }
    return A;
}


vector<vector<int>> strassen(vector<vector<int>>& M1, vector<vector<int>>& M2, int d) {

    if (d < CROSSOVER_POINT) return standard(M1, M2, d);

    int halfd = (d + 1) / 2;    // ceiling of d/2 (if d = 10, we have halfd = 5, if d = 11, we have halfd = 6)
    int fulld = halfd * 2;      // smallest even number geq d
    vector<vector<int>> output (fulld, vector<int> (fulld, 0));

    if (d % 2 != 0) {
        M1 = pad(M1, d);
        M2 = pad(M2, d);
    }

    // A B
    // C D

    // E F
    // G H

    vector<vector<int>> A (halfd, vector<int> (halfd, 0));
    vector<vector<int>> B (halfd, vector<int> (halfd, 0));
    vector<vector<int>> C (halfd, vector<int> (halfd, 0));
    vector<vector<int>> D (halfd, vector<int> (halfd, 0));
    vector<vector<int>> E (halfd, vector<int> (halfd, 0));
    vector<vector<int>> F (halfd, vector<int> (halfd, 0));
    vector<vector<int>> G (halfd, vector<int> (halfd, 0));
    vector<vector<int>> H (halfd, vector<int> (halfd, 0));

    for (int i = 0; i < halfd; i++) {
        for (int j = 0; j < halfd; j++) {
            A[i][j] = M1[i][j];
            B[i][j] = M1[i][halfd + j];
            C[i][j] = M1[halfd + i][j];
            D[i][j] = M1[halfd + i][halfd + j];
            E[i][j] = M2[i][j];
            F[i][j] = M2[i][halfd + j];
            G[i][j] = M2[halfd + i][j];
            H[i][j] = M2[halfd + i][halfd + j];
        }
    }

    // P1 = A(F-H)
    vector<vector<int>> P1 = strassen(A, sub(F, H, halfd));
    // P2 = (A+B)H
    vector<vector<int>> P2 = strassen(add(A, B, halfd), H);
    // P3 = (C+D)E
    vector<vector<int>> P3 = strassen(add(C, D, halfd), E);
    // P4 = D(G-E)
    vector<vector<int>> P4 = strassen(D, sub(G, E, halfd));
    // P5 = (A+D)(E+H)
    vector<vector<int>> P5 = strassen(add(A, D, halfd), add(E, H, halfd));
    // P6 = (B-D)(G+H)
    vector<vector<int>> P6 = strassen(sub(B, D, halfd), add(G, H, halfd));
    // P7 = (C-A)(E+F)
    vector<vector<int>> P7 = strassen(sub(C, A, halfd), add(E, F, halfd));

    for(int i = 0; i < halfd; i++){
        for(int j = 0; j < halfd; j++){
            // AE + BG = -P2 + P4 + P5 + P6
            output[i][j] = P4[i][j] + P5[i][j] + P6[i][j] - P2[i][j];
            // AF + BH = P1 + P2
            output[i][halfd + j] = P1[i][j] + P2[i][j];
            // CE + DG = P3 + P4
            output[halfd + i][j] = P3[i][j] + P4[i][j];
            // CF + DH = P1 - P3 + P5 + P7
            output[halfd + i][halfd + j] = P1[i][j] - P3[i][j] + P5[i][j] + P7[i][j];
        }
    }

    if (d % 2 != 0) output = unpad(output);
    return output;
}

int triangle(float probability) {
    vector<vector<int>> randomgraph (1024, vector<int> (1024, 0));
    srand (time(NULL));
    for (int i = 0; i < 1024; i++) {
        for (int j = 0; j < i; j++) {
                randomgraph[i][j] = ((double) rand() / RAND_MAX) < probability ? 1 : 0;
                randomgraph[i][1024-j] = randomgraph[i][j];
        }
    }
    vector<vector<int>> cubed = strassen(strassen(randomgraph, randomgraph), randomgraph);
    int sum = 0;
    for (int i = 0; i < 1024; i++) {
        sum += cubed[i][i];
    }
    return sum / 6;
}

int main(int argc, char** argv) {
    int flag = atoi(argv[0]);
    int d = atoi(argv[1]);
}