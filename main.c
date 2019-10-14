#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ARG_COUNT 3
#define POLY_SHAPE 1
#define POLY_SIZE 2
#define BUFFER_SIZE 2
#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[]) {

    int pipe_arr[2];
    pid_t cpid;
    char buffer[BUFFER_SIZE];

    if (argc != ARG_COUNT) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char const *SHAPE = argv[POLY_SHAPE];
    char const *SIZE = argv[POLY_SIZE];

    printf("Number of sides: %s \nLength of each side: %s \n", SHAPE, SIZE);

    if (pipe(pipe_arr) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }


    // ================================================== MAIN CODE ==================================================



    if (cpid > 0) {
        int const int_size = atoi(SIZE);
        int const int_sides = atoi(SHAPE);
        printf("\nsize: %d\n", int_size);

        double perimeter = int_size * int_sides;
        double tan_180_s = tan(((double) 180.0) / int_sides);
        double apothem = ((double) int_size) / (2 * tan_180_s);

        double area = (perimeter * apothem) / 2;

        printf("\nArea of a %d sided polygon: %f\n", int_sides, area);

        close(pipe_arr[READ_END]);
        write(pipe_arr[WRITE_END], SHAPE, strlen(SHAPE) + 1);
        close(pipe_arr[WRITE_END]);
    } else if (cpid == 0) {    /* Child reads from pipe */
        close(pipe_arr[WRITE_END]);          /* Close unused write end */

        read(pipe_arr[READ_END], buffer, BUFFER_SIZE);
        close(pipe_arr[READ_END]);

        int const int_sides = atoi(buffer);
        int num_triangles = 0;

        printf("\nsides: %d\n", int_sides);

        if (int_sides >= 4) {
            num_triangles = int_sides - 2;
        }
        printf("\n%d triangles can fit in a %d sided polygon.\n", num_triangles, int_sides);

        _exit(EXIT_SUCCESS);

    } else {            /* Parent writes argv[1] to pipe */
        close(pipe_arr[READ_END]);          /* Close unused read end */
        write(pipe_arr[1], argv[1], strlen(argv[1]));
        close(pipe_arr[1]);          /* Reader will see EOF */
        wait(NULL);                /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}