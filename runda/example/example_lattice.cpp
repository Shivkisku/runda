#include <iostream>
#include <runda.h>

#define CHECK(eval) if (! eval) { \
   const char *e = tagger ? tagger->what() : RuNda::getTaggerError(); \
   std::cerr << "Exception:" << e << std::endl; \
   delete tagger; \
   return -1; }

int main (int argc, char **argv) {
  char input[] = "太郎は次郎が持っている本を花子に渡した。";

  // Create model object.
  RuNda::Model *model = RuNda::createModel(argc, argv);

  // Create Tagger
  // All taggers generated by Model::createTagger() method share
  // the same model/dictoinary.
  RuNda::Tagger *tagger = model->createTagger();
  CHECK(tagger);

  // Create lattice object per thread.
  RuNda::Lattice *lattice = model->createLattice();

  // Gets tagged result in string
  lattice->set_sentence(input);

  // this method is thread safe, as long as |lattice| is thread local.
  CHECK(tagger->parse(lattice));
  std::cout << lattice->toString() << std::endl;

  // Gets node object.
  const RuNda::Node* node = lattice->bos_node();
  CHECK(node);
  for (; node; node = node->next) {
    std::cout << node->id << ' ';
    if (node->stat == RUNDA_BOS_NODE)
      std::cout << "BOS";
    else if (node->stat == RUNDA_EOS_NODE)
      std::cout << "EOS";
    else
      std::cout.write (node->surface, node->length);

    std::cout << ' ' << node->feature
	      << ' ' << (int)(node->surface - input)
	      << ' ' << (int)(node->surface - input + node->length)
	      << ' ' << node->rcAttr
	      << ' ' << node->lcAttr
	      << ' ' << node->posid
	      << ' ' << (int)node->char_type
	      << ' ' << (int)node->stat
	      << ' ' << (int)node->isbest
	      << ' ' << node->alpha
	      << ' ' << node->beta
	      << ' ' << node->prob
	      << ' ' << node->cost << std::endl;
  }


  // begin_nodes/end_nodes
  const size_t len = lattice->size();
  for (int i = 0; i <= len; ++i) {
    RuNda::Node *b = lattice->begin_nodes(i);
    RuNda::Node *e = lattice->end_nodes(i);
    for (; b; b = b->bnext) {
      printf("B[%d] %s\t%s\n", i, b->surface, b->feature);
    }
    for (; e; e = e->enext) {
      printf("E[%d] %s\t%s\n", i, e->surface, e->feature);
    }
  }

  // N best results
  lattice->set_request_type(RUNDA_NBEST);
  lattice->set_sentence(input);
  CHECK(tagger->parse(lattice));
  for (int i = 0; i < 10; ++i) {
    std::cout << "NBEST: " << i << std::endl;
    std::cout << lattice->toString();
    if (!lattice->next()) {
      // No more results
      break;
    }
  }

  // Marginal probabilities
  lattice->remove_request_type(RUNDA_NBEST);
  lattice->set_request_type(RUNDA_MARGINAL_PROB);
  lattice->set_sentence(input);
  CHECK(tagger->parse(lattice));
  std::cout << lattice->theta() << std::endl;
  for (const RuNda::Node *node = lattice->bos_node();
       node; node = node->next) {
    std::cout.write(node->surface, node->length);
    std::cout << "\t" << node->feature;
    std::cout << "\t" << node->prob << std::endl;
  }

  // Dictionary info
  const RuNda::DictionaryInfo *d = model->dictionary_info();
  for (; d; d = d->next) {
    std::cout << "filename: " <<  d->filename << std::endl;
    std::cout << "charset: " <<  d->charset << std::endl;
    std::cout << "size: " <<  d->size << std::endl;
    std::cout << "type: " <<  d->type << std::endl;
    std::cout << "lsize: " <<  d->lsize << std::endl;
    std::cout << "rsize: " <<  d->rsize << std::endl;
    std::cout << "version: " <<  d->version << std::endl;
  }

  // Swap model atomically.
  RuNda::Model *another_model = RuNda::createModel("");
  model->swap(another_model);

  delete lattice;
  delete tagger;
  delete model;

  return 0;
}
