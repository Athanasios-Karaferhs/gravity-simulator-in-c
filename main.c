#include <stdio.h>
#include <math.h>
#define N 100
#define G 6.674e-11
#define EPSILON 1e-3 // prevents divide-by-near-zero,thanks claude
#define DT 0.01

typedef struct
{
    double x, y, z;
    double vx, vy, vz;
    double mass;
} Body;

void simulate_step(Body bodies[], int n);

int main()
{
    Body bodies[N];

    for (int i = 0; i < N; i++)
    {
        simulate_step(bodies, N);
        bodies[i].x = 0;
        bodies[i].y = 0;
        bodies[i].z = 0;
        bodies[i].vx = 0;
        bodies[i].vy = 0;
        bodies[i].vz = 0;
        bodies[i].mass = 1.0;
    }
    return 0;
}

void simulate_step(Body bodies[], int n)
{
    double ax[N], ay[N], az[N];
    for (int i = 0; i < n; i++)
    {
        double fx = 0, fy = 0, fz = 0;

        for (int j = 0; j < n; j++)
        {
            if (i == j)
                continue;
            double dx = bodies[j].x - bodies[i].x;
            double dy = bodies[j].y - bodies[i].y;
            double dz = bodies[j].z - bodies[i].z;

            double dist_sq = dx * dx + dy * dy + dz * dz + EPSILON * EPSILON;
            double dist = sqrt(dist_sq);
            double force = G * bodies[i].mass * bodies[j].mass / dist_sq;

            // accumulate force components (direction * magnitude)
            fx += force * dx / dist;
            fy += force * dy / dist;
            fz += force * dz / dist;
        }

        // a = F/m
        ax[i] = fx / bodies[i].mass;
        ay[i] = fy / bodies[i].mass;
        az[i] = fz / bodies[i].mass;
    }

    // velocity, then position, using the acceleration
    for (int i = 0; i < n; i++)
    {
        bodies[i].vx += ax[i] * DT;
        bodies[i].vy += ay[i] * DT;
        bodies[i].vz += az[i] * DT;

        bodies[i].x += bodies[i].vx * DT;
        bodies[i].y += bodies[i].vy * DT;
        bodies[i].z += bodies[i].vz * DT;
    }
}