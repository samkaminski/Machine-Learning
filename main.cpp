#include "csvstream.hpp"
#include <iostream>
#include <cassert>  //assert
#include <functional> //less
#include <cstddef>
#include <utility>  //pair
#include <fstream>
#include <sstream>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <exception>
#include <set>
#include <cmath>

using namespace std;

set<string> unique_words(const string &str) {
  istringstream source(str);
  set<string> words;
  string word;
  while (source >> word) {
    words.insert(word);
  }
  return words;
}



class Classifier {
     
    string infile;
    map<string, string> row;
    set<string> unique_words_set;
    map<string, int> label_counts;
    map<string, int> word_counts;
    map<string, map<string, int> > label_word_counts;
    int number_of_posts;
    int number_of_unique_words;
    map<string, double> lable_log_prior;
    map<string, map<string, double> > log_label_word_counts;
    std::vector<std::pair<std::string, std::string> > label_content_pairs;
    map<string, std::set<string> > label_content_map;
    map<string, map<std::set<string>, double> > label_probs;
public:

    Classifier(const string &file_) : infile(file_) {}

    void train(const string &infile) {
        csvstream csvin(infile);

        number_of_posts = 0;

        while (csvin >> row) {
            number_of_posts++;
            string label = row["tag"];
            label_counts[label]++;
            label_content_pairs.push_back(std::make_pair(row["tag"], row["content"]));
            label_content_map[label].insert(row["content"]);

            set<string> content_words = unique_words(row["content"]);
            
            for (const string& word : content_words) {
                unique_words_set.insert(word);
                word_counts[word]++;
                label_word_counts[label][word]++;
            }
        }

    number_of_unique_words = unique_words_set.size();

}
int get_number_of_posts() const {
        return number_of_posts;
    }

void calculate_log_priors() {
    for (const auto &label_count : label_counts) {
        const string &label = label_count.first;
        int count = label_count.second;
        lable_log_prior[label] = log(static_cast<double>(count) / number_of_posts);
    }

    for (const auto &label_entry : label_word_counts) {

        
        for (const auto &label_entry_map : label_entry.second) {
            log_label_word_counts[label_entry.first][label_entry_map.first]
             = log(static_cast<double>
             (label_word_counts[label_entry.first][label_entry_map.first])
              / label_counts[label_entry.first]);
          
        }
    }



    
}

string predict(const set<string> &post_words, double &max_log_prob) {
    map<string, double> label_log_probs;
    max_log_prob = -numeric_limits<double>::infinity();
    string best_label;

    for (const auto &label_entry : label_content_map) {
        const string &label = label_entry.first;
        double log_prob = lable_log_prior[label];

        for (const string &word : post_words) {
            if (log_label_word_counts[label].find(word) 
            != log_label_word_counts[label].end()) {
                log_prob += log_label_word_counts[label][word]; 
            } else {
                if (word_counts.find(word) != word_counts.end()) {
                    log_prob += log(static_cast<double>(word_counts[word])
                     / number_of_posts);
                } else {
                    log_prob += log(1.0 / number_of_posts);
                }
            }
        }

        label_log_probs[label] = log_prob; 
    }

    for (const auto &label_prob : label_log_probs) {
        if (label_prob.second > max_log_prob) {
            max_log_prob = label_prob.second;
            best_label = label_prob.first;
        } else if (label_prob.second == 
        max_log_prob && label_prob.first < best_label) {
            best_label = label_prob.first; 
        }
    }

    return best_label;
}





    void print_data() const {
        for (const auto& pair : label_content_pairs) {
            std::cout << "  label = " << pair.first 
            << ", content = " << pair.second << std::endl;
        }
}

    void print_log_prior() const {
        for (const auto  &label_count : label_counts) {
            const string &label = label_count.first;
            int count = label_count.second;
            double log_prior = lable_log_prior.at(label);
            cout << "  " << label << ", " << count 
            << " examples, log-prior = " << log_prior << endl;
        }
    }

    void print_classifier() const {
        for (const auto &label_entry : log_label_word_counts) {
            const string &label = label_entry.first;
            for (const auto &word_log_likelihood : label_entry.second) {
                const string &word = word_log_likelihood.first;
                double log_likelihood = word_log_likelihood.second;
                cout << "  " << label << ":" << word <<
                 ", count = " << label_word_counts.at(label).at(word)
                     << ", log-likelihood = " << log_likelihood << endl;
            }
        }
    }
    int get_vocabulary_size(){
        return number_of_unique_words;
    }
    

};
int main(int argc, char *argv[]) {
    cout.precision(3);

    if (argc != 3 && !(argc == 4 && string(argv[3]) == "--debug")) {
        cout << "Usage: " << argv[0] <<
         " TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }

    string train_f = argv[1];
    string test_f = argv[2];
    bool debug_mode = (argc == 4 && string(argv[3]) == "--debug");

    Classifier classifier(train_f);
    classifier.train(train_f);

    classifier.calculate_log_priors();

   

    if (debug_mode) {
        cout << "training data:" << endl;
        classifier.print_data(); 

        cout << "trained on " << classifier.get_number_of_posts() 
        << " examples" << endl;
        cout << "vocabulary size = " 
        << classifier.get_vocabulary_size() << endl; 
        cout << endl;

        cout << "classes:" << endl;
        classifier.print_log_prior();

        cout << "classifier parameters:" << endl;
        classifier.print_classifier(); 
        cout << endl;
    }else{

    cout << "trained on " << classifier.get_number_of_posts() 
    << " examples" << endl << endl;
    }

  
    int number_of_correct = 0;
    int number_total = 0;

    cout << "test data:" << endl;

    csvstream csvin(test_f);
    map<string, string> row;
    while (csvin >> row) {
        set<string> post_words = unique_words(row["content"]);
    double log_prob;
    string predicted_label = classifier.predict(post_words, log_prob);

    cout << "  correct = " << row["tag"]
             << ", predicted = " << predicted_label
             << ", log-probability score = " << log_prob
             << endl;
        cout << "  content = " << row["content"] << endl << endl;

    if (row["tag"] == predicted_label) {
        number_of_correct++;
    }
    number_total++;
    }

    cout << "performance: " << number_of_correct
         << " / " << number_total
         << " posts predicted correctly" << std::endl;

    return 0;
}