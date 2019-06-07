#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

typedef struct {
    float diffuse_color[3];

}  Material;

typedef struct {
    float center[3];
    float radius;
    Material material;

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

bool scene_intersect(const float origin[], const float dir[], const Sphere s[], int len, float hit[], float N[], Material * ptr_m) {
    float sphere_dist = INT_MAX;

    for (size_t i=0; i < len; i++) {
        float dist_i;
        if (ray_intersect(origin, dir, dist_i, s[i]) && dist_i < sphere_dist) {
            sphere_dist = dist_i;

            float dirDist[3];
            arrScalarMult(dir, dist_i, dirDist, 3);
            arrAdd(origin, dirDist, hit, 3);

            float hitMinusCenter[3];
            arrSub(hit, s[i].center, hitMinusCenter, 3);
            normalize(hitMinusCenter, 3);

            N = hitMinusCenter;
            * ptr_m = s[i].material;
        }
    }
    return sphere_dist<1000;
}

int cast_ray(const float origin[], const float dir[], const Sphere s[], unsigned char colorArr[]) {
    float point[3], N[3];
    Material m;
    Material * ptr_m = &m;

    if (!scene_intersect(origin, dir, s, 3, point, N, ptr_m)) {
        //background
        colorArr[0] = 250; //red
        colorArr[1] = 250; //green
        colorArr[2] = 250; //blue
    } else {
        //light up pixel
        colorArr[0] = m.diffuse_color[0];
        colorArr[1] = m.diffuse_color[1];
        colorArr[2] = m.diffuse_color[2]; 

        printf("Colors: %i, %i, %i\n", colorArr[0], colorArr[1], colorArr[2]);
    }

    

    return 0;
}

int render(const Sphere s[]) {
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

            unsigned char color[3];
            const float origin[] = {0,0,0};
            cast_ray(origin, dir, s, color);
            (void) fwrite(color, 1, 3, fp);
        }
    }
    (void) fclose(fp);
    return 0;
}

int main(void) {
    Material red = {255,0,0};
    Material pink = {150,10,150};
    Material gold = {255, 195, 0};

    //Populate with spheres
    Sphere s[3];
    Sphere originalS = {{-3,0,-16},2,gold};
    Sphere bigS = {{-1.0, -1.5, -12}, 3, red};
    Sphere anotherS = {{7,5,-18},2,pink};

    s[0] = originalS;
    s[1] = bigS;
    s[2] = anotherS;

    render(s);
    printf("Run success!\n");
    return 0;
}