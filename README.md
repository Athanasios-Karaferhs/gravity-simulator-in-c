# N-Body Gravity Simulator (C)

A naive O(n²) N-body gravitational simulator written in C, simulating `N` bodies orbiting a central mass under Newtonian gravity. This is the baseline (unoptimized) implementation — future work will focus on performance optimization (cache-friendly memory layout, multithreading, SIMD) with benchmarked before/after results.

## How it works

Each body has a position, velocity, and mass. At every timestep:

1. **Force calculation** — for every body, sum the gravitational pull from every other body (`F = G*m1*m2/r²`), with a softening factor (`EPSILON`) to avoid divide-by-near-zero when two bodies get very close.
2. **Integration** — convert force to acceleration (`a = F/m`), then update velocity and position using semi-implicit (symplectic) Euler integration.

Positions are logged to `positions.csv` every 10 steps for visualization.

## Build & run

```bash
gcc -g main.c -o main -lm
./main
python3 plot.py   # requires pandas, matplotlib — generates orbits.png
```

## Current baseline parameters

```c
#define N 100          // number of bodies
#define G 0.05         // gravitational constant (simulation units, not SI)
#define EPSILON 1e-2   // softening factor
#define DT 0.001        // timestep
```

One body acts as a central "sun" (mass 500, stationary at the origin); the remaining bodies start scattered randomly around it with small random initial velocities.

Note: these constants are tuned for visible, interesting dynamics in simulation units — they are not meant to represent real physical (SI) values.

## Known behavior

Most bodies orbit near the central mass as expected. A small number of bodies occasionally undergo close encounters with the sun and get ejected at high velocity — this is a genuine limitation of fixed-timestep Euler integration near strong gravitational sources (the timestep isn't small enough to accurately resolve very close, fast interactions), not a bug in the force/integration logic itself. This is a well-documented characteristic of naive fixed-timestep N-body integrators.

## Tech stack

C, standard library (`math.h`, `stdio.h`, `stdlib.h`, `time.h`). Visualization via a small Python/matplotlib script (not part of the core simulator).
