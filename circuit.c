#include <stdio.h>

#define MAX_BRANCHES 10
#define MAX_RESISTORS 10
#define MAX_SUB_BRANCHES 5

typedef struct {
    double resistances[MAX_RESISTORS];
    int count;
} SubBranch;

typedef struct {
    SubBranch sub_branches[MAX_SUB_BRANCHES];
    int sub_count;
} Branch;

double parallel_resistance(double r1, double r2) {
    return (r1 * r2) / (r1 + r2);
}

double calculate_branch_resistance(Branch branch) {
    double total = 0;
    for (int i = 0; i < branch.sub_count; i++) {
        double sub_total = 0;
        for (int j = 0; j < branch.sub_branches[i].count; j++) {
            sub_total += branch.sub_branches[i].resistances[j];
        }
        if (total == 0) {
            total = sub_total;
        } else {
            total = parallel_resistance(total, sub_total);
        }
    }
    return total;
}

void draw_circuit(Branch branches[], int branch_count) {
    printf("\nCircuit Diagram:\n");
    printf("┌───l|────┐\n");
    for (int i = 0; i < branch_count; i++) {
        printf("│         │\n");
        printf("├──");
        for (int sb = 0; sb < branches[i].sub_count; sb++) {
            for (int j = 0; j < branches[i].sub_branches[sb].count; j++) {
                printf("┬[R%.2f]─", branches[i].sub_branches[sb].resistances[j]);
            }
            printf("┤");
            if (sb < branches[i].sub_count - 1) {
                printf("──┴──");
            }
        }
        printf("\n");
    }
    printf("│         │\n");
    printf("└─────────┘\n");
}

int main() {
    double voltage, resistance;
    char choice;
    int branch_count = 0;
    Branch branches[MAX_BRANCHES] = {0};

    printf("Enter the voltage source (in volts): ");
    scanf("%lf", &voltage);

    do {
        printf("\n--- Configuring Branch %d (Parallel Path) ---\n", branch_count + 1);
        printf("Enter the first resistor value (ohms): ");
        scanf("%lf", &resistance);

        branches[branch_count].sub_branches[0].resistances[0] = resistance;
        branches[branch_count].sub_branches[0].count = 1;
        branches[branch_count].sub_count = 1;

        draw_circuit(branches, branch_count + 1);

        char add_more;
        do {
            printf("\nAdd another resistor in this branch?\n");
            printf("1. Series (same path)\n2. Parallel (new path)\n3. Done\nChoice: ");
            int option;
            scanf("%d", &option);

            if (option == 1) {
                printf("Enter resistance value (ohms): ");
                scanf("%lf", &resistance);
                if (branches[branch_count].sub_branches[0].count < MAX_RESISTORS) {
                    int idx = branches[branch_count].sub_branches[0].count;
                    branches[branch_count].sub_branches[0].resistances[idx] = resistance;
                    branches[branch_count].sub_branches[0].count++;
                } else {
                    printf("Max resistors in this path reached!\n");
                }
            } else if (option == 2) {
                printf("Enter resistance value (ohms): ");
                scanf("%lf", &resistance);
                if (branches[branch_count].sub_count < MAX_SUB_BRANCHES) {
                    int new_sub = branches[branch_count].sub_count;
                    branches[branch_count].sub_branches[new_sub].resistances[0] = resistance;
                    branches[branch_count].sub_branches[new_sub].count = 1;
                    branches[branch_count].sub_count++;
                } else {
                    printf("Max parallel paths reached!\n");
                }
            } else {
                break;
            }

            draw_circuit(branches, branch_count + 1);
        } while (1);

        branch_count++;

        if (branch_count < MAX_BRANCHES) {
            printf("\nAdd another parallel branch? (Y/N): ");
            scanf(" %c", &choice);
        } else {
            printf("Maximum branches reached!\n");
            break;
        }
    } while (choice == 'Y' || choice == 'y');

    double total_resistance = 0;
    for (int i = 0; i < branch_count; i++) {
        double branch_resistance = calculate_branch_resistance(branches[i]);
        if (total_resistance == 0) {
            total_resistance = branch_resistance;
        } else {
            total_resistance = parallel_resistance(total_resistance, branch_resistance);
        }
    }

    double current = voltage / total_resistance;

    printf("\n--- Results ---\n");
    printf("Total Equivalent Resistance: %.2f ohms\n", total_resistance);
    printf("Total Current from Source: %.2f A\n", current);

    return 0;
}