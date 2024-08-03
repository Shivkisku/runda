//
//  RuNda -- Yet Another Part-of-Speech and Morphological Analyzer
//
//  Copyright(C) 2001-2006 Taku Kudo <taku@chasen.org>
//  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation
#if defined(_WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#endif

#include "runda.h"
#include "tokenizer.h"
#include "utils.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace {
const char kUnknownError[] = "Unknown Error";
const size_t kErrorBufferSize = 256;
}

#if defined(_WIN32) && !defined(__CYGWIN__)
namespace {
DWORD g_tls_index = TLS_OUT_OF_INDEXES;
}

const char *getGlobalError() {
  LPVOID data = ::TlsGetValue(g_tls_index);
  return data == NULL ? kUnknownError : reinterpret_cast<const char *>(data);
}

void setGlobalError(const char *str) {
  char *data = reinterpret_cast<char *>(::TlsGetValue(g_tls_index));
  if (data == NULL) {
    return;
  }
  strncpy(data, str, kErrorBufferSize - 1);
  data[kErrorBufferSize - 1] = '\0';
}

HINSTANCE DllInstance = 0;

extern "C" {
  BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID) {
    LPVOID data = 0;
    if (!DllInstance) {
      DllInstance = hinst;
    }
    switch (dwReason) {
      case DLL_PROCESS_ATTACH:
        if ((g_tls_index = ::TlsAlloc()) == TLS_OUT_OF_INDEXES) {
          return FALSE;
        }
        // Not break in order to initialize the TLS.
      case DLL_THREAD_ATTACH:
        data = (LPVOID)::LocalAlloc(LPTR, kErrorBufferSize);
        if (data) {
          ::TlsSetValue(g_tls_index, data);
        }
        break;
      case DLL_THREAD_DETACH:
        data = ::TlsGetValue(g_tls_index);
        if (data) {
          ::LocalFree((HLOCAL)data);
        }
        break;
      case DLL_PROCESS_DETACH:
        data = ::TlsGetValue(g_tls_index);
        if (data) {
          ::LocalFree((HLOCAL)data);
        }
        ::TlsFree(g_tls_index);
        g_tls_index = TLS_OUT_OF_INDEXES;
        break;
      default:
        break;
    }
    return TRUE;
  }
}
#else  // _WIN32
namespace {
#ifdef HAVE_TLS_KEYWORD
__thread char kErrorBuffer[kErrorBufferSize];
#else
char kErrorBuffer[kErrorBufferSize];
#endif
}

const char *getGlobalError() {
  return kErrorBuffer;
}

void setGlobalError(const char *str) {
  strncpy(kErrorBuffer, str, kErrorBufferSize - 1);
  kErrorBuffer[kErrorBufferSize - 1] = '\0';
}
#endif

runda_t* runda_new(int argc, char **argv) {
  RuNda::Tagger *tagger = RuNda::createTagger(argc, argv);
  if (!tagger) {
    RuNda::deleteTagger(tagger);
    return 0;
  }
  return reinterpret_cast<runda_t *>(tagger);
}

runda_t* runda_new2(const char *arg) {
  RuNda::Tagger *tagger = RuNda::createTagger(arg);
  if (!tagger) {
    RuNda::deleteTagger(tagger);
    return 0;
  }
  return reinterpret_cast<runda_t *>(tagger);
}

const char *runda_version() {
  return RuNda::Tagger::version();
}

const char* runda_strerror(runda_t *tagger) {
  if (!tagger) {
    return RuNda::getLastError();
  }
  return reinterpret_cast<RuNda::Tagger *>(tagger)->what();
}

void runda_destroy(runda_t *tagger) {
  RuNda::Tagger *ptr = reinterpret_cast<RuNda::Tagger *>(tagger);
  RuNda::deleteTagger(ptr);
  ptr = 0;
}

int  runda_get_partial(runda_t *tagger) {
  return reinterpret_cast<RuNda::Tagger *>(tagger)->partial();
}

void runda_set_partial(runda_t *tagger, int partial) {
  reinterpret_cast<RuNda::Tagger *>(tagger)->set_partial(partial);
}

float  runda_get_theta(runda_t *tagger) {
  return reinterpret_cast<RuNda::Tagger *>(tagger)->theta();
}

void runda_set_theta(runda_t *tagger, float theta) {
  reinterpret_cast<RuNda::Tagger *>(tagger)->set_theta(theta);
}

int  runda_get_lattice_level(runda_t *tagger) {
  return reinterpret_cast<RuNda::Tagger *>(tagger)->lattice_level();
}

