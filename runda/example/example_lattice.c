#include <runda.h>
#include <stdio.h>

#define CHECK(eval) if (! eval) { \
    fprintf (stderr, "Exception:%s\n", runda_strerror (runda)); \
    runda_destroy(runda); \
    return -1; }

int main (int argc, char **argv)  {
  char input[] = "太郎は次郎が持っている本を花子に渡した。";
  runda_model_t *model, *another_model;
  runda_t *runda;
  runda_lattice_t *lattice;
  const runda_node_t *node;
  const char *result;
  int i;
  size_t len;

  model = runda_model_new(argc, argv);
  CHECK(model);

  runda = runda_model_new_tagger(model);
  CHECK(runda);

  lattice = runda_model_new_lattice(model);
  CHECK(lattice);

  runda_lattice_set_sentence(lattice, input);
  runda_parse_lattice(runda, lattice);

  printf("RESULT: %s\n", runda_lattice_tostr(lattice));

  node = runda_lattice_get_bos_node(lattice);
  for (;  node; node = node->next) {
    printf("%d ", node->id);

    if (node->stat == RUNDA_BOS_NODE)
      printf("BOS");
    else if (node->stat == RUNDA_EOS_NODE)
      printf("EOS");
    else
      fwrite (node->surface, sizeof(char), node->length, stdout);

    printf(" %s %d %d %d %d %d %d %d %d %f %f %f %ld\n",
	   node->feature,
	   (int)(node->surface - input),
	   (int)(node->surface - input + node->length),
	   node->rcAttr,
	   node->lcAttr,
	   node->posid,
	   (int)node->char_type,
	   (int)node->stat,
	   (int)node->isbest,
	   node->alpha,
	   node->beta,
	   node->prob,
	   node->cost);
  }

  len = runda_lattice_get_size(lattice);
  for (i = 0; i <= len; ++i) {
    runda_node_t *b, *e;
    b = runda_lattice_get_begin_nodes(lattice, (size_t)i);
    e = runda_lattice_get_end_nodes(lattice, (size_t)i);
    for (; b; b = b->bnext) {
        printf("B[%d] %s\t%s\n", i, b->surface, b->feature);
    }
    for (; e; e = e->enext) {
        printf("E[%d] %s\t%s\n", i, e->surface, e->feature);
    }
  }

  runda_lattice_set_sentence(lattice, input);
  runda_lattice_set_request_type(lattice, RUNDA_NBEST);
  runda_parse_lattice(runda, lattice);
  for (i = 0; i < 10; ++i) {
    fprintf(stdout, "%s", runda_lattice_tostr(lattice));
    if (!runda_lattice_next(lattice)) {
      break;
    }
  }

  runda_lattice_set_sentence(lattice, input);
  runda_lattice_set_request_type(lattice, RUNDA_MARGINAL_PROB);
  runda_lattice_set_theta(lattice, 0.001);
  runda_parse_lattice(runda, lattice);
  node = runda_lattice_get_bos_node(lattice);
  for (;  node; node = node->next) {
    fwrite(node->surface, sizeof(char), node->length, stdout);
    fprintf(stdout, "\t%s\t%f\n", node->feature, node->prob);
  }

  runda_set_lattice_level(runda, 0);
  runda_set_all_morphs(runda, 1);
  node = runda_sparse_tonode(runda, input);
  CHECK(node);
  for (; node; node = node->next) {
    fwrite (node->surface, sizeof(char), node->length, stdout);
    printf("\t%s\n", node->feature);
  }

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
  runda_lattice_destroy(lattice);
  runda_model_destroy(model);

  return 0;
}
