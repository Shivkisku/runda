%module RuNda
%include exception.i
%{
#include "runda.h"

/* Workaround for ruby1.9.x */
#if defined SWIGRUBY
#include "ruby/version.h"
#if RUBY_API_VERSION_CODE >= 10900
#include "ruby/encoding.h"
#define rb_str_new rb_external_str_new
#endif
#endif
%}

%newobject surface;

%exception {
  try { $action }
  catch (char *e) { SWIG_exception (SWIG_RuntimeError, e); }
  catch (const char *e) { SWIG_exception (SWIG_RuntimeError, (char*)e); }
}

%rename(Node) runda_node_t;
%rename(Path) runda_path_t;
%rename(DictionaryInfo) runda_dictionary_info_t;
%ignore    runda_model_t;
%ignore    runda_lattice_t;
%nodefault runda_path_t;
%nodefault runda_node_t;

%feature("notabstract") RuNda::Tagger;
%feature("notabstract") RuNda::Lattice;
%feature("notabstract") RuNda::Model;

%immutable runda_dictionary_info_t::filename;
%immutable runda_dictionary_info_t::charset;
%immutable runda_dictionary_info_t::size;
%immutable runda_dictionary_info_t::lsize;
%immutable runda_dictionary_info_t::rsize;
%immutable runda_dictionary_info_t::type;
%immutable runda_dictionary_info_t::version;
%immutable runda_dictionary_info_t::next;

%immutable runda_path_t::rnode;
%immutable runda_path_t::lnode;
%immutable runda_path_t::rnext;
%immutable runda_path_t::lnext;
%immutable runda_path_t::cost;

%immutable runda_node_t::prev;
%immutable runda_node_t::next;
%immutable runda_node_t::enext;
%immutable runda_node_t::bnext;
%immutable runda_node_t::lpath;
%immutable runda_node_t::rpath;
%immutable runda_node_t::feature;
%immutable runda_node_t::length;
%immutable runda_node_t::rlength;
%immutable runda_node_t::id;
%immutable runda_node_t::rcAttr;
%immutable runda_node_t::lcAttr;
%immutable runda_node_t::posid;
%immutable runda_node_t::char_type;
%immutable runda_node_t::stat;
%immutable runda_node_t::isbest;
%immutable runda_node_t::alpha;
%immutable runda_node_t::beta;
%immutable runda_node_t::wcost;
%immutable runda_node_t::cost;
%immutable runda_node_t::surface;

%extend runda_node_t {
  char *surface;
}

%extend RuNda::Tagger {
   Tagger(const char *argc);
   Tagger();
   const char* parseToString(const char* str, size_t length = 0) {
     return self->parse(str, length);
   }
}

%extend RuNda::Model {
   Model(const char *argc);
   Model();
}

%extend RuNda::Lattice {
  Lattice();
  void set_sentence(const char *sentence) {
    self->add_request_type(RUNDA_ALLOCATE_SENTENCE);
    self->set_sentence(sentence);
  }
}

%{

RuNda::Tagger* new_RuNda_Tagger (const char *arg) {
  char *p = new char [strlen(arg) + 4];
  strcpy(p, "-C ");
  strcat(p, arg);
  RuNda::Tagger *tagger = RuNda::createTagger(p);
  delete [] p;
  if (! tagger) throw RuNda::getLastError();
  return tagger;
}

RuNda::Tagger* new_RuNda_Tagger () {
  RuNda::Tagger *tagger = RuNda::createTagger("-C");
  if (! tagger) throw RuNda::getLastError();
  return tagger;
}

void delete_RuNda_Tagger (RuNda::Tagger *t) {
  delete t;
  t = 0;
}

RuNda::Model* new_RuNda_Model (const char *arg) {
  char *p = new char [strlen(arg) + 4];
  strcpy(p, "-C ");
  strcat(p, arg);
  RuNda::Model *model = RuNda::createModel(p);
  delete [] p;
  if (! model) throw RuNda::getLastError();
  return model;
}

RuNda::Model* new_RuNda_Model () {
  RuNda::Model *model = RuNda::createModel("-C");
  if (! model) throw RuNda::getLastError();
  return model;
}

void delete_RuNda_Model (RuNda::Model *t) {
  delete t;
  t = 0;
}

RuNda::Lattice* new_RuNda_Lattice () {
  return RuNda::createLattice();
}

void delete_RuNda_Lattice (RuNda::Lattice *t) {
  delete t;
  t = 0;
}

char* runda_node_t_surface_get(runda_node_t *n) {
  char *s = new char [n->length + 1];
  memcpy (s, n->surface, n->length);
  s[n->length] = '\0';
  return s;
}
%}

%include ../src/runda.h
%include version.h
