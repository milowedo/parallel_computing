#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{

  //MPI init && process info
  int world_rank, world_size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int len;
  char name[MPI_MAX_PROCESSOR_NAME];
  MPI_Get_processor_name(name, &len);
  printf("I am process %d of total %d on cpu %s\n", world_rank, world_size, name);
  fflush(stdout);
  //

  //file setting
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
  //

  char *buf;
  int bufsize, size;
  MPI_Status status;

  double t1, t2, probeTime;
  int numBytes, msg_length = 1, maxLength = 65536, string_filler_iterator = 0,
                num_loops_within_single_size = 100000, single_size_iterator = 1;
  char msgbuf[maxLength], rmsg1[maxLength * 2];

  for (; string_filler_iterator < maxLength; string_filler_iterator++)
  {
    msgbuf[string_filler_iterator] = 'x';
  }

  if (world_rank == 0)
  {
    fprintf(results_file, "\n");
    for (; msg_length < maxLength; msg_length *= 2)
    {
      //get packet sizes
      MPI_Pack_size(msg_length, MPI_CHAR, MPI_COMM_WORLD, &size);

      //prepare buffer
      bufsize = 3 * MPI_BSEND_OVERHEAD + size;
      buf = (char *)malloc(bufsize);
      MPI_Buffer_attach(buf, bufsize);

      numBytes = sizeof(char) * msg_length * 2;
      fprintf(results_file, "%d", numBytes);
      for (; single_size_iterator <= num_loops_within_single_size; single_size_iterator++)
      {
        t1 = MPI_Wtime();
        // MPI_Send(&msgbuf, msg_length, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Bsend(msgbuf, msg_length, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

        // MPI_Recv(&msgbuf, msg_length, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(rmsg1, msg_length, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        t2 = MPI_Wtime();
        probeTime = t2 - t1;
        fprintf(results_file, ",%lf", probeTime);
      }
      fprintf(results_file, "\n");
      MPI_Buffer_detach(&buf, &bufsize);
      single_size_iterator = 1;
    }
  }
  if (world_rank == 1)
  {
    for (; msg_length < maxLength; msg_length *= 2)
    {
      //get packet sizes
      MPI_Pack_size(msg_length, MPI_CHAR, MPI_COMM_WORLD, &size);

      //prepare buffer
      bufsize = 3 * MPI_BSEND_OVERHEAD + size;
      buf = (char *)malloc(bufsize);
      MPI_Buffer_attach(buf, bufsize);

      for (; single_size_iterator <= num_loops_within_single_size; single_size_iterator++)
      {
        MPI_Recv(rmsg1, msg_length, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Bsend(msgbuf, msg_length, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
      }
      MPI_Buffer_detach(&buf, &bufsize);
      single_size_iterator = 1;
    }
  }

  if (world_rank == 0)
  {

    fclose(results_file);
    realloc(filename, 0);
  }
  MPI_Finalize();
  printf("FINISHED\n");
  return 0;
}