void runda_set_lattice_level(runda_t *tagger, int level) {
  reinterpret_cast<RuNda::Tagger *>(tagger)->set_lattice_level(level);
}

int runda_get_all_morphs(runda_t *tagger) {
  return static_cast<int>(
      reinterpret_cast<RuNda::Tagger *>(tagger)->all_morphs());
}

void runda_set_all_morphs(runda_t *tagger, int all_morphs) {
  reinterpret_cast<RuNda::Tagger *>(tagger)->set_all_morphs(all_morphs);
}

const char* runda_sparse_tostr(runda_t *tagger, const char *str) {
  return reinterpret_cast<RuNda::Tagger *>(tagger)->parse(str);
}

const char* runda_sparse_tostr2(runda_t *tagger, const char *str, size_t len) {
  return reinterpret_cast<RuNda::Tagger *>(tagger)->parse(str, len);
}

char* runda_sparse_tostr3(runda_t *tagger, const char *str, size_t len,
                          char *out, size_t len2) {
  return const_cast<char *>(
      reinterpret_cast<RuNda::Tagger *>(tagger)->parse(
          str, len, out, len2));
}

const runda_node_t* runda_sparse_tonode(runda_t *tagger, const char *str) {
  return reinterpret_cast<const runda_node_t *>(
      reinterpret_cast<RuNda::Tagger *>(tagger)->parseToNode(str));
}

const runda_node_t* runda_sparse_tonode2(runda_t *tagger,
                                         const char *str, size_t len) {
  return reinterpret_cast<const runda_node_t *>(
      reinterpret_cast<RuNda::Tagger *>(tagger)->parseToNode(str, len));
}

const char* runda_nbest_sparse_tostr(runda_t *tagger, size_t N,
                                     const char *str) {
  return reinterpret_cast<RuNda::Tagger *>(tagger)->parseNBest(N, str);
}

const char* runda_nbest_sparse_tostr2(runda_t *tagger, size_t N,
                                      const char* str, size_t len) {
  return reinterpret_cast<RuNda::Tagger *>(
      tagger)->parseNBest(N, str, len);
}

char* runda_nbest_sparse_tostr3(runda_t *tagger, size_t N,
                                const char *str, size_t len,
                                char *out, size_t len2) {
  return const_cast<char *>(
      reinterpret_cast<RuNda::Tagger *>(
          tagger)->parseNBest(N, str, len, out, len2));
}

int runda_nbest_init(runda_t *tagger, const char *str) {
  return reinterpret_cast<
      RuNda::Tagger *>(tagger)->parseNBestInit(str);
}

int runda_nbest_init2(runda_t *tagger, const char *str, size_t len) {
  return reinterpret_cast<
      RuNda::Tagger *>(tagger)->parseNBestInit(str, len);
}

const char* runda_nbest_next_tostr(runda_t *tagger) {
  return reinterpret_cast<RuNda::Tagger *>(tagger)->next();
}

char* runda_nbest_next_tostr2(runda_t *tagger, char *out, size_t len2) {
  return const_cast<char *>(
      reinterpret_cast<RuNda::Tagger *>(tagger)->next(out, len2));
}

const runda_node_t* runda_nbest_next_tonode(runda_t *tagger) {
  return reinterpret_cast<const runda_node_t *>(
      reinterpret_cast<RuNda::Tagger *>(tagger)->nextNode());
}

const char* runda_format_node(runda_t *tagger, const runda_node_t* n) {
  return reinterpret_cast<RuNda::Tagger *>(tagger)->formatNode(n);
}

const runda_dictionary_info_t *runda_dictionary_info(runda_t *tagger) {
  return reinterpret_cast<const runda_dictionary_info_t *>(
      reinterpret_cast<RuNda::Tagger *>(tagger)->dictionary_info());
}

int runda_parse_lattice(runda_t *runda, runda_lattice_t *lattice) {
  return static_cast<int>(
      reinterpret_cast<RuNda::Tagger *>(runda)->parse(
          reinterpret_cast<RuNda::Lattice *>(lattice)));
}

runda_lattice_t *runda_lattice_new() {
  return reinterpret_cast<runda_lattice_t *>(RuNda::createLattice());
}

void runda_lattice_destroy(runda_lattice_t *lattice) {
  RuNda::Lattice *ptr = reinterpret_cast<RuNda::Lattice *>(lattice);
  RuNda::deleteLattice(ptr);
  ptr = 0;
}

void runda_lattice_clear(runda_lattice_t *lattice) {
  reinterpret_cast<RuNda::Lattice *>(lattice)->clear();
}

