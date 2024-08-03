/*
  RuNda -- Yet Another Part-of-Speech and Morphological Analyzer

  Copyright(C) 2001-2011 Taku Kudo <taku@chasen.org>
  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation
*/
#ifndef RUNDA_RUNDA_H_
#define RUNDA_RUNDA_H_

/* C/C++ common data structures  */

/**
 * DictionaryInfo structure
 */
struct runda_dictionary_info_t {
  /**
   * filename of dictionary
   * On Windows, filename is stored in UTF-8 encoding
   */
  const char                     *filename;

  /**
   * character set of the dictionary. e.g., "SHIFT-JIS", "UTF-8"
   */
  const char                     *charset;

  /**
   * How many words are registered in this dictionary.
   */
  unsigned int                    size;

  /**
   * dictionary type
   * this value should be RUNDA_USR_DIC, RUNDA_SYS_DIC, or RUNDA_UNK_DIC.
   */
  int                             type;

  /**
   * left attributes size
   */
  unsigned int                    lsize;

  /**
   * right attributes size
   */
  unsigned int                    rsize;

  /**
   * version of this dictionary
   */
  unsigned short                  version;

  /**
   * pointer to the next dictionary info.
   */
  struct runda_dictionary_info_t *next;
};

/**
 * Path structure
 */
struct runda_path_t {
  /**
   * pointer to the right node
   */
  struct runda_node_t* rnode;

  /**
   * pointer to the next right path
   */
  struct runda_path_t* rnext;

  /**
   * pointer to the left node
   */
  struct runda_node_t* lnode;

  /**
   * pointer to the next left path
   */

  struct runda_path_t* lnext;

  /**
   * local cost
   */
  int                  cost;

  /**
   * marginal probability
   */
  float                prob;
};

/**
 * Node structure
 */
struct runda_node_t {
  /**
   * pointer to the previous node.
   */
  struct runda_node_t  *prev;

  /**
   * pointer to the next node.
   */
  struct runda_node_t  *next;

  /**
   * pointer to the node which ends at the same position.
   */
  struct runda_node_t  *enext;

  /**
   * pointer to the node which starts at the same position.
   */
  struct runda_node_t  *bnext;

  /**
   * pointer to the right path.
   * this value is NULL if RUNDA_ONE_BEST mode.
   */
  struct runda_path_t  *rpath;

  /**
   * pointer to the right path.
   * this value is NULL if RUNDA_ONE_BEST mode.
   */
  struct runda_path_t  *lpath;

  /**
   * surface string.
   * this value is not 0 terminated.
   * You can get the length with length/rlength members.
   */
  const char           *surface;

  /**
   * feature string
   */
  const char           *feature;

  /**
   * unique node id
   */
  unsigned int          id;

  /**
   * length of the surface form.
   */
  unsigned short        length;

  /**
   * length of the surface form including white space before the morph.
   */
  unsigned short        rlength;

  /**
   * right attribute id
   */
  unsigned short        rcAttr;

  /**
   * left attribute id
   */
  unsigned short        lcAttr;

  /**
   * unique part of speech id. This value is defined in "pos.def" file.
   */
  unsigned short        posid;

  /**
   * character type
   */
  unsigned char         char_type;

  /**
   * status of this model.
   * This value is RUNDA_NOR_NODE, RUNDA_UNK_NODE, RUNDA_BOS_NODE, RUNDA_EOS_NODE, or RUNDA_EON_NODE.
   */
  unsigned char         stat;

  /**
   * set 1 if this node is best node.
   */
  unsigned char         isbest;

  /**
   * forward accumulative log summation.
   * This value is only available when RUNDA_MARGINAL_PROB is passed.
   */
  float                 alpha;

  /**
   * backward accumulative log summation.
   * This value is only available when RUNDA_MARGINAL_PROB is passed.
   */
  float                 beta;

  /**
   * marginal probability.
   * This value is only available when RUNDA_MARGINAL_PROB is passed.
   */
  float                 prob;

  /**
   * word cost.
   */
  short                 wcost;

  /**
   * best accumulative cost from bos node to this node.
   */
  long                  cost;
};

/**
 * Parameters for RuNda::Node::stat
 */
enum {
  /**
   * Normal node defined in the dictionary.
   */
  RUNDA_NOR_NODE = 0,
  /**
   * Unknown node not defined in the dictionary.
   */
  RUNDA_UNK_NODE = 1,
  /**
   * Virtual node representing a beginning of the sentence.
   */
  RUNDA_BOS_NODE = 2,
  /**
   * Virtual node representing a end of the sentence.
   */
  RUNDA_EOS_NODE = 3,

  /**
   * Virtual node representing a end of the N-best enumeration.
   */
  RUNDA_EON_NODE = 4
};

/**
 * Parameters for RuNda::DictionaryInfo::type
 */
enum {
  /**
   * This is a system dictionary.
   */
  RUNDA_SYS_DIC = 0,

  /**
   * This is a user dictionary.
   */
  RUNDA_USR_DIC = 1,

  /**
   * This is a unknown word dictionary.
   */
  RUNDA_UNK_DIC = 2
};

/**
 * Parameters for RuNda::Lattice::request_type
 */
