#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <runda.h>
#include <stdlib.h>
#include "../src/thread.h"

namespace RuNda {

class TaggerThread : public thread {
 public:
  void run() {
    int n = 0;
    while (true) {
      for (size_t i = 0; i < sentences_->size(); ++i) {
        lattice_->set_sentence((*sentences_)[i].c_str());
        tagger_->parse(lattice_);
        if (n % 100141 == 0) {
          std::cout << id_ << " " << n << " parsed" << std::endl;
//	  std::cout << lattice_->toString();
        }
        ++n;
      }
    }
  }

  TaggerThread(std::vector<std::string> *sentences,
               RuNda::Model *model,
               int id)
      : sentences_(sentences), tagger_(0), id_(id) {
    tagger_ = model->createTagger();
    lattice_ = model->createLattice();
  }

  ~TaggerThread() {
    delete tagger_;
    delete lattice_;
  }

 private:
  std::vector<std::string> *sentences_;
  RuNda::Tagger *tagger_;
  RuNda::Lattice *lattice_;
  int id_;
};

class ModelUpdater : public thread {
 public:
  void run() {
    const char *kParams[] = {
#ifdef _WIN32
      "-d unidic",
      "-d ipadic",
      "-d jumandic"
#else
      "-d /usr/local/lib/runda/dic/unidic/",
      "-d /usr/local/lib/runda/dic/ipadic/",
      "-d /usr/local/lib/runda/dic/jumandic/"
#endif
    };

    int i = 0;
    while (true) {
#ifdef _WIN32
      ::Sleep(4000);
#else
      sleep(4);
#endif
      RuNda::Model *model = RuNda::createModel(kParams[i % 3]);
      std::cout << "Updating..." << std::endl;
      if (!model_->swap(model)) {
        std::cerr << "cannot swap" << std::endl;
        exit(-1);
      }
      std::cout << "Done!" << std::endl;
      ++i;
    }
  }

  ModelUpdater(RuNda::Model *model) : model_(model) {}

 private:
  RuNda::Model *model_;
};
}

int main (int argc, char **argv) {
  std::ifstream ifs("japanese_sentences.txt");
  std::string line;
  std::vector<std::string> sentences;
  while (std::getline(ifs, line)) {
    sentences.push_back(line);
    if (sentences.size() == 10000) {
      break;
    }
  }

  RuNda::Model *model = RuNda::createModel(argc, argv);
  if (!model) {
    std::cerr << "model is NULL" << std::endl;
    return -1;
  }
  RuNda::ModelUpdater updater(model);
  updater.start();

  const int kMaxThreadSize = 8;

  std::vector<RuNda::TaggerThread *> threads(kMaxThreadSize);
  for (int i = 0; i < kMaxThreadSize; ++i) {
    threads[i] = new RuNda::TaggerThread(&sentences, model, i);
  }

  for (int i = 0; i < kMaxThreadSize; ++i) {
    threads[i]->start();
  }

  for (int i = 0; i < kMaxThreadSize; ++i) {
    threads[i]->join();
    delete threads[i];
  }

  updater.join();
   
  delete model;

  return 0;
}
