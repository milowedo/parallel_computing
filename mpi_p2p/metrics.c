#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  int world_rank, world_size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int len;
  char name[MPI_MAX_PROCESSOR_NAME];
  MPI_Get_processor_name(name, &len);
  printf("I am process %d of total %d on cpu %s\n", world_rank, world_size, name);
  fflush(stdout);

  char *filename;
  int filename_size = 0;
  FILE *results_file;

  if (world_rank == 0)
  {
    if (argc > 1)
    {
      filename_size += strlen(argv[1]);
      filename = malloc(filename_size);
      filename[0] = '\0';
      strncat(filename, argv[1], filename_size);
    }
    else
    {
      filename_size += strlen("results");
      filename = malloc(filename_size);
      filename[0] = '\0';
      strncat(filename, "results", filename_size);
    }
    results_file = fopen(filename, "w");
  }

  //MPI_Request request;
  MPI_Status status;

  double t1, t2, probeTime;
  int numBytes, msg_length = 512, maxLength = 8388608, string_filler_iterator = 0,
                num_loops_within_single_size = 15*1000, single_size_iterator = 1;
  char* msgbuf = (char *)malloc(sizeof(char) * maxLength);;

  for (; string_filler_iterator < maxLength; string_filler_iterator++)
  {
    msgbuf[string_filler_iterator] = 'x';
  }

  if (world_rank == 0)
  {
    fprintf(results_file, "\n");
    for (; msg_length < maxLength; msg_length *= 2)
    {
      numBytes = sizeof(char) * msg_length * 2;
      fprintf(results_file, "%d", numBytes);
      for (; single_size_iterator <= num_loops_within_single_size; single_size_iterator++)
      {
        t1 = MPI_Wtime();
        MPI_Send(msgbuf, msg_length, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(msgbuf, msg_length, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
        t2 = MPI_Wtime();
        probeTime = t2 - t1;
        fprintf(results_file, ",%lf", probeTime);
      }
      fprintf(results_file, "\n");
      single_size_iterator = 1;
    }
  }
  if (world_rank == 1)
  {
    for (; msg_length < maxLength; msg_length *= 2)
    {
      for (; single_size_iterator <= num_loops_within_single_size; single_size_iterator++)
      {
        MPI_Recv(msgbuf, msg_length, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Send(msgbuf, msg_length, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
      }
      single_size_iterator = 1;
    }
  }

  if (world_rank == 0)
  {

    fclose(results_file);
    free(filename);
  }
  free(msgbuf);
  MPI_Finalize();
  printf("FINISHED\n");
  return 0;
}