int runda_lattice_is_available(runda_lattice_t *lattice) {
  return static_cast<int>(
      reinterpret_cast<RuNda::Lattice *>(lattice)->is_available());
}
runda_node_t *runda_lattice_get_bos_node(runda_lattice_t *lattice) {
  return reinterpret_cast<runda_node_t *>(
      reinterpret_cast<RuNda::Lattice *>(lattice)->bos_node());
}

runda_node_t *runda_lattice_get_eos_node(runda_lattice_t *lattice) {
  return reinterpret_cast<runda_node_t *>(
      reinterpret_cast<RuNda::Lattice *>(lattice)->eos_node());
}

runda_node_t **runda_lattice_get_all_begin_nodes(runda_lattice_t *lattice) {
  return reinterpret_cast<runda_node_t **>(
      reinterpret_cast<RuNda::Lattice *>(lattice)->begin_nodes());
}

runda_node_t **runda_lattice_get_all_end_nodes(runda_lattice_t *lattice) {
  return reinterpret_cast<runda_node_t **>(
      reinterpret_cast<RuNda::Lattice *>(lattice)->end_nodes());
}

runda_node_t *runda_lattice_get_begin_nodes(runda_lattice_t *lattice,
                                            size_t pos) {
  return reinterpret_cast<runda_node_t *>(
      reinterpret_cast<RuNda::Lattice *>(lattice)->begin_nodes(pos));
}

runda_node_t    *runda_lattice_get_end_nodes(runda_lattice_t *lattice,
                                             size_t pos) {
  return reinterpret_cast<runda_node_t *>(
      reinterpret_cast<RuNda::Lattice *>(lattice)->end_nodes(pos));
}

const char  *runda_lattice_get_sentence(runda_lattice_t *lattice) {
  return reinterpret_cast<RuNda::Lattice *>(lattice)->sentence();
}

void  runda_lattice_set_sentence(runda_lattice_t *lattice,
                                 const char *sentence) {
  reinterpret_cast<RuNda::Lattice *>(lattice)->set_sentence(sentence);
}

void runda_lattice_set_sentence2(runda_lattice_t *lattice,
                                 const char *sentence, size_t len) {
  reinterpret_cast<RuNda::Lattice *>(lattice)->set_sentence(
      sentence, len);
}

size_t runda_lattice_get_size(runda_lattice_t *lattice) {
  return reinterpret_cast<RuNda::Lattice *>(lattice)->size();
}

double runda_lattice_get_z(runda_lattice_t *lattice) {
  return reinterpret_cast<RuNda::Lattice *>(lattice)->Z();
}

void runda_lattice_set_z(runda_lattice_t *lattice, double Z) {
  reinterpret_cast<RuNda::Lattice *>(lattice)->set_Z(Z);
}

double runda_lattice_get_theta(runda_lattice_t *lattice) {
  return reinterpret_cast<RuNda::Lattice *>(lattice)->theta();
}

void runda_lattice_set_theta(runda_lattice_t *lattice, double theta) {
  reinterpret_cast<RuNda::Lattice *>(lattice)->set_theta(theta);
}

int runda_lattice_next(runda_lattice_t *lattice) {
  return static_cast<int>(
      reinterpret_cast<RuNda::Lattice *>(lattice)->next());
}

int runda_lattice_get_request_type(runda_lattice_t *lattice) {
  return reinterpret_cast<RuNda::Lattice *>(lattice)->request_type();
}

int runda_lattice_has_request_type(runda_lattice_t *lattice,
                                   int request_type) {
  return reinterpret_cast<RuNda::Lattice *>(
      lattice)->has_request_type(request_type);
}

void runda_lattice_set_request_type(runda_lattice_t *lattice,
                                    int request_type) {
  reinterpret_cast<RuNda::Lattice *>(
      lattice)->set_request_type(request_type);
}

void runda_lattice_add_request_type(runda_lattice_t *lattice,
                                    int request_type) {
  reinterpret_cast<RuNda::Lattice *>(
      lattice)->add_request_type(request_type);
}

void runda_lattice_remove_request_type(runda_lattice_t *lattice,
                                       int request_type) {
  return reinterpret_cast<RuNda::Lattice *>(
      lattice)->remove_request_type(request_type);
}

runda_node_t    *runda_lattice_new_node(runda_lattice_t *lattice) {
  return reinterpret_cast<runda_node_t *>(
      reinterpret_cast<RuNda::Lattice *>(lattice)->newNode());
}

const char *runda_lattice_tostr(runda_lattice_t *lattice) {
  return reinterpret_cast<RuNda::Lattice *>(lattice)->toString();
}

