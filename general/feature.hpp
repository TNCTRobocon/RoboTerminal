#if 0

/*
#include <map>
#include <string>
#include <tuple>

//std::tuple<std::string, std::string, std::string>
//using FACTORS_MAX = 10;

using namespace std;



enum Factors{
  TYPE=0,DETAIL,OPTION
};

class Feature{
private:
  map<int,tuple<string,string,string>> device;
public:
  Feature(){
    //device[16] = make_tuple("motor","dc-motor","encoder");
    //get<TYPE>(device[16]);
  }
};
*/
#pragma once
#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class Feature;

using address_t = uint32_t;
using factor_t = std::string;
using csv_factors_t = std::string;
// using feature_sptr = std::shared_ptr<Feature>;

class Feature {
private:
    std::map<address_t, std::unordered_set<factor_t>> ft;
    std::unordered_set<factor_t> temp;

public:
    // Feature();
    // static inline std::unique_ptr<Feature> GenerateFeature(){
    //  return std::unique_ptr<Feature>(new Feature());
    //}
    void Read_csv(csv_factors_t);
    bool WhoAmI(
        address_t,
        csv_factors_t);  //与えられた文字列が正しく機能を表していればtrueを返す
};

#endif
