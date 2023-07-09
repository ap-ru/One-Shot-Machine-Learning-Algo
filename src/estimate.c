#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freeMatrix(double** matrix, int row){
    for(int i = 0; i < row; i++){
        free(matrix[i]);
    }
    free(matrix);
}

void printMatrix(double** matrix, int row, int col){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            printf("%.0f", matrix[i][j]);
        }
        printf("\n");
    }
    return;
}

void transpose(double** matrix, double** final, int row, int col){
    int temp;
    temp = row;
    row = col;
    col = temp;
    //compute matrix
    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++) {
            final[i][j] = matrix[j][i];
        }
    }
}

double** inverse(double** M, double** N, int n) {   
    double f;
    for(int p = 0; p < n; p++){
        f = M[p][p];
        for(int j = 0; j < n; j++){
            M[p][j] = M[p][j]/f;
            N[p][j] = N[p][j]/f;
        }
        for(int i = p + 1; i < n; i++){
            f = M[i][p];

            for(int j = 0; j < n; j++){
                M[i][j] = M[i][j] - (M[p][j]*f);
                N[i][j] = N[i][j] - (N[p][j]*f);
            }
            
        }
    }
    for(int p = n - 1; p >= 0; p--){
        for(int i = p - 1; i >= 0; i--){
            f = M[i][p];
            for(int j = 0; j < n; j++){
                M[i][j] = M[i][j] - (M[p][j]*f);
                N[i][j] = N[i][j] - (N[p][j]*f);
            }
            
        }
    }
    return N;
} 


double** multiply(double** a, double** b, int row, int col, int bcol) {
    
    double **final = malloc(row*sizeof(double*));
    for(int i = 0; i < row; i++){
        final[i] = malloc((col)*sizeof(double));
    }
    for(int i = 0; i < row; i++){
        for(int j = 0; j < bcol; j++){
            final[i][j] = 0;
        }
    }
    for(int i = 0; i < row; i++){
        for(int j = 0; j < bcol; j++){
            for(int k = 0; k < col; k++){
                final[i][j] += (a[i][k] * b[k][j]);
            }
        }
    }
    return final;
}

int main(int argc, char **argv){
    //printf("\n0\n");
    FILE* train = fopen(argv[1], "r");
    FILE* data = fopen(argv[2], "r");
    //printf("\nm\n");
    char t[20];
    char d[20];
    //printf("\n1\n");
    fscanf(train, "%s", t);
    fscanf(data, "%s", d);
    //printf("\nn\n");
    //printf("\n1\n");
    //finds houses and attributes of train
    int trainHouses;
    int trainAttributes;
    fscanf(train, "%d", &trainAttributes);
    fscanf(train, "%d", &trainHouses);
    double **attributeMatrix = malloc(trainHouses*sizeof(double*));
    for(int i = 0; i < trainHouses; i++){
        attributeMatrix[i] = malloc((trainAttributes+1)*sizeof(double));
    }
    double **priceMatrix = malloc(trainHouses*sizeof(double));
    for(int i = 0; i < trainHouses; i++){
        priceMatrix[i] = malloc(sizeof(double));
    }
    //printf("\n2\n");
    //fill matrices w/info
    double tempInput;
    for(int i = 0; i < trainHouses; i++){
        attributeMatrix[i][0] = 1;
    }
    for (int i = 0; i < trainHouses; i++){
        for (int j = 0; j < trainAttributes+1; j++){
            fscanf(train, "%lf", &tempInput);
            if(j == trainAttributes){
                priceMatrix[i][0] = tempInput;
            } else {
                attributeMatrix[i][j+1] = tempInput;
            }
        }
    }
    //printf("\nog\n");
    //printMatrix(attributeMatrix, trainHouses, trainAttributes+1);
    //printf("\n");
    //printMatrix(priceMatrix, trainHouses, 1);
    //make identity matrix
    double **identityMatrix = malloc((trainAttributes+1)*sizeof(double*));
    for(int i = 0; i < trainAttributes+1; i++){
        identityMatrix[i] = malloc((trainAttributes+1)*sizeof(double));
    }
    for(int i = 0; i < trainAttributes+1; i++){
        for(int j = 0; j < trainAttributes+1; j++){
            if(i == j){
                identityMatrix[i][j] = 1;
            }
            else{
                identityMatrix[i][j] = 0;
            }
        }
    }
    //printf("\nidentity\n");
    //printMatrix(identityMatrix, trainAttributes+1, trainAttributes+1);
    //printf("\ntranspose\n");
    //transpose (5*10)
    double** tattributes = malloc((trainAttributes+1)*sizeof(double*));
    for(int i = 0; i < trainAttributes+1; i++){
        tattributes[i] = malloc((trainHouses)*sizeof(double));
    }
    transpose(attributeMatrix, tattributes, trainHouses, trainAttributes+1);
    //printMatrix(tattributes, trainAttributes+1, trainHouses);
    //printf("\ntranspose*attribute\n");
    //transpose*attribute (5*5)
    double** mattributes = multiply(tattributes, attributeMatrix, trainAttributes+1, trainHouses, trainAttributes+1);
    //printMatrix(mattributes, trainAttributes+1, trainAttributes+1);
    //printf("\ntranspose*prices\n");
    //transpose*prices (5*1)
    double** mattributes2 = multiply(tattributes, priceMatrix, trainAttributes+1, trainHouses, 1);
    //printMatrix(mattributes2, trainAttributes+1, 1);
    //printf("\ninverse attribute product\n");
    //inverse attribute product (5*5)
    double** iattributes = inverse(mattributes, identityMatrix, trainAttributes+1);
    //printMatrix(iattributes, trainAttributes+1, trainAttributes+1);
    //printf("\nweights\n");
    //find weights: inverse*product2 (5*1)
    double** weight = multiply(iattributes, mattributes2, trainAttributes+1, trainAttributes+1, 1);
    //printMatrix(weight, trainAttributes+1, 1);
    //printf("\n");
    int rows;
    int cols;
    fscanf(data, "%d", &cols);
    fscanf(data, "%d", &rows);
    double **dataMatrix = malloc(rows*sizeof(double*));
    for(int i = 0; i < rows; i++){
        dataMatrix[i] = malloc((cols+1)*sizeof(double));
    }
    double temp;
    for(int i = 0; i < rows; i++){
        dataMatrix[i][0] = 1;
    }
    for (int i = 0; i < rows; i++){
        for (int j = 1; j < cols+1; j++){
            fscanf(data, "%lf", &temp);

            dataMatrix[i][j] = temp;
        }
    }
    //printf("data\n");
    //printMatrix(dataMatrix, rows, cols+1);
    double** final = multiply(dataMatrix, weight, rows, cols+1, 1);
    //printf("\nfinal\n");
    printMatrix(final, rows, 1);
    freeMatrix(attributeMatrix, trainHouses);
    freeMatrix(priceMatrix, trainHouses);
    freeMatrix(identityMatrix, trainAttributes+1);
    freeMatrix(tattributes, trainAttributes+1);
    freeMatrix(mattributes, trainAttributes+1);
    //freeMatrix(mattributes2, trainAttributes+1);
    for(int i = 0; i < trainAttributes+1; i++){
        free(mattributes2[i]);
    }
    free(mattributes2);
    //freeMatrix(iattributes, trainAttributes+1);
    freeMatrix(weight, trainAttributes+1);
    freeMatrix(dataMatrix, rows);
    freeMatrix(final, rows);

    return EXIT_SUCCESS;
}