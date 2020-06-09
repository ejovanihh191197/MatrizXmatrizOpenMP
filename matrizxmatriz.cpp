#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
#include<omp.h>
using namespace std;

void matrizxmatriz(double** vector, double** matriz, double** resultado,int a, int b, int c);

int getDimension(const char* name,int dim){
    int rows=0;
    int cols=0;
    FILE *archivo;
 	char caracteres[1000];
    char * token;

 	archivo = fopen(name,"r");
 	
 	while (feof(archivo) == 0){
 		fgets(caracteres,100,archivo);
 		token = strtok(caracteres, ",");
        cols=0;
        while( token != NULL ) {
            token = strtok(NULL, ",");
            cols++;
        }
        rows++;
    }

    fclose(archivo);

    if(dim==0){
        return rows;
    }else{
        return cols;
    }
}

double** getMatriz(const char* name){
    int rows=getDimension(name,0);
    int cols=getDimension(name,1);
    int i,j;
    FILE *archivo;
 	double** mat;
 	char caracteres[1000];
    char * token;

 	archivo = fopen(name,"r");
 	
    mat = new double*[rows];
    for(int x=0; x<rows; x++){
        mat[x] = new double[cols];
    }
    i=0;
 	while (feof(archivo) == 0){
 		fgets(caracteres,100,archivo);
 		token = strtok(caracteres, ",");
        j=0;
        while( token != NULL ) {
            mat[i][j] = atof(token);
            //printf("%s ", token );
            token = strtok(NULL, ",");
            j++;
        }    
        i++;        
    }
    fclose(archivo);
    return mat;
}

int main(){
    double **M = getMatriz("matriz.txt");
    double **M2 = getMatriz("matriz2.txt");
    int rowsM1=getDimension("matriz.txt",0);
    int colsM1=getDimension("matriz.txt",1);
    int rowsM2=getDimension("matriz2.txt",0);
    int colsM2=getDimension("matriz2.txt",1);
    double** resul;

    resul = new double*[rowsM1];
    for (int i = 0; i < rowsM1; i++)
    {
        resul[i] = new double[colsM2];
    }
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        matrizxmatriz(M2,M,resul,rowsM1,colsM1,colsM2);
    }
    
    cout<<"\n";
    for (int x = 0; x < rowsM1; x++){
        for (int y = 0; y < colsM2; y++){
            cout<<resul[x][y]<<"\t";
        }
        cout<<"\n";
    }
    
    return 0;
}

void matrizxmatriz(double** vector, double** matriz, double** resultado,int a, int b, int c){
    int suma =0;
    #pragma omp for
    for (int i = 0; i < a; i++){
        for (int j = 0; j < c; j++){
            suma=0;
            for (int k = 0; k < b; k++){
                suma = suma + vector[i][k] * matriz[k][j];
            }
            resultado[i][j] = suma;
        }
    }  
}