const char *runda_lattice_tostr2(runda_lattice_t *lattice,
                                 char *buf, size_t size) {
  return reinterpret_cast<RuNda::Lattice *>(
      lattice)->toString(buf, size);
}
const char *runda_lattice_nbest_tostr(runda_lattice_t *lattice,
                                      size_t N) {
  return reinterpret_cast<RuNda::Lattice *>(
      lattice)->enumNBestAsString(N);
}
const char *runda_lattice_nbest_tostr2(runda_lattice_t *lattice,
                                       size_t N, char *buf, size_t size) {
  return reinterpret_cast<RuNda::Lattice *>(
      lattice)->enumNBestAsString(N, buf, size);
}

int runda_lattice_has_constraint(runda_lattice_t *lattice) {
  return static_cast<bool>(reinterpret_cast<RuNda::Lattice *>(
                               lattice)->has_constraint());
}

int runda_lattice_get_boundary_constraint(runda_lattice_t *lattice,
                                          size_t pos) {
  return reinterpret_cast<RuNda::Lattice *>(
      lattice)->boundary_constraint(pos);
}

const char *runda_lattice_get_feature_constraint(runda_lattice_t *lattice,
                                                 size_t pos) {
  return reinterpret_cast<RuNda::Lattice *>(
      lattice)->feature_constraint(pos);
}

void runda_lattice_set_boundary_constraint(runda_lattice_t *lattice,
                                           size_t pos, int boundary_type) {
  return reinterpret_cast<RuNda::Lattice *>(
      lattice)->set_boundary_constraint(pos, boundary_type);
}

void runda_lattice_set_feature_constraint(runda_lattice_t *lattice,
                                          size_t begin_pos, size_t end_pos,
                                          const char *feature) {
  return reinterpret_cast<RuNda::Lattice *>(
      lattice)->set_feature_constraint(begin_pos, end_pos, feature);
}

void runda_lattice_set_result(runda_lattice_t *lattice,
                              const char *result) {
  return reinterpret_cast<RuNda::Lattice *>(lattice)->set_result(result);
}

const char *runda_lattice_strerror(runda_lattice_t *lattice) {
  return reinterpret_cast<RuNda::Lattice *>(lattice)->what();
}

runda_model_t *runda_model_new(int argc, char **argv) {
  RuNda::Model *model = RuNda::createModel(argc, argv);
  if (!model) {
    RuNda::deleteModel(model);
    return 0;
  }
  return reinterpret_cast<runda_model_t *>(model);
}

runda_model_t *runda_model_new2(const char *arg) {
  RuNda::Model *model = RuNda::createModel(arg);
  if (!model) {
    RuNda::deleteModel(model);
    return 0;
  }
  return reinterpret_cast<runda_model_t *>(model);
}

void runda_model_destroy(runda_model_t *model) {
  RuNda::Model *ptr = reinterpret_cast<RuNda::Model *>(model);
  RuNda::deleteModel(ptr);
  ptr = 0;
}

runda_t *runda_model_new_tagger(runda_model_t *model) {
  return reinterpret_cast<runda_t *>(
      reinterpret_cast<RuNda::Model *>(model)->createTagger());
}

runda_lattice_t *runda_model_new_lattice(runda_model_t *model) {
  return reinterpret_cast<runda_lattice_t *>(
      reinterpret_cast<RuNda::Model *>(model)->createLattice());
}

int runda_model_swap(runda_model_t *model, runda_model_t *new_model) {
  return static_cast<int>(
      reinterpret_cast<RuNda::Model *>(model)->swap(
          reinterpret_cast<RuNda::Model *>(new_model)));
}

const runda_dictionary_info_t* runda_model_dictionary_info(
    runda_model_t *model) {
  return reinterpret_cast<const runda_dictionary_info_t *>(
      reinterpret_cast<RuNda::Model *>(model)->dictionary_info());
}

int runda_model_transition_cost(runda_model_t *model,
                                unsigned short rcAttr,
                                unsigned short lcAttr) {
  return reinterpret_cast<RuNda::Model *>(model)->transition_cost(
      rcAttr, lcAttr);
}

runda_node_t *runda_model_lookup(runda_model_t *model,
                                 const char *begin,
                                 const char *end,
                                 runda_lattice_t *lattice) {
  return reinterpret_cast<runda_node_t *>(
      reinterpret_cast<RuNda::Model *>(model)->lookup(
          begin, end,
          reinterpret_cast<RuNda::Lattice *>(lattice)));
}