enum {
  /**
   * One best result is obtained (default mode)
   */
  RUNDA_ONE_BEST          = 1,
  /**
   * Set this flag if you want to obtain N best results.
   */
  RUNDA_NBEST             = 2,
  /**
   * Set this flag if you want to enable a partial parsing mode.
   * When this flag is set, the input |sentence| needs to be written
   * in partial parsing format.
   */
  RUNDA_PARTIAL           = 4,
  /**
   * Set this flag if you want to obtain marginal probabilities.
   * Marginal probability is set in RuNda::Node::prob.
   * The parsing speed will get 3-5 times slower than the default mode.
   */
  RUNDA_MARGINAL_PROB     = 8,
  /**
   * Set this flag if you want to obtain alternative results.
   * Not implemented.
   */
  RUNDA_ALTERNATIVE       = 16,
  /**
   * When this flag is set, the result linked-list (Node::next/prev)
   * traverses all nodes in the lattice.
   */
  RUNDA_ALL_MORPHS        = 32,

  /**
   * When this flag is set, tagger internally copies the body of passed
   * sentence into internal buffer.
   */
  RUNDA_ALLOCATE_SENTENCE = 64
};

/**
 * Parameters for RuNda::Lattice::boundary_constraint_type
 */
enum {
  /**
   * The token boundary is not specified.
   */
  RUNDA_ANY_BOUNDARY = 0,

  /**
   * The position is a strong token boundary.
   */
  RUNDA_TOKEN_BOUNDARY = 1,

  /**
   * The position is not a token boundary.
   */
  RUNDA_INSIDE_TOKEN = 2
};

/* C interface  */
#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <windows.h>
#  ifdef DLL_EXPORT
#    define RUNDA_DLL_EXTERN  __declspec(dllexport)
#    define RUNDA_DLL_CLASS_EXTERN  __declspec(dllexport)
#  else
#    define RUNDA_DLL_EXTERN  __declspec(dllimport)
#  endif
#endif

#ifndef RUNDA_DLL_EXTERN
#  define RUNDA_DLL_EXTERN extern
#endif

#ifndef RUNDA_DLL_CLASS_EXTERN
#  define RUNDA_DLL_CLASS_EXTERN
#endif

  typedef struct runda_t                 runda_t;
  typedef struct runda_model_t           runda_model_t;
  typedef struct runda_lattice_t         runda_lattice_t;
  typedef struct runda_dictionary_info_t runda_dictionary_info_t;
  typedef struct runda_node_t            runda_node_t;
  typedef struct runda_path_t            runda_path_t;

