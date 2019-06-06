#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

typedef struct {
    float center[3];
    float radius;

} Sphere;

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

    float sumSqr;
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

bool ray_intersect(const float origin[], const float dir[], float t0, Sphere s) {
    /*
    Ray-Sphere Intersection
    
    Vectors:
        origin (the zero vector)
        dir (direction vector)
        L (vector from origin to center of sphere)
    Scalars:
        tca
        d2
        thc
        t0
        t1    
    */
    float L[3] = {0,0,0}; //The zero vector
    arrSub(s.center, origin, L, 3); //L is now the vector from origin to the sphere's center

    float tca = dotProduct(L, dir, 3); //Projection of L onto dir
    float d2 = dotProduct(L, L, 3) - tca*tca;

    if (d2 > s.radius * s.radius) return false; //There is no intersection, so return false.

    float thc = sqrtf((s.radius*s.radius - d2));
    t0 = tca - thc;
    float t1 = tca + thc;
    if (t0 < 0) {
        t0 = t1;
    }
    if (t0 < 0) return false;

    return true;
}

int cast_ray(const float origin[], const float dir[], const Sphere s, unsigned char colorArr[]) {
    float sphere_dist = INT_MAX;
    if (!ray_intersect(origin, dir, sphere_dist, s)) {
        //background
        colorArr[0] = 250; //red
        colorArr[1] = 50; //green
        colorArr[2] = 50; //blue
    } else {
        //light up pixel
        colorArr[0] = 10;
        colorArr[1] = 125;
        colorArr[2] = 25; 
    }

    return 0;
}

int render(const Sphere s) {
    /*
    Creates image in a new color each step.
    */
    const int width = 1024;
    const int height = 768;

    FILE *fp = fopen("spheres.ppm", "wb"); // Write in binary mode
    (void) fprintf(fp, "P6\n%d %d\n255\n", width, height);

    float fov = 3.1415926535/2.; // Field of View

    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {

            float x = (2*(i+.5)/(float)width - 1)*tan(fov/2.)*width/(float)height;
            float y = -(2*(j+.5)/(float)height - 1)*tan(fov/2.);

            float dir[] = {x,y,-1};
            normalize(dir, 3);

            static unsigned char color[3];
            const float origin[] = {0,0,0};
            cast_ray(origin, dir, s, color);
            (void) fwrite(color, 1, 3, fp);
        }
    }
    (void) fclose(fp);
    return 0;
}

int main(void) {
    Sphere s = {{-3,0,-16}, 2};

    render(s);
    printf("Run success!\n");
    return 0;
}