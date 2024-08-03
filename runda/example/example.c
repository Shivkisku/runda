#include <runda.h>
#include <stdio.h>

#define CHECK(eval) if (! eval) { \
    fprintf (stderr, "Exception:%s\n", runda_strerror (runda)); \
    runda_destroy(runda); \
    return -1; }

int main (int argc, char **argv)  {
  char input[] = "太郎は次郎が持っている本を花子に渡した。";
  runda_t *runda;
  const runda_node_t *node;
  const char *result;
  int i;
  size_t len;

  // Create tagger object
  runda = runda_new(argc, argv);
  CHECK(runda);

  // Gets tagged result in string.
  result = runda_sparse_tostr(runda, input);
  CHECK(result)
  printf ("INPUT: %s\n", input);
  printf ("RESULT:\n%s", result);

  // Gets N best results
  result = runda_nbest_sparse_tostr (runda, 3, input);
  CHECK(result);
  fprintf (stdout, "NBEST:\n%s", result);

  CHECK(runda_nbest_init(runda, input));
  for (i = 0; i < 3; ++i) {
    printf ("%d:\n%s", i, runda_nbest_next_tostr (runda));
  }

  // Gets node object
  node = runda_sparse_tonode(runda, input);
  CHECK(node);
  for (; node; node = node->next) {
    if (node->stat == RUNDA_NOR_NODE || node->stat == RUNDA_UNK_NODE) {
      fwrite (node->surface, sizeof(char), node->length, stdout);
      printf("\t%s\n", node->feature);
    }
  }

  // Dictionary info
  const runda_dictionary_info_t *d = runda_dictionary_info(runda);
  for (; d; d = d->next) {
    printf("filename: %s\n", d->filename);
    printf("charset: %s\n", d->charset);
    printf("size: %d\n", d->size);
    printf("type: %d\n", d->type);
    printf("lsize: %d\n", d->lsize);
    printf("rsize: %d\n", d->rsize);
    printf("version: %d\n", d->version);
  }

  runda_destroy(runda);

  return 0;
}
