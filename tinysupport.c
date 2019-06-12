#include <math.h>

int arrSub(const float arr1[], const float arr2[], float subArr[], int length) {
    /*
    Requires 3 equally sized arrays (denoted as length),
    arr1 - arr2 will result in the third array subArr
    */
    for (int i = 0; i < length; i++) {
        subArr[i] = arr1[i] - arr2[i];
    }
    return 0;
}

int arrAdd(const float arr1[], const float arr2[], float addArr[], int length) {
    /*
    Requires 3 equally sized arrays (denoted as length),
    arr1 + arr2 will result in the third array subArr
    */
    for (int i = 0; i < length; i++) {
        addArr[i] = arr1[i] + arr2[i];
    }
    return 0;
}

int arrScalarMult(const float arr1[], float scalar, float newArr[], int length) {
    /*
    Requires 3 equally sized arrays (denoted as length),
    arr1 - arr2 will result in the third array subArr
    */
    for (int i = 0; i < length; i++) {
        newArr[i] = arr1[i] * scalar;
    }
    return 0;
}

float dotProduct(const float arr1[], const float arr2[], int length) {
    /*
    Returns the dot product of two equal sized arrays 
    (treated as vectors)
    a (dot) b = a1b1 + a2b2 + ... anbn
    */
    float result = 0;

    for (int i = 0; i < length; i++) {
        result += arr1[i] * arr2[i];
    }

    return result;
}

int normalize(float arr[], int len) {
    //Normalize a vector (array)

    float sumSqr = 0;
    float norm;

    for (int i = 0; i < len; i++) {
        sumSqr += arr[i] * arr[i];
    }

    norm = sqrt(sumSqr);

    for (int i = 0; i < len; i++) {
        arr[i] = arr[i] / norm;
    }

    return 0;
}

int reflect(const float I[], const float N[], float updatedArr[], int length) {
    float NI = dotProduct(N, I, length);
    float tempArr[length];

    arrScalarMult(N, NI * 2.f, tempArr, length);
    
    arrSub(I, tempArr, updatedArr, length);

    return 0;
    
}