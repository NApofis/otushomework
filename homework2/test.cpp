//
// Created by ntikhonov on 07.07.24.
//

#include <fstream>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include <iomanip>

#include "ip_filter.h"
#include "../googletest/googletest/include/gtest/gtest.h"
#include "info.h"

TEST(ReaderTest, BasicAssertions) {
    const auto result1 = read_id("100.100.100.100 world world world world", ' ');
    constexpr decltype(result1) test1 = {true, {100, 100, 100, 100}};
    ASSERT_TRUE(result1 == test1);

    const auto result2 = read_id("100.100.100.100 world", '-');
    constexpr decltype(result2) test2 = {false, {}};
    ASSERT_TRUE(result2 == test2);

    const auto result3 = read_id("101.102.103.104\t2\t3", '\t');
    constexpr decltype(result3) test3 = {true, {101, 102, 103, 104}};
    ASSERT_TRUE(result3 == test3);

    const auto result4 = read_id("", '\t');
    constexpr decltype(result4) test4 = {false, {}};
    ASSERT_TRUE(result4 == test4);

    const auto result5 = read_id("101.102.103.104", ' ');
    constexpr decltype(result5) test5 = {false, {}};
    ASSERT_TRUE(result5 == test5);
}

TEST(FilterTest, BasicAssertions) {
    const std::list<ip_type> test = {{1, 2, 4, 4}, {1, 1, 2, 3}, {2, 3, 4, 5}, {1, 2, 3, 5}};

    std::stringstream ss;
    const std::string result1 = "2.3.4.5\n";
    filter_data(test, {{2, }, false}, ss);
    ASSERT_TRUE(ss.str() == result1);

    std::stringstream ss2;
    const std::string result2 = "1.2.3.5\n2.3.4.5\n";
    filter_data(test, {{5, 5, 5, 5}, true}, ss2);
    ASSERT_TRUE(ss2.str() == result2);

    std::stringstream ss3;
    const std::string result3 = "1.2.3.5\n1.1.2.3\n1.2.4.4\n";
    filter_data(test, {{1, 2}, true}, ss3);
    ASSERT_TRUE(ss3.str() == result3);

    std::stringstream ss4;
    const std::string result4 = "1.2.3.5\n2.3.4.5\n1.1.2.3\n1.2.4.4\n";
    filter_data(test, {}, ss4);
    ASSERT_TRUE(ss4.str() == result4);
}



TEST(BaseTest, BasicAssertions) {
    const short hash_sum_size = 16;

    std::ifstream f;
    std::stringstream out;
    f.open(CMAKE_CURRENT_SOURCE_DIR"/homework2/ip_filter.tsv");
    ASSERT_TRUE(f.is_open());

    start_filter(f, out);
    auto data = out.str();

    EVP_MD_CTX* md5Context = EVP_MD_CTX_new();

    EVP_MD_CTX_init(md5Context);
    EVP_DigestInit_ex(md5Context, EVP_md5(), nullptr);
    EVP_DigestUpdate(md5Context, data.c_str(), data.size());

    uint8_t result[hash_sum_size];
    EVP_DigestFinal_ex(md5Context, result, nullptr);
    EVP_MD_CTX_free(md5Context);

    std::stringstream hex;
    for (int i=0; i<hash_sum_size; i++) {
        hex << std::hex << std::setw(2) << std::setfill('0') << (int) result[i];
    }
    ASSERT_TRUE(hex.str() == "24e7a7b2270daee89c64d3ca5fb3da1a");
}