#ifndef SWIG
  /* C interface */

  /* old runda interface */
  /**
   * C wrapper of RuNda::Tagger::create(argc, argv)
   */
  RUNDA_DLL_EXTERN runda_t*      runda_new(int argc, char **argv);

  /**
   * C wrapper of RuNda::Tagger::create(arg)
   */
  RUNDA_DLL_EXTERN runda_t*      runda_new2(const char *arg);

  /**
   * C wrapper of RuNda::Tagger::version()
   */
  RUNDA_DLL_EXTERN const char*   runda_version();

  /**
   * C wrapper of RuNda::getLastError()
   */
  RUNDA_DLL_EXTERN const char*   runda_strerror(runda_t *runda);

  /**
   * C wrapper of RuNda::deleteTagger(tagger)
   */
  RUNDA_DLL_EXTERN void          runda_destroy(runda_t *runda);

  /**
   * C wrapper of RuNda::Tagger:set_partial()
   */
  RUNDA_DLL_EXTERN int           runda_get_partial(runda_t *runda);

  /**
   * C wrapper of RuNda::Tagger::partial()
   */
  RUNDA_DLL_EXTERN void          runda_set_partial(runda_t *runda, int partial);

  /**
   * C wrapper of RuNda::Tagger::theta()
   */
  RUNDA_DLL_EXTERN float         runda_get_theta(runda_t *runda);

  /**
   * C wrapper of  RuNda::Tagger::set_theta()
   */
  RUNDA_DLL_EXTERN void          runda_set_theta(runda_t *runda, float theta);

  /**
   * C wrapper of RuNda::Tagger::lattice_level()
   */
  RUNDA_DLL_EXTERN int           runda_get_lattice_level(runda_t *runda);

  /**
   * C wrapper of RuNda::Tagger::set_lattice_level()
   */
  RUNDA_DLL_EXTERN void          runda_set_lattice_level(runda_t *runda, int level);

  /**
   * C wrapper of RuNda::Tagger::all_morphs()
   */
  RUNDA_DLL_EXTERN int           runda_get_all_morphs(runda_t *runda);

  /**
   * C wrapper of RuNda::Tagger::set_all_moprhs()
   */
  RUNDA_DLL_EXTERN void          runda_set_all_morphs(runda_t *runda, int all_morphs);

  /**
   * C wrapper of RuNda::Tagger::parse(RuNda::Lattice *lattice)
   */
  RUNDA_DLL_EXTERN int           runda_parse_lattice(runda_t *runda, runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Tagger::parse(const char *str)
   */
  RUNDA_DLL_EXTERN const char*   runda_sparse_tostr(runda_t *runda, const char *str);

  /**
   * C wrapper of RuNda::Tagger::parse(const char *str, size_t len)
   */
  RUNDA_DLL_EXTERN const char*   runda_sparse_tostr2(runda_t *runda, const char *str, size_t len);

  /**
   * C wrapper of RuNda::Tagger::parse(const char *str, char *ostr, size_t olen)
   */
  RUNDA_DLL_EXTERN char*         runda_sparse_tostr3(runda_t *runda, const char *str, size_t len,
                                                     char *ostr, size_t olen);

  /**
   * C wrapper of RuNda::Tagger::parseToNode(const char *str)
   */
  RUNDA_DLL_EXTERN const runda_node_t* runda_sparse_tonode(runda_t *runda, const char*);

  /**
   * C wrapper of RuNda::Tagger::parseToNode(const char *str, size_t len)
   */
  RUNDA_DLL_EXTERN const runda_node_t* runda_sparse_tonode2(runda_t *runda, const char*, size_t);

  /**
   * C wrapper of RuNda::Tagger::parseNBest(size_t N, const char *str)
   */
  RUNDA_DLL_EXTERN const char*   runda_nbest_sparse_tostr(runda_t *runda, size_t N, const char *str);

  /**
   * C wrapper of RuNda::Tagger::parseNBest(size_t N, const char *str, size_t len)
   */
  RUNDA_DLL_EXTERN const char*   runda_nbest_sparse_tostr2(runda_t *runda, size_t N,
                                                           const char *str, size_t len);

  /**
   * C wrapper of RuNda::Tagger::parseNBest(size_t N, const char *str, char *ostr, size_t olen)
   */
  RUNDA_DLL_EXTERN char*         runda_nbest_sparse_tostr3(runda_t *runda, size_t N,
                                                           const char *str, size_t len,
                                                           char *ostr, size_t olen);

  /**
   * C wrapper of RuNda::Tagger::parseNBestInit(const char *str)
   */
  RUNDA_DLL_EXTERN int           runda_nbest_init(runda_t *runda, const char *str);

  /**
   * C wrapper of RuNda::Tagger::parseNBestInit(const char *str, size_t len)
   */
  RUNDA_DLL_EXTERN int           runda_nbest_init2(runda_t *runda, const char *str, size_t len);

  /**
   * C wrapper of RuNda::Tagger::next()
   */
  RUNDA_DLL_EXTERN const char*   runda_nbest_next_tostr(runda_t *runda);

  /**
   * C wrapper of RuNda::Tagger::next(char *ostr, size_t olen)
   */
  RUNDA_DLL_EXTERN char*         runda_nbest_next_tostr2(runda_t *runda, char *ostr, size_t olen);

  /**
   * C wrapper of RuNda::Tagger::nextNode()
   */
  RUNDA_DLL_EXTERN const runda_node_t* runda_nbest_next_tonode(runda_t *runda);

  /**
   * C wrapper of RuNda::Tagger::formatNode(const Node *node)
   */
  RUNDA_DLL_EXTERN const char*   runda_format_node(runda_t *runda, const runda_node_t *node);

  /**
   * C wrapper of RuNda::Tagger::dictionary_info()
   */
  RUNDA_DLL_EXTERN const runda_dictionary_info_t* runda_dictionary_info(runda_t *runda);

  /* lattice interface */
  /**
   * C wrapper of RuNda::createLattice()
   */
  RUNDA_DLL_EXTERN runda_lattice_t *runda_lattice_new();

  /**
   * C wrapper of RuNda::deleteLattice(lattice)
   */
  RUNDA_DLL_EXTERN void             runda_lattice_destroy(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::clear()
   */
  RUNDA_DLL_EXTERN void             runda_lattice_clear(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::is_available()
   */

  RUNDA_DLL_EXTERN int              runda_lattice_is_available(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::bos_node()
   */
  RUNDA_DLL_EXTERN runda_node_t    *runda_lattice_get_bos_node(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::eos_node()
   */
  RUNDA_DLL_EXTERN runda_node_t    *runda_lattice_get_eos_node(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::begin_nodes()
   */

  RUNDA_DLL_EXTERN runda_node_t   **runda_lattice_get_all_begin_nodes(runda_lattice_t *lattice);
  /**
   * C wrapper of RuNda::Lattice::end_nodes()
   */
  RUNDA_DLL_EXTERN runda_node_t   **runda_lattice_get_all_end_nodes(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::begin_nodes(pos)
   */
  RUNDA_DLL_EXTERN runda_node_t    *runda_lattice_get_begin_nodes(runda_lattice_t *lattice, size_t pos);

  /**
   * C wrapper of RuNda::Lattice::end_nodes(pos)
   */
  RUNDA_DLL_EXTERN runda_node_t    *runda_lattice_get_end_nodes(runda_lattice_t *lattice, size_t pos);

  /**
   * C wrapper of RuNda::Lattice::sentence()
   */
  RUNDA_DLL_EXTERN const char      *runda_lattice_get_sentence(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::set_sentence(sentence)
   */
  RUNDA_DLL_EXTERN void             runda_lattice_set_sentence(runda_lattice_t *lattice, const char *sentence);

  /**
   * C wrapper of RuNda::Lattice::set_sentence(sentence, len)
   */

  RUNDA_DLL_EXTERN void             runda_lattice_set_sentence2(runda_lattice_t *lattice, const char *sentence, size_t len);

  /**
   * C wrapper of RuNda::Lattice::size()
   */
  RUNDA_DLL_EXTERN size_t           runda_lattice_get_size(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::Z()
   */
  RUNDA_DLL_EXTERN double           runda_lattice_get_z(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::set_Z()
   */
  RUNDA_DLL_EXTERN void             runda_lattice_set_z(runda_lattice_t *lattice, double Z);

  /**
   * C wrapper of RuNda::Lattice::theta()
   */
  RUNDA_DLL_EXTERN double           runda_lattice_get_theta(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::set_theta()
   */

  RUNDA_DLL_EXTERN void             runda_lattice_set_theta(runda_lattice_t *lattice, double theta);

  /**
   * C wrapper of RuNda::Lattice::next()
   */
  RUNDA_DLL_EXTERN int              runda_lattice_next(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::request_type()
   */
  RUNDA_DLL_EXTERN int              runda_lattice_get_request_type(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::has_request_type()
   */
  RUNDA_DLL_EXTERN int              runda_lattice_has_request_type(runda_lattice_t *lattice, int request_type);

  /**
   * C wrapper of RuNda::Lattice::set_request_type()
   */
  RUNDA_DLL_EXTERN void             runda_lattice_set_request_type(runda_lattice_t *lattice, int request_type);

  /**
   * C wrapper of RuNda::Lattice::add_request_type()
   */

  RUNDA_DLL_EXTERN void             runda_lattice_add_request_type(runda_lattice_t *lattice, int request_type);

  /**
   * C wrapper of RuNda::Lattice::remove_request_type()
   */
  RUNDA_DLL_EXTERN void             runda_lattice_remove_request_type(runda_lattice_t *lattice, int request_type);

  /**
   * C wrapper of RuNda::Lattice::newNode();
   */
  RUNDA_DLL_EXTERN runda_node_t    *runda_lattice_new_node(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::toString()
   */
  RUNDA_DLL_EXTERN const char      *runda_lattice_tostr(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::toString(buf, size)
   */
  RUNDA_DLL_EXTERN const char      *runda_lattice_tostr2(runda_lattice_t *lattice, char *buf, size_t size);

  /**
   * C wrapper of RuNda::Lattice::enumNBestAsString(N)
   */
  RUNDA_DLL_EXTERN const char      *runda_lattice_nbest_tostr(runda_lattice_t *lattice, size_t N);

  /**
   * C wrapper of RuNda::Lattice::enumNBestAsString(N, buf, size)
   */

  RUNDA_DLL_EXTERN const char      *runda_lattice_nbest_tostr2(runda_lattice_t *lattice, size_t N, char *buf, size_t size);

  /**
   * C wrapper of RuNda::Lattice::has_constraint()
   */
  RUNDA_DLL_EXTERN int             runda_lattice_has_constraint(runda_lattice_t *lattice);

  /**
   * C wrapper of RuNda::Lattice::boundary_constraint(pos)
   */
  RUNDA_DLL_EXTERN int             runda_lattice_get_boundary_constraint(runda_lattice_t *lattice, size_t pos);


  /**
   * C wrapper of RuNda::Lattice::feature_constraint(pos)
   */
  RUNDA_DLL_EXTERN const char     *runda_lattice_get_feature_constraint(runda_lattice_t *lattice, size_t pos);

  /**
   * C wrapper of RuNda::Lattice::boundary_constraint(pos, type)
   */
  RUNDA_DLL_EXTERN void            runda_lattice_set_boundary_constraint(runda_lattice_t *lattice, size_t pos, int boundary_type);

  /**
   * C wrapper of RuNda::Lattice::set_feature_constraint(begin_pos, end_pos, feature)
   */
  RUNDA_DLL_EXTERN void            runda_lattice_set_feature_constraint(runda_lattice_t *lattice, size_t begin_pos, size_t end_pos, const char *feature);

  /**
   * C wrapper of RuNda::Lattice::set_result(result);
   */
  RUNDA_DLL_EXTERN void            runda_lattice_set_result(runda_lattice_t *lattice, const char *result);

  /**
   * C wrapper of RuNda::Lattice::what()
   */
  RUNDA_DLL_EXTERN const char      *runda_lattice_strerror(runda_lattice_t *lattice);


  /* model interface */
  /**
   * C wapper of RuNda::Model::create(argc, argv)
   */
  RUNDA_DLL_EXTERN runda_model_t   *runda_model_new(int argc, char **argv);

  /**
   * C wapper of RuNda::Model::create(arg)
   */
  RUNDA_DLL_EXTERN runda_model_t   *runda_model_new2(const char *arg);

  /**
   * C wapper of RuNda::deleteModel(model)
   */

  RUNDA_DLL_EXTERN void             runda_model_destroy(runda_model_t *model);

  /**
   * C wapper of RuNda::Model::createTagger()
   */
  RUNDA_DLL_EXTERN runda_t         *runda_model_new_tagger(runda_model_t *model);

  /**
   * C wapper of RuNda::Model::createLattice()
   */
  RUNDA_DLL_EXTERN runda_lattice_t *runda_model_new_lattice(runda_model_t *model);

  /**
   * C wrapper of RuNda::Model::swap()
   */
  RUNDA_DLL_EXTERN int runda_model_swap(runda_model_t *model, runda_model_t *new_model);

  /**
   * C wapper of RuNda::Model::dictionary_info()
   */
  RUNDA_DLL_EXTERN const runda_dictionary_info_t* runda_model_dictionary_info(runda_model_t *model);

  /**
   * C wrapper of RuNda::Model::transition_cost()
   */
  RUNDA_DLL_EXTERN int runda_model_transition_cost(runda_model_t *model,
                                                   unsigned short rcAttr,
                                                   unsigned short lcAttr);

  /**
   * C wrapper of RuNda::Model::lookup()
   */
  RUNDA_DLL_EXTERN runda_node_t *runda_model_lookup(runda_model_t *model,
                                                    const char *begin,
                                                    const char *end,
                                                    runda_lattice_t *lattice);

  /* static functions */
  RUNDA_DLL_EXTERN int           runda_do(int argc, char **argv);
  RUNDA_DLL_EXTERN int           runda_dict_index(int argc, char **argv);
  RUNDA_DLL_EXTERN int           runda_dict_gen(int argc, char **argv);
  RUNDA_DLL_EXTERN int           runda_cost_train(int argc, char **argv);
  RUNDA_DLL_EXTERN int           runda_system_eval(int argc, char **argv);
  RUNDA_DLL_EXTERN int           runda_test_gen(int argc, char **argv);
#endif

#ifdef __cplusplus
}
#endif

/* C++ interface */
#ifdef __cplusplus

namespace RuNda {
typedef struct runda_dictionary_info_t DictionaryInfo;
typedef struct runda_path_t            Path;
typedef struct runda_node_t            Node;

template <typename N, typename P> class Allocator;
class Tagger;

/**
 * Lattice class
 */
class RUNDA_DLL_CLASS_EXTERN Lattice {
public:
  /**
   * Clear all internal lattice data.
   */
  virtual void clear()              = 0;

  /**
   * Return true if result object is available.
   * @return boolean
   */
  virtual bool is_available() const = 0;

  /**
   * Return bos (begin of sentence) node.
   * You can obtain all nodes via "for (const Node *node = lattice->bos_node(); node; node = node->next) {}"
   * @return bos node object
   */
  virtual Node *bos_node() const              = 0;

  /**
   * Return eos (end of sentence) node.
   * @return eos node object
   */
  virtual Node *eos_node() const              = 0;

#ifndef SWIG
  /**
   * This method is used internally.
   */
  virtual Node **begin_nodes() const          = 0;

  /**
   * This method is used internally.
   */
  virtual Node **end_nodes() const            = 0;
#endif

  /**
   * Return node linked list ending at |pos|.
   * You can obtain all nodes via "for (const Node *node = lattice->end_nodes(pos); node; node = node->enext) {}"
   * @param pos position of nodes. 0 <= pos < size()
   * @return node linked list
   */
  virtual Node *end_nodes(size_t pos) const   = 0;

  /**
   * Return node linked list starting at |pos|.
   * You can obtain all nodes via "for (const Node *node = lattice->begin_nodes(pos); node; node = node->bnext) {}"
   * @param pos position of nodes. 0 <= pos < size()
   * @return node linked list
   */
  virtual Node *begin_nodes(size_t pos) const = 0;

  /**
   * Return sentence.
   * If RUNDA_NBEST or RUNDA_PARTIAL mode is off, the returned poiner is the same as the one set by set_sentence().
   * @return sentence
   */
  virtual const char *sentence() const = 0;

  /**
   * Set sentence. This method does not take the ownership of the object.
   * @param sentence sentence
   */
  virtual void set_sentence(const char *sentence)             = 0;

#ifndef SWIG
  /**
   * Set sentence. This method does not take the ownership of the object.
   * @param sentence sentence
   * @param len length of the sentence
   */
  virtual void set_sentence(const char *sentence, size_t len) = 0;
#endif

  /**
   * Return sentence size.
   * @return sentence size
   */
  virtual size_t size() const                                 = 0;

  /**
   * Set normalization factor of CRF.
   * @param Z new normalization factor.
   */
  virtual void   set_Z(double Z) = 0;

  /**
   * return normalization factor of CRF.
   * @return normalization factor.
   */
  virtual double Z() const = 0;

  /**
   * Set temparature parameter theta.
   * @param theta temparature parameter.
   */
  virtual void  set_theta(float theta) = 0;

  /**
   * Return temparature parameter theta.
   * @return temparature parameter.
   */
  virtual float theta() const          = 0;

  /**
   * Obtain next-best result. The internal linked list structure is updated.
   * You should set RUNDA_NBEST reques_type in advance.
   * Return false if no more results are available or request_type is invalid.
   * @return boolean
   */
  virtual bool next() = 0;

  /**
   * Return the current request type.
   * @return request type
   */
  virtual int request_type() const                = 0;

  /**
   * Return true if the object has a specified request type.
   * @return boolean
   */
  virtual bool has_request_type(int request_type) const = 0;

  /**
   * Set request type.
   * @param request_type new request type assigned
   */
  virtual void set_request_type(int request_type) = 0;

  /**
   * Add request type.
   * @param request_type new request type added
   */
  virtual void add_request_type(int request_type) = 0;

  /**
   * Remove request type.
   * @param request_type new request type removed
   */
  virtual void remove_request_type(int request_type) = 0;

#ifndef SWIG
  /**
   * This method is used internally.
   */
  virtual Allocator<Node, Path> *allocator() const = 0;
#endif

  /**
   * Return new node. Lattice objects has the ownership of the node.
   * @return new node object
   */
  virtual Node *newNode() = 0;

  /**
   * Return string representation of the lattice.
   * Returned object is managed by this instance. When clear/set_sentence() method
   * is called, the returned buffer is initialized.
   * @return string representation of the lattice
   */
  virtual const char *toString()                = 0;

  /**
   * Return string representation of the node.
   * Returned object is managed by this instance. When clear/set_sentence() method
   * is called, the returned buffer is initialized.
   * @return string representation of the node
   * @param node node object
   */
  virtual const char *toString(const Node *node) = 0;

  /**
   * Return string representation of the N-best results.
   * Returned object is managed by this instance. When clear/set_sentence() method
   * is called, the returned buffer is initialized.
   * @return string representation of the node
   * @param N how many results you want to obtain
   */
  virtual const char *enumNBestAsString(size_t N) = 0;

#ifndef SWIG
  /**
   * Return string representation of the lattice.
   * Result is saved in the specified buffer.
   * @param buf output buffer
   * @param size output buffer size
   * @return string representation of the lattice
   */
  virtual const char *toString(char *buf, size_t size) = 0;

  /**
   * Return string representation of the node.
   * Result is saved in the specified buffer.
   * @param node node object
   * @param buf output buffer
   * @param size output buffer size
   * @return string representation of the lattice
   */
  virtual const char *toString(const Node *node,
                               char *buf, size_t size) = 0;

  /**
   * Return string representation of the N-best result.
   * Result is saved in the specified.
   * @param N how many results you want to obtain
   * @param buf output buffer
   * @param size output buffer size
   * @return string representation of the lattice
   */
  virtual const char *enumNBestAsString(size_t N, char *buf, size_t size) = 0;
#endif

  /**
   * Returns true if any parsing constraint is set
   */
  virtual bool has_constraint() const = 0;

  /**
   * Returns the boundary constraint at the position.
   * @param pos the position of constraint
   * @return boundary constraint type
   */
  virtual int boundary_constraint(size_t pos) const = 0;

  /**
   * Returns the token constraint at the position.
   * @param pos the beginning position of constraint.
   * @return constrained node starting at the position.
   */
  virtual const char *feature_constraint(size_t pos) const = 0;

  /**
   * Set parsing constraint for partial parsing mode.
   * @param pos the position of the boundary
   * @param boundary_constraint_type the type of boundary
   */
  virtual void set_boundary_constraint(size_t pos,
                                       int boundary_constraint_type) = 0;

  /**
   * Set parsing constraint for partial parsing mode.
   * @param begin_pos the starting position of the constrained token.
   * @param end_pos the the ending position of the constrained token.
   * @param feature the feature of the constrained token.
   */
  virtual void set_feature_constraint(
      size_t begin_pos, size_t end_pos,
      const char *feature) = 0;

  /**
   * Set golden parsing results for unittesting.
   * @param result the parsing result written in the standard runda output.
   */
  virtual void set_result(const char *result) = 0;

  /**
   * Return error string.
   * @return error string
   */
  virtual const char *what() const            = 0;

  /**
   * Set error string. given string is copied to the internal buffer.
   * @param str new error string
   */
  virtual void set_what(const char *str)        = 0;

#ifndef SWIG
  /**
   * Create new Lattice object
   * @return new Lattice object
   */
  static Lattice *create();
#endif

  virtual ~Lattice() {}
};

/**
 * Model class
 */
class RUNDA_DLL_CLASS_EXTERN Model {
public:
  /**
   * Return DictionaryInfo linked list.
   * @return DictionaryInfo linked list
   */
  virtual const DictionaryInfo *dictionary_info() const = 0;

  /**
   * Return transtion cost from rcAttr to lcAttr.
   * @return transtion cost
   */
  virtual int transition_cost(unsigned short rcAttr,
                              unsigned short lcAttr) const = 0;

  /**
   * perform common prefix search from the range [begin, end).
   * |lattice| takes the ownership of return value.
   * @return node linked list.
   */
  virtual Node *lookup(const char *begin, const char *end,
                       Lattice *lattice) const = 0;

  /**
   * Create a new Tagger object.
   * All returned tagger object shares this model object as a parsing model.
   * Never delete this model object before deleting tagger object.
   * @return new Tagger object
   */
  virtual Tagger  *createTagger() const = 0;

  /**
   * Create a new Lattice object.
   * @return new Lattice object
   */
  virtual Lattice *createLattice() const = 0;

  /**
   * Swap the instance with |model|.
   * The ownership of |model| always moves to this instance,
   * meaning that passed |model| will no longer be accessible after calling this method.
   * return true if new model is swapped successfully.
   * This method is thread safe. All taggers created by
   * Model::createTagger() method will also be updated asynchronously.
   * No need to stop the parsing thread excplicitly before swapping model object.
   * @return boolean
   * @param model new model which is going to be swapped with the current model.
   */
  virtual bool swap(Model *model) = 0;

  /**
   * Return a version string
   * @return version string
   */
  static const char *version();

  virtual ~Model() {}

#ifndef SWIG
  /**
   * Factory method to create a new Model with a specified main's argc/argv-style parameters.
   * Return NULL if new model cannot be initialized. Use RuNda::getLastError() to obtain the
   * cause of the errors.
   * @return new Model object
   * @param argc number of parameters
   * @param argv parameter list
   */
  static Model* create(int argc, char **argv);

  /**
   * Factory method to create a new Model with a string parameter representation, i.e.,
   * "-d /user/local/runda/dic/ipadic -Ochasen".
   * Return NULL if new model cannot be initialized. Use RuNda::getLastError() to obtain the
   * cause of the errors.
   * @return new Model object
   * @param arg single string representation of the argment.
   */
  static Model* create(const char *arg);
#endif
};

/**
 * Tagger class
 */
class RUNDA_DLL_CLASS_EXTERN Tagger {
public:
  /**
   * Handy static method.
   * Return true if lattice is parsed successfully.
   * This function is equivalent to
   * {
   *   Tagger *tagger = model.createModel();
   *   cosnt bool result = tagger->parse(lattice);
   *   delete tagger;
   *   return result;
   * }
   * @return boolean
   */
  static bool  parse(const Model &model, Lattice *lattice);

  /**
   * Parse lattice object.
   * Return true if lattice is parsed successfully.
   * A sentence must be set to the lattice with Lattice:set_sentence object before calling this method.
   * Parsed node object can be obtained with Lattice:bos_node.
   * This method is thread safe.
   * @return lattice lattice object
   * @return boolean
   */
  virtual bool parse(Lattice *lattice) const                = 0;

  /**
   * Parse given sentence and return parsed result as string.
   * You should not delete the returned string. The returned buffer
   * is overwritten when parse method is called again.
   * This method is NOT thread safe.
   * @param str sentence
   * @return parsed result
   */
  virtual const char* parse(const char *str)                = 0;

  /**
   * Parse given sentence and return Node object.
   * You should not delete the returned node object. The returned buffer
   * is overwritten when parse method is called again.
   * You can traverse all nodes via Node::next member.
   * This method is NOT thread safe.
   * @param str sentence
   * @return bos node object
   */
  virtual const Node* parseToNode(const char *str)          = 0;

  /**
   * Parse given sentence and obtain N-best results as a string format.
   * Currently, N must be 1 <= N <= 512 due to the limitation of the buffer size.
   * You should not delete the returned string. The returned buffer
   * is overwritten when parse method is called again.
   * This method is DEPRECATED. Use Lattice class.
   * @param N how many results you want to obtain
   * @param str sentence
   * @return parsed result
   */
  virtual const char* parseNBest(size_t N, const char *str) = 0;

  /**
   * Initialize N-best enumeration with a sentence.
   * Return true if initialization finishes successfully.
   * N-best result is obtained by calling next() or nextNode() in sequence.
   * This method is NOT thread safe.
   * This method is DEPRECATED. Use Lattice class.
   * @param str sentence
   * @return boolean
   */
  virtual bool  parseNBestInit(const char *str)             = 0;

  /**
   * Return next-best parsed result. You must call parseNBestInit() in advance.
   * Return NULL if no more reuslt is available.
   * This method is NOT thread safe.
   * This method is DEPRECATED. Use Lattice class.
   * @return node object
   */
  virtual const Node* nextNode()                            = 0;

  /**
   * Return next-best parsed result. You must call parseNBestInit() in advance.
   * Return NULL if no more reuslt is available.
   * This method is NOT thread safe.
   * This method is DEPRECATED. Use Lattice class.
   * @return parsed result
   */
  virtual const char* next()                                = 0;

  /**
   * Return formatted node object. The format is specified with
   * --unk-format, --bos-format, --eos-format, and --eon-format respectively.
   * You should not delete the returned string. The returned buffer
   * is overwritten when parse method is called again.
   * This method is NOT thread safe.
   * This method is DEPRECATED. Use Lattice class.
   * @param node node object.
   * @return parsed result
   */
  virtual const char* formatNode(const Node *node)          = 0;

#ifndef SWIG
  /**
   * The same as parse() method, but input length and output buffer are passed.
   * Return parsed result as string. The result pointer is the same as |ostr|.
   * Return NULL, if parsed result string cannot be stored within |olen| bytes.
   * @param str sentence
   * @param len sentence length
   * @param ostr output buffer
   * @param olen output buffer length
   * @return parsed result
   */
  virtual const char* parse(const char *str, size_t len, char *ostr, size_t olen) = 0;

  /**
   * The same as parse() method, but input length can be passed.
   * @param str sentence
   * @param len sentence length
   * @return parsed result
   */
  virtual const char* parse(const char *str, size_t len)                          = 0;

  /**
   * The same as parseToNode(), but input lenth can be passed.
   * @param str sentence
   * @param len sentence length
   * @return node object
   */
  virtual const Node* parseToNode(const char *str, size_t len)                    = 0;

  /**
   * The same as parseNBest(), but input length can be passed.
   * @param N how many results you want to obtain
   * @param str sentence
   * @param len sentence length
   * @return parsed result
   */
  virtual const char* parseNBest(size_t N, const char *str, size_t len)           = 0;

  /**
   * The same as parseNBestInit(), but input length can be passed.
   * @param str sentence
   * @param len sentence length
   * @return boolean
   * @return parsed result
   */
  virtual bool  parseNBestInit(const char *str, size_t len)                  = 0;

  /**
   * The same as next(), but output buffer can be passed.
   * Return NULL if more than |olen| buffer is required to store output string.
   * @param ostr output buffer
   * @param olen output buffer length
   * @return parsed result
   */
  virtual const char* next(char *ostr , size_t olen)                        = 0;

  /**
   * The same as parseNBest(), but input length and output buffer can be passed.
   * Return NULL if more than |olen| buffer is required to store output string.
   * @param N how many results you want to obtain
   * @param str input sentence
   * @param len input sentence length
   * @param ostr output buffer
   * @param olen output buffer length
   * @return parsed result
   */
  virtual const char* parseNBest(size_t N, const char *str,
                                 size_t len, char *ostr, size_t olen)       = 0;

  /**
   * The same as formatNode(), but output buffer can be passed.
   * Return NULL if more than |olen| buffer is required to store output string.
   * @param node node object
   * @param ostr output buffer
   * @param olen output buffer length
   * @return parsed result
   */
  virtual const char* formatNode(const Node *node, char *ostr, size_t olen) = 0;
#endif

  /**
   * Set request type.
   * This method is DEPRECATED. Use Lattice::set_request_type(RUNDA_PARTIAL).
   * @param request_type new request type assigned
   */
  virtual void set_request_type(int request_type) = 0;

  /**
   * Return the current request type.
   * This method is DEPRECATED. Use Lattice class.
   * @return request type
   */
  virtual int  request_type() const = 0;

  /**
   * Return true if partial parsing mode is on.
   * This method is DEPRECATED. Use Lattice::has_request_type(RUNDA_PARTIAL).
   * @return boolean
   */
  virtual bool  partial() const                             = 0;

  /**
   * set partial parsing mode.
   * This method is DEPRECATED. Use Lattice::add_request_type(RUNDA_PARTIAL) or Lattice::remove_request_type(RUNDA_PARTIAL)
   * @param partial partial mode
   */
  virtual void  set_partial(bool partial)                   = 0;

  /**
   * Return lattice level.
   * This method is DEPRECATED. Use Lattice::*_request_type()
   * @return int lattice level
   */
  virtual int   lattice_level() const                       = 0;

  /**
   * Set lattice level.
   * This method is DEPRECATED. Use Lattice::*_request_type()
   * @param level lattice level
   */
  virtual void  set_lattice_level(int level)                = 0;

  /**
   * Return true if all morphs output mode is on.
   * This method is DEPRECATED. Use Lattice::has_request_type(RUNDA_ALL_MORPHS).
   * @return boolean
   */
  virtual bool  all_morphs() const                          = 0;

  /**
   * set all-morphs output mode.
   * This method is DEPRECATED. Use Lattice::add_request_type(RUNDA_ALL_MORPHS) or Lattice::remove_request_type(RUNDA_ALL_MORPHS)
   * @param all_morphs
   */
  virtual void  set_all_morphs(bool all_morphs)             = 0;

  /**
   * Set temparature parameter theta.
   * @param theta temparature parameter.
   */
  virtual void  set_theta(float theta)                      = 0;

  /**
   * Return temparature parameter theta.
   * @return temparature parameter.
   */
  virtual float theta() const                               = 0;

  /**
   * Return DictionaryInfo linked list.
   * @return DictionaryInfo linked list
   */
  virtual const DictionaryInfo* dictionary_info() const = 0;

  /**
   * Return error string.
   * @return error string
   */
  virtual const char* what() const = 0;

  virtual ~Tagger() {}

#ifndef SWIG
  /**
   * Factory method to create a new Tagger with a specified main's argc/argv-style parameters.
   * Return NULL if new model cannot be initialized. Use RuNda::getLastError() to obtain the
   * cause of the errors.
   * @return new Tagger object
   * @param argc number of parameters
   * @param argv parameter list
   */
  static Tagger *create(int argc, char **argv);

  /**
   * Factory method to create a new Tagger with a string parameter representation, i.e.,
   * "-d /user/local/runda/dic/ipadic -Ochasen".
   * Return NULL if new model cannot be initialized. Use RuNda::getLastError() to obtain the
   * cause of the errors.
   * @return new Model object
   * @param arg single string representation of the argment.
   */
  static Tagger *create(const char *arg);
#endif

  /**
   * Return a version string
   * @return version string
   */
  static const char *version();
};

#ifndef SWIG
/**
 * Alias of Lattice::create()
 */
RUNDA_DLL_EXTERN Lattice     *createLattice();

/**
 * Alias of Mode::create(argc, argv)
 */
RUNDA_DLL_EXTERN Model       *createModel(int argc, char **argv);

/**
 * Alias of Mode::create(arg)
 */
RUNDA_DLL_EXTERN Model       *createModel(const char *arg);

/**
 * Alias of Tagger::create(argc, argv)
 */
RUNDA_DLL_EXTERN Tagger      *createTagger(int argc, char **argv);

/**
 * Alias of Tagger::create(arg)
 */
RUNDA_DLL_EXTERN Tagger      *createTagger(const char *arg);

/**
 * delete Lattice object.
 * This method calles "delete lattice".
 * In some environment, e.g., MS-Windows, an object allocated inside a DLL must be deleted in the same DLL too.
 * @param lattice lattice object
 */
RUNDA_DLL_EXTERN void        deleteLattice(Lattice *lattice);


/**
 * delete Model object.
 * This method calles "delete model".
 * In some environment, e.g., MS-Windows, an object allocated inside a DLL must be deleted in the same DLL too.
 * @param model model object
 */
RUNDA_DLL_EXTERN void        deleteModel(Model *model);

/**
 * delete Tagger object.
 * This method calles "delete tagger".
 * In some environment, e.g., MS-Windows, an object allocated inside a DLL must be deleted in the same DLL too.
 * @param tagger tagger object
 */
RUNDA_DLL_EXTERN void        deleteTagger(Tagger *tagger);

/**
 * Return last error string.
 * @return error string
 */
RUNDA_DLL_EXTERN const char*  getLastError();

/**
 * An alias of getLastError.
 * It is kept for backward compatibility.
 * @return error string
 */
RUNDA_DLL_EXTERN const char*  getTaggerError();
#endif
}
#endif
#endif  /* RUNDA_RUNDA_H_ */
