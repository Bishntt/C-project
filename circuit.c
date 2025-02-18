#include <stdio.h>

#define MAX_BRANCHES 10
#define MAX_RESISTORS 10
#define MAX_SUB_BRANCHES 5

typedef struct {
    double resistances[MAX_RESISTORS]; // Resistors in this sub-branch
    int count;                         // Number of resistors in this sub-branch
} SubBranch;

typedef struct {
    SubBranch sub_branches[MAX_SUB_BRANCHES]; // Sub-branches in this branch
    int sub_count;                            // Number of sub-branches in this branch
} Branch;

// Function to calculate parallel resistance
double parallel_resistance(double r1, double r2) {
    return (r1 * r2) / (r1 + r2);
}

// Function to calculate equivalent resistance for a branch
double calculate_branch_resistance(Branch branch) {
    double total = 0;
    for (int i = 0; i < branch.sub_count; i++) {
        double sub_total = 0;
        for (int j = 0; j < branch.sub_branches[i].count; j++) {
            sub_total += branch.sub_branches[i].resistances[j]; // Series resistance in sub-branch
        }
        if (total == 0) {
            total = sub_total;
        } else {
            total = parallel_resistance(total, sub_total); // Parallel resistance between sub-branches
        }
    }
    return total;
}

// Function to draw the circuit
void draw_circuit(Branch branches[], int branch_count) {
    printf("\nCircuit Representation:\n");
    printf(" ----l| ----\n");
    for (int i = 0; i < branch_count; i++) {
        printf("|           |\n");
        for (int sb = 0; sb < branches[i].sub_count; sb++) {
            for (int j = 0; j < branches[i].sub_branches[sb].count; j++) {
                printf(" ----^^^---- ");
            }
            if (sb < branches[i].sub_count - 1) {
                printf(" || "); // Indicate parallel sub-branches
            }
        }
        printf("\n");
    }
    printf("|           |\n");
}

int main() {
    double voltage, resistance;
    char choice;
    int branch_count = 0;
    Branch branches[MAX_BRANCHES] = {0};

    printf("Enter the voltage source (in volts): ");
    scanf("%lf", &voltage);

    do {
        printf("\nAdding resistors to branch %d:\n", branch_count + 1);
        printf("Enter the resistance value (in ohms): ");
        scanf("%lf", &resistance);

        // Initialize the first sub-branch in the current branch
        branches[branch_count].sub_branches[0].resistances[0] = resistance;
        branches[branch_count].sub_branches[0].count = 1;
        branches[branch_count].sub_count = 1;

        draw_circuit(branches, branch_count + 1);

        char add_more;
        do {
            printf("\nDo you want to add another resistor to this branch? (Y/N): ");
            scanf(" %c", &add_more);

            if (add_more == 'Y' || add_more == 'y') {
                int series_parallel;
                printf("Add in series (1) or parallel (2) to the existing resistor(s)?: ");
                scanf("%d", &series_parallel);

                if (series_parallel == 1) { // Series
                    printf("Enter the resistance value (in ohms): ");
                    scanf("%lf", &resistance);
                    branches[branch_count].sub_branches[0].resistances[branches[branch_count].sub_branches[0].count++] = resistance;
                } else if (series_parallel == 2) { // Parallel
                    printf("Enter the resistance value (in ohms): ");
                    scanf("%lf", &resistance);
                    if (branches[branch_count].sub_count < MAX_SUB_BRANCHES) {
                        branches[branch_count].sub_branches[branches[branch_count].sub_count].resistances[0] = resistance;
                        branches[branch_count].sub_branches[branches[branch_count].sub_count].count = 1;
                        branches[branch_count].sub_count++;
                    } else {
                        printf("Maximum sub-branches reached in this branch!\n");
                    }
                }
                draw_circuit(branches, branch_count + 1);
            }
        } while ((add_more == 'Y' || add_more == 'y') && branches[branch_count].sub_count < MAX_SUB_BRANCHES);

        branch_count++;

        printf("\nDo you want to add another branch? (Y/N): ");
        scanf(" %c", &choice);

    } while ((choice == 'Y' || choice == 'y') && branch_count < MAX_BRANCHES);

    // Compute equivalent resistance
    double total_resistance = 0;
    for (int i = 0; i < branch_count; i++) {
        double branch_resistance = calculate_branch_resistance(branches[i]);
        if (total_resistance == 0) {
            total_resistance = branch_resistance;
        } else {
            total_resistance = parallel_resistance(total_resistance, branch_resistance);
        }
    }

    // Compute current
    double current = voltage / total_resistance;

    // Display results
    printf("\nEquivalent Resistance: %.2f ohms\n", total_resistance);
    printf("Total Circuit Current: %.2f A\n", current);

    return 0;
}