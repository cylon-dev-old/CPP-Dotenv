/*
Dotenv.hpp - A small utility module for working with enviroment variables
           - Zero dependencies and a single .hpp file for ease of use
Creator: Seniatical @Seniatical -> cylon.wtf
Maintainer: Seniatical @Seniatical
*/

#ifndef DOT_ENV_H
#define DOT_ENV_H

#include <iostream>
#include <stdlib.h>
#include <map>
#include <fstream>
#include <string>
#include <regex>
#include <variant>

#include <stdexcept>
#include <cerrno>

auto _getenv = &getenv;
auto _setenv = &setenv;

namespace dotenv {
    std::string whitespace = " \t\n\r\x0b\x0c";
    int SET_ENV_ERROR_CODE = -1;
    auto LINE_RE = std::regex("^ *([a-zA-Z_]*) *= *(([0-9]+\\.[0-9]+)|([0-9]+)|(true|false)|(\".*\")|)(?: *(?:#.*)?)");

    /* TRIM FUNCTIONS */

    std::string& leftTrim(std::string& str, std::string& chars) {
        str.erase(0, str.find_first_not_of(chars));
        return str;
    }

    std::string& rightTrim(std::string& str, std::string& chars) {
        str.erase(str.find_last_not_of(chars) + 1);
        return str;
    }

    std::string& trim(std::string& str, std::string& chars) {
        return leftTrim(rightTrim(str, chars), chars);
    }

    /* PARSING FUNCTIONS */

    bool parse_line(std::string line, bool warn_on_error, int lineno, std::string &name, std::string &value) {
        if (line.rfind("#", 0) == 0) {   // Ignore comments
            return false;
        }
        std::smatch matchRes;

        if (!(std::regex_search(line, matchRes, LINE_RE))) {
            if (warn_on_error) {
                std::cerr << std::to_string(lineno) << "| Incorrectly formed line" << std::endl;
            }
            return false;
        }

        name = matchRes.str(1);
        value = matchRes.str(2);

        // NULL
        if (value == "") {
            value = "01a";
        }

        return true;
    }

    /* RESULT CLASS Definition */

    class envItem {
        public:
            std::string item;
            bool can_parse;
            std::string k_name;

            static envItem create(
                std::string item, 
                bool can_parse, 
                std::string k_name = ""
            ) {
                envItem itemWrapper;
                itemWrapper.item = item;
                itemWrapper.can_parse = can_parse;
                itemWrapper.k_name = k_name;

                return itemWrapper;
            }

            std::variant<int, double, bool, std::string, long int> parse() {
                // <ENV> - 0, 4
                if (!can_parse) {return item;}
                else if (item == "01a") {return NULL;}
                else if (item[0] == '"' && item.back() == '"') {return item.substr(1, item.size() - 2);}
                else if (item == "true") {return true;}
                else if (item == "false") {return false;}

                try {return std::stod(item);}
                catch (std::invalid_argument _) {
                    try {return std::stoi(item);}
                    catch (std::invalid_argument _) {return item;}
                }
            }
    };

    /* DOTENV CLASS */

    class dotenv {
        public:
            std::string dotenv_path = "./.env";
            std::map<std::string, std::string> vars = {};
            bool overwrite = false;
            bool ignore_whitespace = true;
            bool warn_on_overwrite = true;
            bool warn_on_error = true;
            bool throw_err = false;
            bool warn_on_set_err = true;
            bool warn_not_found = true;

            static dotenv load_dotenv(
                std::string dotenv_path = "./.env",
                std::map<std::string, std::string> vars = {},
                bool overwrite = false,
                bool ignore_whitespace = true,
                bool warn_on_error = true,
                bool throw_err = false,
                bool warn_on_set_err = true
            ) {
                dotenv NewEnv;

                NewEnv.dotenv_path = dotenv_path;
                NewEnv.vars = vars;
                NewEnv.overwrite = overwrite;
                NewEnv.ignore_whitespace = ignore_whitespace;
                NewEnv.warn_on_error = warn_on_error;
                NewEnv.throw_err = throw_err;
                NewEnv.warn_on_set_err = warn_on_set_err;

                NewEnv.parse_env();
                NewEnv.load_variables();

                return NewEnv;
            }

            void parse_env() {
                int lineno = 0;
                std::string line;
                std::ifstream envFile(dotenv_path);

                if (envFile.fail()) {
                    if (throw_err == true) {
                        throw std::invalid_argument("Failed to open .env file, check permissions and provided file path");
                    }
                    else {
                        std::cerr << "Failed to open .env file, check permissions and provided file path" << std::endl;
                    }
                    return;
                }

                while (std::getline(envFile, line)) {
                    lineno = lineno + 1;

                    if (envFile.eof()) {
                        break;
                    }

                    if ((line.find_first_not_of(whitespace) == std::string::npos) && ignore_whitespace) {
                        continue;
                    }

                    std::string name, value;
                    bool result = parse_line(line, warn_on_error, lineno, name, value);

                    if (!result) {
                        continue;
                    }

                    to_load[name] = value;
                    line.clear();
                };

                envFile.close();
            }

            void load_variables() {
                int flag = 0;

                to_load.insert(vars.begin(), vars.end());

                if (overwrite) {
                    flag = 1;
                }

                for (auto const& [key, value]: to_load) {
                    int statusCode = _setenv(const_cast<char*>(key.c_str()),
                                            const_cast<char*>(value.c_str()),
                                            flag);

                    if (statusCode == SET_ENV_ERROR_CODE) {
                        if (errno == EINVAL) {
                            std::cerr << "The name argument is a null pointer, points to an empty string, or points to a string containing an '=' character."
                                      << std::endl
                                      << "ERRNO_ACT: " << strerror(errno)
                                      << std::endl;
                        }
                        else if (errno == ENOMEM) {
                            std::cerr << "Insufficient memory was available to add a variable or its value to the environment."
                                      << std::endl
                                      << "ERNO_ACT: " << strerror(errno)
                                      << std::endl;
                        }
                        else {
                            std::cerr << "Cannot describe error :: ERRNO_NUM " << errno
                                      << std::endl
                                      << "ERRNO_ACT: " << strerror(errno)
                                      << std::endl;
                        }
                    }
                }

                to_load.clear();
            }

            void setenv(std::string key, std::string value, bool __overwrite = false) {
                bool o_ = overwrite;
                this->overwrite = __overwrite;

                to_load[key] = value;
                load_variables();
                this->overwrite = o_;
            }

            static envItem getenv(std::string key, bool can_be_parsed = true) {
                try {
                    std::string value = _getenv(const_cast<char*>(key.c_str()));
                    return envItem::create(value, can_be_parsed);
                }
                // NULL returned = KEY dont exist
                catch (std::logic_error _) {
                    std::cerr << "Key '" << key << "' Not found, returning NULL value" << std::endl; 

                    return envItem::create("01a", true, key);
                }
            }

            static bool delvar(std::string key) {
                if (_setenv(const_cast<char*>(key.c_str()), NULL, 1) != 0) {
                    return false;
                }
                return true;
            }

            static bool clearenv() {
                if (clearenv() != 0) {
                    return false;
                }
                return true;
            }

            /* OPERATORS */
            const envItem operator[](const std::string key) {
                return getenv(key, true);
            }

        private:
            std::map<std::string, std::string> to_load;
    };

}

#endif

