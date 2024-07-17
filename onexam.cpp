#include <iostream>  
#include <string>   
#include <vector>   
#include <algorithm> 
#include <random>    
#include <fstream>  

class Question {
public:
    std::string questionText;            
    std::vector<std::string> options;    
    int correctOption;                   

    Question(std::string text, std::vector<std::string> opts, int correct)
        : questionText(text), options(opts), correctOption(correct) {}

    Question(const Question& other)
        : questionText(other.questionText), options(other.options), correctOption(other.correctOption) {}

    void shuffleOptions() {
        std::vector<std::string> originalOptions = options;
        std::shuffle(options.begin(), options.end(), std::default_random_engine{});
        correctOption = std::distance(options.begin(), std::find(options.begin(), options.end(), originalOptions[correctOption]));
    }

    void display(std::ofstream& htmlFile, int questionIndex) const {
        htmlFile << "<div class='question'>" << std::endl;
        htmlFile << "<p>" << questionText << "</p>" << std::endl;
        htmlFile << "<ul class='options'>" << std::endl;
        for (size_t i = 0; i < options.size(); ++i) {
            htmlFile << "<li><input type='radio' name='q" << questionIndex << "' value='" << i << "'> " << options[i] << "</li>" << std::endl;
        }
        htmlFile << "</ul>" << std::endl;
        htmlFile << "<input type='hidden' id='correct_q" << questionIndex << "' value='" << correctOption << "'>" << std::endl;
        htmlFile << "</div>" << std::endl;
    }
};

class Exam {
public:
    std::string examTitle;               
    std::vector<Question> questions;     

    Exam(std::string title) : examTitle(title) {}

    Exam(const Exam& other)
        : examTitle(other.examTitle), questions(other.questions) {
        
        std::shuffle(questions.begin(), questions.end(), std::default_random_engine{});

        for (auto& question : questions) {
            question.shuffleOptions();
        }
    }

    void addQuestion(const Question& question) {
        questions.push_back(question);
    }

    void display(std::ofstream& htmlFile) const {
        htmlFile << "<h1>Exam: " << examTitle << "</h1>" << std::endl;
        for (size_t i = 0; i < questions.size(); ++i) {
            questions[i].display(htmlFile, i);
        }
        htmlFile << "<button onclick='calculateScore()'>Submit</button>" << std::endl;
        htmlFile << "<p id='score'></p>" << std::endl;
    }
};

Question getQuestionFromUser() {
    std::string text;
    std::vector<std::string> options(4);
    int correctOption;

    std::cout << "Enter question text: ";
    std::cin.ignore();
    std::getline(std::cin, text);

    for (int i = 0; i < 4; ++i) {
        std::cout << "Enter option " << i + 1 << ": ";
        std::getline(std::cin, options[i]);
    }

    std::cout << "Enter the number of the correct option (1-4): ";
    std::cin >> correctOption;

    return Question(text, options, correctOption - 1);
}

int main() {
    std::string title;
    int numQuestions;
    std::cout << "Enter exam title: ";
    std::getline(std::cin, title);
    Exam exam(title);
    std::cout << "Enter the number of questions: ";
    std::cin >> numQuestions;
    for (int i = 0; i < numQuestions; ++i) {
        std::cout << "Enter details for question " << i + 1 << ":\n";
        Question q = getQuestionFromUser();
        exam.addQuestion(q);
    }
    Exam examCopy = exam;
    std::ofstream htmlFile("exam.html");
    htmlFile << "<!DOCTYPE html>" << std::endl;
    htmlFile << "<html><head><title>Exam</title>" << std::endl;
    htmlFile << "<style>" << std::endl;
    htmlFile << "body { font-family: Arial, sans-serif; margin: 40px; background-image: url('https://hdqwalls.com/wallpapers/minimalism-4k-lu.jpg');background-size: cover; background-repeat: no-repeat; background-attachment: fixed; }" << std::endl;
    htmlFile << "h1 { color: #333; }" << std::endl;
    htmlFile << ".question { margin-bottom: 20px; padding: 10px; border: 1px solid #ddd; border-radius: 5px; background-color: rgba(255, 255, 255, 0.8); }" << std::endl;
    htmlFile << ".question p { margin: 0 0 10px; font-weight: bold; }" << std::endl;
    htmlFile << ".options { list-style-type: none; padding: 0; }" << std::endl;
    htmlFile << ".options li { margin: 5px 0; }" << std::endl;
    htmlFile << "input[type='radio'] { margin-right: 10px; }" << std::endl;
    htmlFile << "button { padding: 10px 20px; background-color: #4CAF50; color: white; border: none; border-radius: 5px; cursor: pointer; }" << std::endl;
    htmlFile << "button:hover { background-color: #45a049; }" << std::endl;
    htmlFile << "</style>" << std::endl;
    htmlFile << "<script>" << std::endl;
    htmlFile << "function calculateScore() {" << std::endl;
    htmlFile << "  let score = 0;" << std::endl;
    htmlFile << "  let totalQuestions = " << examCopy.questions.size() << ";" << std::endl;
    htmlFile << "  for (let i = 0; i < totalQuestions; i++) {" << std::endl;
    htmlFile << "    let correctOption = document.getElementById('correct_q' + i).value;" << std::endl;
    htmlFile << "    let radios = document.getElementsByName('q' + i);" << std::endl;
    htmlFile << "    for (let j = 0; j < radios.length; j++) {" << std::endl;
    htmlFile << "      if (radios[j].checked && radios[j].value == correctOption) {" << std::endl;
    htmlFile << "        score++;" << std::endl;
    htmlFile << "      }" << std::endl;
    htmlFile << "    }" << std::endl;
    htmlFile << "  }" << std::endl;
    htmlFile << "  document.getElementById('score').innerText = 'Your score is: ' + score + ' out of ' + totalQuestions;" << std::endl;
    htmlFile << "}" << std::endl;
    htmlFile << "</script>" << std::endl;
    htmlFile << "</head><body>" << std::endl;
    examCopy.display(htmlFile);
    htmlFile << "</body></html>" << std::endl;
    htmlFile.close();
    std::cout << "Copied exam details have been written to exam.html" << std::endl;
    return 0;
}