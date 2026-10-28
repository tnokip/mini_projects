#include <stdio.h>
#include <string.h>
#include "cube.h"
#include "parser.h"
#include "solver.h"

int main(int argc, char **argv)
{
    int stats = (argc > 1 && strcmp(argv[1], "--stats") == 0);
    const char *path = (argc > 1 && !stats) ? argv[1] : "state.csv";

    Cube cube = cube_solved();
    if (!stats)
    {
        char err[128];
        if (parse_cube_file(path, &cube, err, sizeof err))
        {
            printf("Error: %s.\n", err);
            return 1;
        }
    }

    solver_build_table();
    if (stats) 
    { 
        solver_print_stats(); return 0; 
    }

    Cube original = cube;
    int moves[MAX_SOLUTION];
    int n = solver_solve(&cube, moves);
    if (n < 0) 
    { 
        printf("Error: no solution found.\n");
        return 1;
    }
    if (n == 0) 
    {
        printf("The cube is already solved.\n"); 
        return 0; 
    }

    printf("Optimal solution (%d moves):", n);
    for (int i = 0; i < n; i++) printf(" %s", MOVE_STR[moves[i]]);
    printf("\n");

    for (int i = 0; i < n; i++) cube_apply_move(&original, moves[i]);
    printf("Verified: %s\n", cube_is_solved(&original) ? "solved" : "FAILED");
    return 0;
}
