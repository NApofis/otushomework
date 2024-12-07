//
// Created by ntikhonov on 07.12.24.
//


#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string.h>
#include <iomanip>
#include <Eigen/Dense>

const size_t input_dim = 784;
const size_t hidden_dim = 128;
const size_t output_dim = 10;


class Classifier
{

    template <typename T>
    auto sigma(T x)
    {
        return 1 / (1 + std::exp(-x));
    }

    Eigen::VectorXf sigmav(const Eigen::VectorXf& v)
    {
        Eigen::VectorXf res{v.rows()};
        for (size_t i = 0; i < v.rows(); ++i)
        {
            res(i) = sigma(v(i));
        }
        return res;
    }

    Eigen::VectorXf softmax(const Eigen::VectorXf& v)
    {
        Eigen::VectorXf res{v.rows()};
        float denominator = 0.0f;

        for (size_t i = 0; i < v.rows(); ++i)
        {
            denominator += std::exp(v(i));
        }
        for (size_t i = 0; i < v.rows(); ++i)
        {
            res(i) = std::exp(v(i)) / denominator;
        }
        return res;
    }


    Eigen::MatrixXf w1, w2;


public:
    struct FileMatrix
    {
        size_t rows;
        size_t cols;
        std::string file;
    };


    Classifier(const FileMatrix& m1, const FileMatrix& m2)
    {
        w1 = std::move(get_transpose_matrix_from_file(m1));
        w2 = std::move(get_transpose_matrix_from_file(m2));
    }

    [[nodiscard]] long num_classes() const
    {
        return w2.cols();
    }

    [[nodiscard]] std::vector<float> predict_proba(const std::vector<float>& feat)
    {
        Eigen::VectorXf x{feat.size()};
        for (size_t i = 0; i < feat.size(); ++i)
        {
            x[i] = feat[i] / 255;
        }

        Eigen::VectorXf val1 = w1 * x;
        auto o1 = sigmav(val1);
        Eigen::VectorXf val2 = w2 * o1;
        auto o2 = softmax(val2);

        std::vector<float> res;
        for (size_t i = 0; i < o2.rows(); ++i)
        {
            res.push_back(o2(i));
        }
        return res;
    }

    [[nodiscard]] long predict(const std::vector<float>& feat)
    {
        auto proba = predict_proba(feat);
        auto argmax = std::max_element(proba.begin(), proba.end());
        return std::distance(proba.begin(), argmax);
    };

    template<typename T>
    static bool is_near(T val1, T val2, T abs_error)
    {
        const auto diff = std::abs( static_cast<long>(val1 - val2) );
        return diff <= abs_error;
    }

    static bool read_features(std::istream& stream, size_t& result, std::vector<float>& features)
    {

        features.clear();

        std::string line;
        std::getline(stream, line);
        if (line.empty())
        {
            return false;
        }

        char* token = std::strtok(const_cast<char*>(line.data()), ",");

        std::vector<std::string> words;
        while (token != nullptr)
        {
            words.push_back(token);
            token = std::strtok(nullptr, ",");
        }
        result = std::stol(words[0]);
        for (auto it = words.begin() + 1; it != words.end(); ++it)
        {
            features.push_back(std::stof(*it));
        }

        return true;
    }

    static Eigen::MatrixXf get_transpose_matrix_from_file(const FileMatrix& m)
    {
        std::ifstream stream{m.file};
        Eigen::MatrixXf res(m.rows, m.cols);
        for (size_t i = 0; i < m.rows; ++i)
        {
            for (size_t j = 0; j < m.cols; ++j)
            {
                float val;
                stream >> val;
                res(i, j) = val;
            }
        }
        return res.transpose();
    }
};


int main(int argc, char* argv[])
{
    if (argc != 2 && argc != 4)
    {
        std::cerr << "Нужно передать тестовый файл. Или тестовый файл, файл с матрицей 1 и файл с матрицей 2" << std::endl;
        return EXIT_FAILURE;
    }

    std::string file1 = "w1.txt";
    std::string file2 = "w2.txt";
    if (argc == 4)
    {
        file1 = argv[2];
        file2 = argv[3];
    }
    auto clf = Classifier(
        {input_dim, hidden_dim, file1},
        {hidden_dim, output_dim, file2}
    );


    std::ifstream test_data(argv[1]);
    std::vector<float> features;
    size_t y_true;
    float all_counter = 0;
    float true_counter = 0;
    while (Classifier::read_features(test_data, y_true, features))
    {
        all_counter++;
        auto proba = clf.predict(features);
        if (Classifier::is_near<size_t>(y_true, proba, 1e-5))
        {
            true_counter++;
        }
    }
    std::cout << true_counter / all_counter << std::endl;
    return EXIT_SUCCESS;
}
