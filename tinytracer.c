#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    float center[3];
    float radius;

} Sphere;

int arrSub(const float arr1[], const float arr2[], float addedArr[], int length) {
    //Subtracts components of two arrays (treating them as vectors)
    for (int i = 0; i < length; i++) {
        addedArr[i] = arr1[i] - arr2[i];
    }
    return 0;
}

int dotProduct(const float arr1[], const float arr2[], int length) {
    //Returns the dot product
    float result = 0;

    for (int i = 0; i < length; i++) {
        result += arr1[i] * arr2[i];
    }
    return result;
}

bool ray_intersect(const float origin[], const float dir[], float * t0, Sphere s) {
    /*
    Ray-Sphere Intersection
    */
    float L[3] = {0,0,0}; //line
    arrSub(s.center, origin, L, 3); //L now is updated

    float tca = dotProduct(L, dir, 3);
    float d2 = dotProduct(L, L, 3) - tca*tca;

    if (d2 > s.radius * s.radius) return false;

    float thc = sqrtf((s.radius*s.radius - d2));
    * t0 = tca - thc;
    float t1 = tca + thc;
    if (* t0 < 0) {
        * t0 = t1;
    }

    if (* t0 < 0) return false;

    return true;
}

int cast_ray(const float origin[], const float dir[], const Sphere s, unsigned char colorArr[]) {
    float sphere_dist = 200000;
    float * t0 = &sphere_dist;
    if (!ray_intersect(origin, dir, t0, s)) {
        colorArr[0], colorArr[1], colorArr[2] = .2, .7, .8; //background color
    } else {
        colorArr[0], colorArr[1], colorArr[2] = .4,.4,.3;
    }

    return 0;
}

int render(const Sphere s) {
    /*
    Creates image in a new color each step.
    */
    const int width = 1024;
    const int height = 768;

    FILE *fp = fopen("first.ppm", "wb"); // Write in binary mode
    (void) fprintf(fp, "P6\n%d %d\n255\n", width, height);

    float fov = 100; // Field of View

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {

            float x = (2*(i+.5)/(float)width - 1)*tan(fov/2.)*width/(float)height;
            float y = -(2*(j+.5)/(float)height - 1)*tan(fov/2.);

            float dir[] = {x,y,-1};

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
    Sphere s = {{1,1,1}, 1};

    render(s);
    printf("Run success!\n");
    return 0;
}