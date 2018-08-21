#include "feature.hpp"

#include <map>
#include <string>
#include <iostream>
#include <unordered_set>

//const static string XX[]...

//Feature::Feature(){
//}

void Feature::Read_csv(csv_factors_t str){
  auto first = str.begin();
  while(first != str.end()){
    auto last = first;
    while(last != str.end() && *last != ','){
      ++last;
    }
    temp.emplace(std::string(first, end));
    if(last != str.end()){
      ++last;
    }
    first = last;
  }
}

bool Feature::WhoAmI(address_t address, csv_factors_t response){
  if(!response.empty()){
    if(response.back() == '\r'){
      response.pop_back();
      Read_csv(response);
      ft[address] = temp;
      return true;
    }
  }
  return false;
}
