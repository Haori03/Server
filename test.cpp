#include <iostream>
#include <limits>
#include "includer.h"
#include <UnitTest++/UnitTest++.h>
#include <random>
#include <thread>
SUITE(logger_test){
    TEST(incorrect_path){
        logger log;
        CHECK_THROW(log.write_log("_!@#$%^","test"),critical_error);
    }
    TEST(correct_path){
        logger log;
        int temp = log.write_log("log.txt","test");
        CHECK_EQUAL(0,temp);
    }
    TEST(empty_path){
        logger log;
        CHECK_THROW(log.write_log("","test"),critical_error);
    }
    TEST(newline_in_path){
        logger log;
        CHECK_THROW(log.write_log("lll.txt\n","test"),critical_error);
    }
}
SUITE(server_data_handler_test){
    TEST(max_overflow){
        server SERV;
        SERV.log_location = "log.txt";
        int64_t temp = std::numeric_limits<int64_t>::max();
        int64_t res = SERV.calculation(temp,temp);
        CHECK_EQUAL(std::numeric_limits<int64_t>::max(),res);

    }
    TEST(min_overflow){
        server SERV;
        SERV.log_location = "log.txt";
        int64_t temp = std::numeric_limits<int64_t>::min();
        int64_t res = SERV.calculation(temp,temp);
        CHECK_EQUAL(std::numeric_limits<int64_t>::min(),res);

    }
    TEST(standart_numbers){
        server SERV;
        SERV.log_location = "log.txt";
        int64_t temp = 123;
        int64_t res = SERV.calculation(temp,temp);
        CHECK_EQUAL(temp+temp,res);

    }
    TEST(negative_nums){
        server SERV;
        SERV.log_location = "log.txt";
        int64_t temp = -123;
        int64_t res = SERV.calculation(temp,temp);
        CHECK_EQUAL(temp+temp,res);
    }
    TEST(num_with_point){
        server SERV;
        SERV.log_location = "log.txt";
        int64_t temp = -123.123;
        int64_t res = SERV.calculation(temp,temp);
        CHECK_EQUAL(-246,res);
    }
}
SUITE(server_test){
    TEST(salt_gen_test){
        server SERV;
        SERV.log_location = "log.txt";
        std::string salt1 = SERV.SALT_generate();
        std::chrono::milliseconds duration(1000);
        std::this_thread::sleep_for(duration);
        std::string salt2 = SERV.SALT_generate();
        if(salt1==salt2){
            CHECK(false);
        }
        else{
            CHECK(true);
        }
    }
    TEST(length_of_salt){
        server SERV;
        SERV.log_location = "log.txt";
        std::string salt1 = SERV.SALT_generate();
        if(salt1.length()==16){
            CHECK(true);
        }
        else{
            CHECK(false);
        }
    }
    TEST(different_passes_hash_gen){
        server SERV;
        SERV.log_location = "log.txt";
        std::string salt1 = SERV.SALT_generate();
        std::string p1="zxcvb";
        std::chrono::milliseconds duration(1000);
        std::this_thread::sleep_for(duration);
        std::string salt2 = SERV.SALT_generate();
        std::string p2="fghjk";
        if(SERV.hash_gen(salt1,p1)!=SERV.hash_gen(salt2,p2)){
            CHECK(true);
        }
        else{
            CHECK(false);
        }

    }
    TEST(equal_passes_hash_gen){
        server SERV;
        SERV.log_location = "log.txt";
        std::string salt1 = SERV.SALT_generate();
        std::string p1="qwerty";
        if(SERV.hash_gen(salt1,p1)==SERV.hash_gen(salt1,p1)){
            CHECK(true);
        }
        else{
            CHECK(false);
        }
    }

}
SUITE(base_test){
    TEST(correct_path){
        CHECK_THROW(base_reader b("base.txt","log.txt"),critical_error);
    }
    TEST(incorrect_path){
        CHECK_THROW(base_reader b("a.txt","log.txt"),critical_error);
    }
    TEST(newline_in_path){
        CHECK_THROW(base_reader b("base\n.txt","log.txt"),critical_error);
    }
    TEST(empty_path){
        CHECK_THROW(base_reader b("","log.txt"),critical_error);
    }
    TEST(get_logins){
        base_reader b("base.txt","log.txt");
        std::vector<std::string> logins = b.get_logins();
        CHECK_EQUAL("login",logins[0]);
    }
    TEST(get_passwords){
        base_reader b("base.txt","log.txt");
        std::vector<std::string> pass = b.get_passwords();
        CHECK_EQUAL("password",pass[0]);
    }
}
int main(){
    return UnitTest::RunAllTests();
}