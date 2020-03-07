#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv)
{
  int world_rank, world_size;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming at least 2 processes for this task
  if (world_size < 2)
  {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  // End of world size check




  // //MPI_Send() - blocking call
  // int number;
  // if (world_rank == 0)
  // {
  //   number = 10;
  //   int i = 1;
  //   for (i = 1 ; i < world_size ; i++){
  //     //int MPI Ssend(void *buf, int count, MPI Datatype datatype, int dest, int tag, MPI Comm comm)
  //     number = number + 6;
  //     printf("Process %d MPI_Send {\"int\" : %d } to process %d\n", world_rank, number, i);
  //     MPI_Send(&number, 1, MPI_INT, i , 0 , MPI_COMM_WORLD);
  //   }
  // }
  // else
  // {
  //  //int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status * status)
  //   MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  //   printf("Process %d MPI_Recv number %d\n", world_rank, number, );
  //   fflush(stdout);
  // }

  // //MPI_Ssend() - synchronous
  // int number = 94;
  // if (world_rank == 0)
  // {
  //   MPI_Ssend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  // }
  // else if (world_rank == 1)
  // {
  //   MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  //   printf("MPI_Ssend Process 1 received number %d from process 0\n", number);
  // }




  // //MPI_Bsend() - buffer space provided with MPI_Buffer_attach
  // char *buf;
  // int bufsize, size1, size2, stringMessageLength = 4, doubleMessageLength = 2;
  // char msg1[stringMessageLength], rmsg1[64];
  // double msg2[doubleMessageLength], rmsg2[64];

  // //get packet sizes
  // MPI_Pack_size(stringMessageLength, MPI_CHAR, MPI_COMM_WORLD, &size1);
  // MPI_Pack_size(doubleMessageLength, MPI_DOUBLE, MPI_COMM_WORLD, &size2);

  // //prepare buffer
  // bufsize = 3 * MPI_BSEND_OVERHEAD + size1 + size2;
  // buf = (char *)malloc(bufsize);
  // MPI_Buffer_attach(buf, bufsize);

  // //pack information
  // strncpy(msg1, "hejo", stringMessageLength);
  // msg2[0] = 1.23; msg2[1] = 3.21;

  // if (world_rank == 0)
  // {
  //   MPI_Bsend(msg1, stringMessageLength, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
  //   MPI_Bsend(msg2, doubleMessageLength, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
  // }
  // else if (world_rank == 1)
  // {
  //   MPI_Recv(rmsg1, stringMessageLength, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  //   MPI_Recv(rmsg2, doubleMessageLength, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  //   printf("MPI_Bsend Process %d received string %s from process 0\n", world_rank, rmsg1);
  //   printf("MPI_Bsend Process %d received doubles %f and  %f from process 0\n", world_rank, rmsg2[0], rmsg2[1]);
  //   MPI_Buffer_detach( &buf, &bufsize );
  // }




  // //MPI_Rsend() - blocking ready send, thread safe
  // int number = 66;

  // MPI_Request request;
  // //MPI_Status status;
  // if (world_rank == 0)
  // {
  //   // int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request * request)
  //   MPI_Irecv(          &number,   1,       MPI_INT,                 1,         0,      MPI_COMM_WORLD,      &request);
  //   printf("MPI_Rsend Process 1 received number %d from process 0\n", number);
  // }
  // else if (world_rank == 1)
  // {
  //   MPI_Rsend(&number, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
  //   //MPI_Wait(&request, &status);
  // }




  //MPI_Isend() - async
  // int number = 17, flag=0;
  // MPI_Request request;
  // MPI_Status status;

  // if (world_rank == 0)
  // {
  //   //int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request * request)
  //   MPI_Isend(&number, 1, MPI_INT, 1, 4444, MPI_COMM_WORLD, &request);
  //   MPI_Wait(&request, &status);
  // }
  // else if (world_rank == 1)
  // {
  //   // int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request * request)
  //   MPI_Irecv(&number, 1, MPI_INT, 0, 4444, MPI_COMM_WORLD, &request);
  //   MPI_Test(&request, &flag, &status);
  //   // now we will be testing until the opreation of sending is completed
  //   while(flag == 0){   
  //     MPI_Test(&request, &flag, &status); 
  //   }
  //   printf("MPI_Isend Process 1 received number %d from process 0\n", number);  
  // }

  MPI_Finalize();
  return 0;
}
