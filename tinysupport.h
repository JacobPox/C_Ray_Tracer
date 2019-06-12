#ifndef TINYSUPPORT_H_
#define TINYSUPPORT_H_

int arrSub(const float arr1[], const float arr2[], float subArr[], int length);

int arrAdd(const float arr1[], const float arr2[], float addArr[], int length);

int arrScalarMult(const float arr1[], float scalar, float newArr[], int length);

int arrScalarMult(const float arr1[], float scalar, float newArr[], int length);

float dotProduct(const float arr1[], const float arr2[], int length);

int normalize(float arr[], int len);

int reflect(const float I[], const float N[], float updatedArr[], int length);

#endif