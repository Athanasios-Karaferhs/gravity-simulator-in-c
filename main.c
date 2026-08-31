#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define N 100        // πληθως σωματον
#define G 0.05       // βαρυτικη σταθερα
#define EPSILON 1e-2 // prevents divide-by-near-zerο
#define DT 0.001     // time per step Δτ

typedef struct
{
    // αναπαραστατση μιας θέσης r = (x, y, z), ένος διανυσματος ταχύτητας v = (vx, vy, vz), και μάζας m.
    double x, y, z;    //  θέση στο χώρο (διάνυσμα θέσης r)
    double vx, vy, vz; // ταχυτητα
    double mass;       // μαζα m
} Body;

void simulate_step(Body bodies[], int n);
void init_bodies(Body bodies[], int n);
void log_positions(FILE *f, Body bodies[], int n, int step);

int main()
{
    Body bodies[N];
    init_bodies(bodies, N);

    FILE *f = fopen("positions.csv", "w");
    fprintf(f, "step,body,x,y,z\n");

    int num_steps = 20000;
    for (int step = 0; step < num_steps; step++)
    {
        simulate_step(bodies, N);
        if (step % 10 == 0)
        { // log every 10th step, keeps file size reasonable
            log_positions(f, bodies, N, step);
        }
    }
    printf("done");
    fclose(f);
    return 0;
}

void simulate_step(Body bodies[], int n)
{
    double ax[N], ay[N], az[N]; // Temporary arrays to hold each bodys acceleration for this step
    for (int i = 0; i < n; i++)
    {
        double fx = 0, fy = 0, fz = 0;

        for (int j = 0; j < n; j++)
        {
            if (i == j) // sum up continue on i==j cause it doesnt pull it self
                continue;

            // διανυσμα διαφορας θεσης:
            double dx = bodies[j].x - bodies[i].x;
            double dy = bodies[j].y - bodies[i].y;
            double dz = bodies[j].z - bodies[i].z;

            // υπολογισμους μετρου (επι κανουνικιες συνθηκες ακολουθει τον κανονα |r_i]=sq_root(dx^2+dy^2+dz^2)) με προσθεση epsilon^2 ως softening αποτρεποντας το dist να γίνει σχεδον μηδεν οταν δυο σωματα βρεθουν πολυ κοντα
            double dist_sq = dx * dx + dy * dy + dz * dz + EPSILON * EPSILON;
            double dist = sqrt(dist_sq);

            // Νομος νευτονα (παγκωσμια ελξη), F=(G*m1*m2)/r^2
            double force = G * bodies[i].mass * bodies[j].mass / dist_sq;

            // μετρα δυναμης (με σωστο μεγεθος και κατευθυνση)
            // μετρα δυναμης (με σωστο μεγεθος και κατευθυνση)
            /*με το += αθροιζουμε τη συνεισφορα από ολα τα αλλα σωματα j F_i =Σ(G*m1*m2)/ abs(r ij)^2 *rij
                                                                            j!=i
            */
            fx += force * dx / dist;
            fy += force * dy / dist;
            fz += force * dz / dist;
        }

        // a = F/m
        ax[i] = fx / bodies[i].mass;
        ay[i] = fy / bodies[i].mass;
        az[i] = fz / bodies[i].mass;
    }

    // ολοκληρωματα κινησης  uold=unew+a*Δτ και xold=xnew+a*Δτ
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

// thanks claude!
void init_bodies(Body bodies[], int n)
{
    srand(time(NULL));

    bodies[0].x = 0;
    bodies[0].y = 0;
    bodies[0].z = 0;
    bodies[0].vx = 0;
    bodies[0].vy = 0;
    bodies[0].vz = 0;
    bodies[0].mass = 500; // μαζα του ηλιου

    // remaining bodies: scattered randomly around it
    for (int i = 1; i < n; i++)
    {
        bodies[i].x = ((double)rand() / RAND_MAX - 0.5) * 100.0; // range -50..50
        bodies[i].y = ((double)rand() / RAND_MAX - 0.5) * 100.0;
        bodies[i].z = ((double)rand() / RAND_MAX - 0.5) * 100.0;

        bodies[i].vx = ((double)rand() / RAND_MAX - 0.5) * 0.1;
        bodies[i].vy = ((double)rand() / RAND_MAX - 0.5) * 0.1;
        bodies[i].vz = ((double)rand() / RAND_MAX - 0.5) * 0.1;

        bodies[i].mass = 1.0;
    }
}

void log_positions(FILE *f, Body bodies[], int n, int step)
{
    for (int i = 0; i < n; i++)
    {
        fprintf(f, "%d,%d,%f,%f,%f\n", step, i, bodies[i].x, bodies[i].y, bodies[i].z);
    }
}