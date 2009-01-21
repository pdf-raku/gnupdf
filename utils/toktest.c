#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pdf-rd-tokeniser.h"
#include "pdf-obj.h"
#include "pdf-error.h"

void print_file(FILE *file)
{
  pdf_status_t rv;
  pdf_tokeniser_t tokeniser = NULL;
  pdf_obj_t token;
  pdf_stm_t stm = NULL;

  rv = pdf_stm_cfile_new (file, 0, 0 /*cache_size*/, PDF_STM_READ, &stm);
  if (rv != PDF_OK)
    {
      fprintf(stderr, "failed to create stream\n");
      goto out;
    }

  rv = pdf_tokeniser_new(stm, &tokeniser);
  if (rv != PDF_OK)
    {
      fprintf(stderr, "failed to create tokeniser\n");
      goto out;
    }

  while (( rv = pdf_tokeniser_read(tokeniser, &token) ) == PDF_OK)
    {
      print_obj(token);
      pdf_obj_destroy(token);
    }

  if (rv != PDF_EEOF)
    {
      fprintf(stderr, "read_token error %d\n", rv);
      goto out;
    }

  fprintf(stderr, "done\n");
out:
  if (tokeniser) pdf_tokeniser_destroy(tokeniser);
  if (stm) pdf_stm_destroy(stm);
}

int main(int argc, char **argv)
{
  setlocale(LC_ALL, "");
  print_file(stdin);
  return 0;
}
