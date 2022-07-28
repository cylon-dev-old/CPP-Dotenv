# CPP-Dotenv
A small module for working with .env files in CPP.

## Installation
Simply shove the header file were it is needed and code right away, it is that simple.

```sh
curl https://github.com/cylon-dev/CPP-Dotenv/src/dotenv.hpp -o dotenv.hpp
```

## Usage
Using the module has never been easier.

### Setting up
```cpp
#include "dotenv.hpp"   // Include the header file

auto env = dotenv::dotenv::load_dotenv();   // Intialise the class once and your ready to go
```

Now lets access your variables
```cpp
// Using the intialised class
env.getenv("Foo");
env["Foo"];

// Or use std::getenv || READ NEXT SECTION!!!
std::getenv("Foo");

// To maintain type conversions, the getenv method in dotenv is static :D
dotenv::dotenv::get_env("Foo");
```

### Variable assigning and handling
* **Comments**: Defined by using `#` (Ignored if in quotations)
* **Strings**: type[std::string] : `VAR = "String # Here"` -> `getenv("VAR") == "String # Here"`
* **Integers**: type[int] : `VAR = 10` -> `getenv("VAR") == 10`
* **Floats**: type[double] : `VAR = 0.1` -> `getenv("VAR") == 0.1`
* **Booleans** type[bool] : `VAR = true` -> `getenv("VAR") == true`   | Case insensitive
* **NULL** type[NULL] : `VAR =` -> `getenv("VAR") == NULL`

> ⚠️ Variable types will only be handled if your using `dotenv::dotenv::getenv`
You will only get handled types by using the classes getenv method.

The regular `std::getenv` will return all vars with the type `char*`

* Strings will be fetched with quotations wrapped around
* int, float and bool will all remain the same `char* var = "10" "0.1" "true"`
* NULL will be equal to `char* var = "01a"`

### Adding new variables
```cpp
// Using the class :: Works statically as well :O
env::setenv(NAME, VAR);

// Standard function will also work, You just wont get any verification before its added
setenv(NAME, VAR);
```

### Deleting variables
COMING SOON

### Editing existing variables
COMING SOON

### Clearing environment
COMING SOON

## Error Reporting
...

## Signitures
**dotenv::dotenv::load_dotenv
```cpp
static dotenv load_dotenv(
    std::string dotenv_path = "./.env",           // path to .env file
    std::map<std::string, std::string> vars = {}, // additional variables that are not found in .env
                                                  // - Will overwrite any variables from the .env file
    bool overwrite = false,                       // whether to overwrite exisiting enviroment variables
    bool ignore_whitespace = true,                // whether to ignore lines that just contain whitespace
    bool warn_on_overwrite = true,                // whether to warn before overwriting variable
    bool warn_on_error = true,                    // whether to warn when an error occurs through parsing
    bool throw_err = false,                       // whether to throw errors which can be critical - .env file can't opened
    bool warn_on_set_err = true                   // whether to disable error tracking
);
```
