#pragma once
#include "base.hpp"
#include <pch.hpp>
namespace General {
    
    




    class CommandElement{
        const std::string command;
        const std::vector<std::string> parameters;
    public:
        CommandElement(const std::string& _command,const std::vector<std::string>& _parameter):
            command(_command),parameters(_parameter){}
        
        static CommandElement Decode(const std::string& line);
        std::string ToString()const;
    };
    using CommandReceiver= std::function<void(const std::string&,const std::vector<std::string>&)>;
    struct CommandHandler{    
        CommandElement message;
        CommandReceiver receiver;  
    };







}