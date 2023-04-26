#include <chrono>
#include <fstream>
#include <iostream>

#include <date/date.h>
#include <nlohmann/json.hpp>

#include "internship.h"

using json = nlohmann::json;
using namespace date;

using day_point = std::chrono::time_point<std::chrono::system_clock, days>;

 
namespace internship {

    void quick_sort(int how_long[], std::string what_name[],std::string what_cycle[], int left, int right)
    {
       if(right >= left) return;
        
        int i = left - 1, j = right + 1, 
        middle = how_long[(left+right)/2]; 
        
        while(1)
        {
        
            while(middle<how_long[++i]);
        
            while(middle>how_long[--j]);
            
            if( i >= j){
                int help_int = 0;
                std::string help_name;
                std::string help_cycle;

                help_int = how_long[i];
                how_long[i] = how_long[j];
                how_long[j] = help_int;
                
                help_name = what_name[i];
                what_name[i] = what_name[j];
                what_name[j] = help_name;

                help_cycle = what_cycle[i];
                what_cycle[i] = what_cycle[j];
                what_cycle[j] = help_cycle;
              
            }
            else
                break;
        }

       
        if(j < left)
        quick_sort(how_long, what_name, what_cycle, left, j);
        if(i > right)
        quick_sort(how_long, what_name, what_cycle, i, right);
    }
 
    
   void anwser(const std::string& jsonFileName, int elementsCount) {
        std::ifstream f(jsonFileName);
        json data = json::parse(f);

        int how_long[elementsCount]= {};
        std::string what_name[elementsCount] = {};
        std::string what_cycle[elementsCount] = {};
        bool if_done_correctly = false;

        for (const auto& [id, product] : data.items()) {
            if(product.contains("name") && product.contains("os")){
                if(product["os"] == true){
                    for (const auto& version : product["versions"]) {
                        if(version.contains("releaseDate") && version.contains("eol") && version.contains("cycle")){
                            if(version["releaseDate"] != true  && version["releaseDate"] != false      //all those if help to make sure that
                            && version["releaseDate"] != nullptr && version["eol"] != true              //no invalid data were allowed in heart
                            && version["eol"] != false && version["eol"]!= nullptr){                    //of the function
                                std::string release = version["releaseDate"];                
                                std::string end = version["eol"];                 

                                std::tm tm_r = {};
                                std::istringstream ss_r(release);
                                ss_r >> std::get_time(&tm_r, "%Y-%m-%d");

                                std::chrono::system_clock::time_point tp_r = std::chrono::system_clock::from_time_t(std::mktime(&tm_r));
                                
                                day_point dp_r = std::chrono::time_point_cast<days>(tp_r);

                                std::tm tm_e = {};
                                std::istringstream ss_e(end);
                                ss_e >> std::get_time(&tm_e, "%Y-%m-%d");

                                std::chrono::system_clock::time_point tp_e = std::chrono::system_clock::from_time_t(std::mktime(&tm_e));
                                
                                day_point dp_e = std::chrono::time_point_cast<days>(tp_e);

                                std::chrono::duration<int, std::ratio<60*60*24>> days = std::chrono::duration_cast<std::chrono::duration<int, std::ratio<60*60*24>>>(tp_e - tp_r);
                                //all the operations above help to count how many days have passed betweenthe release date
                                //and end of the license of the operation system

                                for (int i =0; i < elementsCount; i++){     //this for helps to make sure that only values with
                                    if (days.count() + 1 > how_long[i]){    //the longest operating system support period
                                        how_long[i] = days.count() + 1;
                                        what_name[i] = product["name"];
                                        what_cycle[i] = version["cycle"];
                                        break;
                                    }   
                                }

                                if_done_correctly = true;
                            }
                        }
                    }
                }
            }
        }

        if(if_done_correctly == true){
            quick_sort(how_long, what_name, what_cycle, 0, elementsCount-1); 

            for (int i =0; i< elementsCount; i++){
                if(how_long[i] == 0){ //if there is not enough element the function make an exit
                    return;
                }
                
                std::cout << what_name[i] << " " << what_cycle[i] << " " << how_long[i] <<"\n";

            } 
        }
        else{ 
            return;
        }
    }
   
    void solution(const std::string& jsonFileName, int elementsCount) {
        anwser(jsonFileName, elementsCount); // call of the function I prepered

    }
}