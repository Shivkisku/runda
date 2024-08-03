AUTOMAKE_OPTIONS = no-dependencies
lib_LTLIBRARIES = librunda.la
EXTRA_DIST = Makefile.msvc.in make.bat
pkglibexecdir = ${libexecdir}/runda
INCLUDES = -DDIC_VERSION=$(DIC_VERSION) $(RUNDA_NO_TLS) $(RUNDA_USE_UTF8_ONLY) -DRUNDA_DEFAULT_RC="\"$(RUNDA_DEFAULT_RC)\""
librunda_la_LDFLAGS = -no-undefined -version-info $(LTVERSION)
librunda_la_SOURCES = viterbi.cpp tagger.cpp  utils.cpp utils.h eval.cpp iconv_utils.cpp iconv_utils.h \
                      dictionary_rewriter.h dictionary_rewriter.cpp dictionary_generator.cpp \
	              dictionary_compiler.cpp context_id.h context_id.cpp \
                      winmain.h thread.h connector.cpp nbest_generator.h nbest_generator.cpp connector.h \
                      writer.h writer.cpp mmap.h ucs.h  \
	              string_buffer.h string_buffer.cpp \
		      tokenizer.h stream_wrapper.h common.h darts.h char_property.h ucstable.h \
			freelist.h viterbi.h param.cpp tokenizer.cpp \
			ucstable.h char_property.cpp dictionary.h scoped_ptr.h \
			param.h runda.h dictionary.cpp \
			feature_index.cpp  feature_index.h  lbfgs.cpp \
			lbfgs.h  learner_tagger.cpp  learner_tagger.h  learner.cpp  \
			learner_node.h librunda.cpp

include_HEADERS = runda.h
bin_PROGRAMS    = runda
pkglibexec_PROGRAMS = runda-dict-index runda-dict-gen runda-cost-train runda-system-eval runda-test-gen

runda_dict_index_SOURCES = runda-dict-index.cpp
runda_dict_index_LDADD = librunda.la

runda_dict_gen_SOURCES = runda-dict-gen.cpp
runda_dict_gen_LDADD = librunda.la

runda_system_eval_SOURCES = runda-system-eval.cpp
runda_system_eval_LDADD = librunda.la

runda_cost_train_SOURCES = runda-cost-train.cpp
runda_cost_train_LDADD = librunda.la

runda_test_gen_SOURCES = runda-test-gen.cpp
runda_test_gen_LDADD = librunda.la

runda_SOURCES = runda.cpp
runda_LDADD = librunda.la
